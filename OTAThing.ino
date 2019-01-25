/////////////////////////////////////////////////////////////////////////////
// MyOTAThing.ino
// COM3505 lab assessment: Over-The-Air update
/////////////////////////////////////////////////////////////////////////////

// the wifi and HTTP server libraries ///////////////////////////////////////
#include <WiFi.h>         // wifi
#include <ESPWebServer.h> // simple webserver
#include <HTTPClient.h>   // ESP32 library for making HTTP requests
#include <Update.h>       // OTA update library
#include <stdlib.h>       // Library used to cast string to int
#include "OTAThing.h"   // Header file for this OTA code
#include "WiFiThing.h"  // Header file for the WiFi connection code

int currentVersion = 9; // TODO keep up-to-date! (used to check for updates)
char* currentKey = ""; // Version 8 GUID: "4d45a48a-ffae-4abf-9cd0-2de563af4a6a"; // TODO keep up-to-date! (used to check for trust)

// SETUP: initialisation entry point ////////////////////////////////////////
void setupOTA() {
  
  getMAC(MAC_ADDRESS);          // store the MAC address
  Serial.printf("\nMyOTAThing setup...\nESP32 MAC = %s\n", MAC_ADDRESS);
  Serial.printf("firmware is at version %d\n", currentVersion);
  
  // WiFi.mode(WIFI_STA);
  Serial.print("Starting AP");
  startAP();            // fire up the AP...
  Serial.print("Starting Web Server");
  initWebServer();      // ...and the web server
  Serial.print("Starting WiFi");
  WiFi.begin(); 

  // Attempt to connect 15 times before moving on
  for (uint8_t i = 0; i < 15; i++) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Wifi connected");
      irrecv.enableIRIn(); // Enables the IR receiver. Fails if wifi isnt connected
    } else {
      delay(500);
      Serial.printf(".");
    }
    webServer.handleClient();
  }
}

// LOOP: task entry point ///////////////////////////////////////////////////
void loopOTA() {
  webServer.handleClient(); 
  
  
    Serial.println("In OTA loop");
    int highestAvailableVersion = -1;
    if (checkOTAStatus(&highestAvailableVersion)) {
      doOTAUpdate(highestAvailableVersion);
    }
 }

// Checks if an update is required, returning true if an update should be performed
// Passes version by reference so we can use this in the 'do' part of the update
bool checkOTAStatus(int *highestAvailableVersion) {
  HTTPClient http; // manage the HTTP request process
  int respCode;    // the response code from the request (e.g. 404, 200, ...)

  // do a GET to read the version file from the cloud
  Serial.println("checking for firmware updates...");
  respCode = doCloudGet(&http, gitID, "version");
  // check response code  (200 means success)
  if (respCode == 200) {
    *highestAvailableVersion = atoi(http.getString().c_str()); // gets highest version as an integer from its string form
  } else {
    Serial.printf("couldn't get version! rtn code: %d\n", respCode);
    return false;
  }
    
  http.end(); // free resources

  // check if current version is equal to or higher than highest available version
  if (currentVersion >= *highestAvailableVersion) {
    Serial.printf("firmware is up to date\n\n");
    return false;
  }
  return true;
}

// Creates a server file name from a version number
String getServerFile(int versionNumber) {
  char serverFileName[10]; // the variable that will store the name of the firmware file
  itoa(versionNumber, serverFileName, 10);
  strcat(serverFileName, binFileType);
  
  return serverFileName;
}

// Creates a server file name from a version number
String getServerKey(int versionNumber) {
  char serverFileKey[10]; // the variable that will store the name of the firmware file
  itoa(versionNumber, serverFileKey, 10);
  strcat(serverFileKey, txtFileType);
  
  return serverFileKey;
}

// Checks to see if the key in the current program matches that of the next program
bool checkUpdateKey(HTTPClient& http, int versionNumber) {
  // If no key is present in current program, then assume next update is equally trustworthy
  if (strcmp(currentKey, emptyString) == 0) {
    return true;
  }

  int respCode;    // the response code from the request (e.g. 404, 200, ...)
  char* nextKey = new char [40];  // the key currently on the server
  
  // do a GET to read the key file from the cloud
  Serial.println("checking for firmware key...");
  respCode = doCloudGet(&http, gitID, getServerKey(versionNumber));
  // check response code  (200 means success)
  if (respCode == 200) {
    strcpy(nextKey, http.getString().c_str()); // gets the key as a string and stores this in the next key variable
  } else {
    Serial.printf("couldn't get key! rtn code: %d\n", respCode);
    http.end();
    return false;
  }
  
  // free resources
  http.end();

  // check to see if server key of next version matches the current
  if (strcmp(trimWhitespace(nextKey), currentKey) == 0) {
    Serial.println("Key matches server so OTA will proceed");
    return true;
  } else {
    Serial.println("Key does not match, therefore source may not be trusted");
    return false;
  }
}

