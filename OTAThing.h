/////////////////////////////////////////////////////////////////////////////
// MyOTAThing.h
// COM3505 lab assessment: Over-The-Air update header
/////////////////////////////////////////////////////////////////////////////

#ifndef OTA_THING_H 
#define OTA_THING_H

#include <HTTPClient.h>
// Constants ////////////////////////////////////////////////////////////////
static const char ssid[32] = "uos-other";		        // the ssid to connect to
static const char password[32] = "shefotherkey05";	// the password to use when connecting
static const char gitID[13] = "peterkentish"; 	    // the git id being used
static const char * emptyString = "";               // empty string used in missing key checks
static const char binFileType[5] = ".bin";          // the bin file type which can be concatenated
                                                    // with the version number
static const char txtFileType[5] = ".txt";          // the key file type which can be concatenated
                                                    // with the version number
static const int onboardLEDPin = 13;			          // the pin of the onboard LED
static const int sliceSize = 70000;		            	// the value at which the loop will do some
							                                      // check

// Variables //////////////////////////////////////////////////////////
char MAC_ADDRESS[13]; 					                    // MAC addresses are 12 chars, plus the NULL
							                                      // terminator
unsigned long loopIterationOTA = 0;		                // the variable used to store the iteration
							                                      // of the loop

// OTA Methods ////////////////////////////////////////////////////////////////
int doCloudGet(HTTPClient *, String, String); 		  // helper for downloading from cloud
bool checkOTAStatus(int);	                          // checks if an OTA update is required
String getServerFileName(int);                      // creates server file name from a version number
String getServerFileKey(int);                       // creates server file name from a version number
bool checkUpdateKey(HTTPClient&, int);              // checks key matches that of server
bool getUpdateData(HTTPClient&, int *, int);        // gets the update data via a tcp request
void doOTAUpdate(int);                              // executes the OTA update

// MAC and IP helpers ///////////////////////////////////////////////////////
void getMAC(char *);					                      // gets the mac address of the current board
String ip2str(IPAddress);                 		      // helper for printing IP addresses

// LED utilities, loop slicing //////////////////////////////////////////////
void ledOn();						                            // turns the onboard LED on
void ledOff();					                            // turns the onboard LED off
void blink(int = 1, int = 300);				              // blinks the onboard LED

char* trimWhitespace(char *);                       // trims whitespace from string
#endif