// Gets the update data via a tcp request
bool getUpdateData(HTTPClient& http, int *contentLength, int versionNumber) {
  int respCode;            // the response code from the request (e.g. 404, 200, ...)

  Serial.printf(
    "upgrading firmware from version %d to version %d\n",
    currentVersion, versionNumber
  );
  
  // do a GET to read the firmware from the cloud
  Serial.println("finding firmware file");
  
  respCode = doCloudGet(&http, gitID, getServerFile(versionNumber));
  // check response code  (200 means success)
  if (respCode == 200) {
    *contentLength = http.getSize();
    return true;
  } else {
    Serial.printf("couldn't get firmware! rtn code: %d\n", respCode);
    return false;
  }
}

// Completes an over the air update, looking for a firmware with the version number passed in to the function
void doOTAUpdate(int newVersion) {
  HTTPClient http;         // manage the HTTP request process
  http.setTimeout(10000);  // increase the timeout of a request
  int contentLength = 0;

  if (!checkUpdateKey(http, newVersion) || !getUpdateData(http, &contentLength, newVersion)) {
    return;
  }

  WiFiClient* stream; // the stream object that will be read from
  int percent = 0; // the percent variable that will be increased with each buffer write
  
  // check if there is enough space to begin the OTA Update. If there isn't, output a message to the serial
  // so the user knows this is the issue
  if (Update.begin(contentLength)) {
    Serial.println("Begin OTA. This may take a little while to complete. Things might be quiet for a while.");
    stream = http.getStreamPtr(); // get tcp stream
    size_t written; // initialise a size that will store how much has been written
    
    // while the http connection is connected
    while (http.connected()) {
      size_t remainingBytes = stream->available(); // get available data size
      int chunks = remainingBytes < 1024 ? remainingBytes : 1024; // get the chunk size (1024 or less if there is less remaining)
      uint8_t buff[chunks] = { 0 }; //create buffer for read

      // if it has a size, write bytes to Update otherwise break the while loop
      if (remainingBytes) {
        int bytesCount = stream->readBytes(buff, (sizeof(buff))); // read up the number of bytes to match the size of the buffer
        written += Update.write(buff, bytesCount); // write it to the device
        int current = (100 * written / contentLength); // compute the current percentage completed
        
        // if the new percentage is greater than the last percentage, and is a multiple of 5, print the percentage
        // to the serial
        if (percent < current && current % 5 == 0) {
          Serial.print(current);
          Serial.println("%");
        }
        percent = current; // update the last percentage
      } else {
        break;
      }
      delay(50); // add a bit of a delay between loops which helps success rate
    }

    // if the written amount is the same length as or longer than the original content length, then it should
    // have been written to the device successfully
    if (written >= contentLength) {
      Serial.println("Written : " + String(written) + " successfully");
    } else {
      Serial.println("Written only : " + String(written) + "/" + String(contentLength) + ". Trying again on next loop.");
    }

    // if the end function returns a successful boolean then the OTA has been completed, otherwise output the error
    // that update has run into
    if (Update.end()) {
      Serial.println("OTA done!");
      // if the update is finished then this means it has nothing left to right to the device
      // and so we know it has been successful. The device can then be restarted
      if (Update.isFinished()) {
        Serial.println("Update successfully completed. Rebooting.");
        ESP.restart();
      } else {
        Serial.println("There remains some data that hasn't been written to the device. Something went wrong!");
      }
    } else {
      Serial.println("Error Occurred. Error #: " + String(Update.getError()));
    }
  } else {
    Serial.println("Not enough space to begin OTA");
  }

  // free resources
  stream->flush();
  http.end(); 
}

// helper for downloading from cloud firmware server via HTTP GET
int doCloudGet(HTTPClient *http, String gitID, String fileName) {
  // build up URL from components; for example:
  // http://com3505.gate.ac.uk/repos/com3505-labs-2018-adalovelace/BinFiles/2.bin
  String baseUrl =
    "http://com3505.gate.ac.uk/repos/";
  String url =
    baseUrl + "com3505-labs-2018-" + gitID + "/BinFiles/" + fileName;

  Serial.println(url);
  // make GET request and return the response code
  http->begin(url);
  http->addHeader("User-Agent", "ESP32");
  return http->GET();
}

// misc utilities //////////////////////////////////////////////////////////
// get the ESP's MAC address
void getMAC(char *buf) { // the MAC is 6 bytes, so needs careful conversion...
  uint64_t mac = ESP.getEfuseMac(); // ...to string (high 2, low 4):
  char rev[13];
  sprintf(rev, "%04X%08X", (uint16_t) (mac >> 32), (uint32_t) mac);

  // the byte order in the ESP has to be reversed relative to normal Arduino
  for (int i = 0, j = 11; i <= 10; i += 2, j -= 2) {
    buf[i] = rev[j - 1];
    buf[i + 1] = rev[j];
  }
  buf[12] = '\0';
}



// Trims whitespace from a c string
char *trimWhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;
}
