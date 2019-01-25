// MyProjectThing.ino
// COM3505 2018 project template sketch. Do your project code here.
// Out of the box the sketch is configured to kick the tyres on all the
// modules, and allow stepping through tests via the touch screen. Change the
// TestScreen::activate(true); to false to change this behaviour.

#include "unphone.h"
#include "container.h"
#include "DrawingThing.h"
#include "ListeningThing.h"
#include "DoingThing.h"
#include "IRCodes.h"
#include "SendingThing.h"
#include "LearningThing.h"
#include "OTAThing.h"
#include "WiFiThing.h"

#include <ESPWebServer.h> // simple webserver
#include <HTTPClient.h> // simple webserver
#include <IRremote.h>
#include <SD.h>
#include <list>

// Stores the iteration of the loop
int loopIteration = 0;

// Stores the page currently being displayed on screen
Pages inMenuSection = Home;

// Stores the last touch position as input by user
uint16_t lastX;
uint16_t lastY;

// Sets up the IR Receiver
int recvPin = A0;
IRrecv irrecv(recvPin);
bool irReceiverAttached = false;

// Creates the containers for each tile on the screen
Container* box1 = new Container();
Container* box2 = new Container();
Container* box3 = new Container();
Container* box4 = new Container();
Container* box5 = new Container();
Container* box6 = new Container();

// Stores the magnitude of the last accelerometer reading so that the difference between the two can
// be calculated
int lastMagnitude;
// Stores a stack of the most recent ten magnitudes
std::list<int> magnitudeStack;

// Main setup function
void setup() {
  
  Wire.setClock(100000); // higher rates trigger an IOExpander bug
  UNPHONE_DBG = true;
  Serial.begin(115200);  // init the serial line
  setupOTA(); // Sets up the WiFi and OTA Update section
  // fire up I²C, and the unPhone's IOExpander library
  recoverI2C();
  Wire.begin();
  IOExpander::begin();
  
  checkPowerSwitch(); // check if power switch is now off

  // which board version are we running?
  int version = IOExpander::getVersionNumber(), spin;
  if (version == 7) spin = 4;
  Serial.printf("starting, running on spin %d\n", spin);

  // show initial test screen on the LCD
  IOExpander::digitalWrite(IOExpander::BACKLIGHT, LOW);
  tft.begin(HX8357D);
  TestScreen::activate(true);
  TestScreen::init();
  IOExpander::digitalWrite(IOExpander::BACKLIGHT, HIGH);

  if (! ts.begin()) { // init the touchscreen
    D("failed to start touchscreen controller");
    TestScreen::fail("TOUCH");
    delay(3000);
  } else {
    D("touchscreen started");
  }

  if (!accel.begin()) // set up the accelerometer
  {
    D("Failed to start accelerometer");
    TestScreen::fail("ACCEL");
    delay(3000);
  }

  i2s_config(); // configure the I2S bus

  pinMode(IR_LEDS, OUTPUT); // IR_LED pin

  // set up the SD card
  IOExpander::digitalWrite(IOExpander::SD_CS, LOW);
  if (!SD.begin(-1)) {
    D("Card Mount Failed");
    TestScreen::fail("SD CARD");
    delay(3000);
  }
  IOExpander::digitalWrite(IOExpander::SD_CS, HIGH);

  if (! musicPlayer.begin()) { // initialise the music player
    D("Couldn't find VS1053, do you have the right pins defined?");
    TestScreen::fail("AUDIO");
    delay(3000);
  } else {
    D("VS1053 found");
  }

  // send a LoRaWAN message to TTN
  lmic_init();
  lmic_do_send(&sendjob);

  //THESE TITLES MUST NOT CHANGE
  //char* arrays that will be used to fill the containers
  char* defaultBrand = "Technika";
  char* power = "Power";
  char* volUp = "Vol up";
  char* volDown = "Vol down";
  char* chanUp = "Chan up";
  char* chanDown = "Chan down";
  char* settings = "Settings";
  // Initialises each of the containers with some default value
  box1->init(power, defaultBrand, HX8357_RED);
  box2->init(volUp, defaultBrand, HX8357_GREEN);
  box3->init(chanUp, defaultBrand, HX8357_BLUE);
  box4->init(volDown, defaultBrand, HX8357_GREEN);
  box5->init(chanDown, defaultBrand, HX8357_BLUE);
  box6->init(settings, defaultBrand, HX8357_WHITE);

  pinMode(25,INPUT);
  analogReadResolution(12);  // 10 bit is 0-1023, 11 bit is 0-2047, 12 bit is 0-4095
  analogSetPinAttenuation(25, ADC_6db); // 0db is 0-1V, 2.5db is 0-1.5V, 6db is 0-2.2v, 11db is 0-3.3v
  
  //setupSD(); // Tries to write to the SD card- couldn't get implemented properly.
}

// Main loop
void loop() {
  checkPowerSwitch(); // check if power switch is now off
  webServer.handleClient(); // handles the web client that users can connect to

  // Gets the accelerometer event
  sensors_event_t event;
  accel.getEvent(&event);

  // Calculate magnitude of the acceleration and subtract the previous magnitude. This will get the
  // difference between the two.
  int magnitude = event.acceleration.x+event.acceleration.y+event.acceleration.z;
  magnitudeStack.push_back(magnitude-lastMagnitude); // Push the difference onto the stack
  lastMagnitude = magnitude; // Update the last magnitude that was stored.

  // If the size is greater than 10, then pop the first value of the stack
  if (magnitudeStack.size() > 10) {
    magnitudeStack.pop_front();
  }
  
  //TestScreen::testSequence(usbPowerOn); // run a test on all modules
  
  // Draw the screen if this is the very first loop or if the user is in the scanning a remote signal
  // and a signal change has been detected.
  if (loopIteration == 0 || (inMenuSection == Scanning && learnCode())) {
    draw();
  }
  // Listen to the user shaking the device on any page that can go back using the function
  // (everything except the home page)
  if (inMenuSection != Home) {
    listenToShake();
  }
  // Always listen for touches on the screen
  listenToScreen();
  loopIteration++;
}

// Get the containers as an iterable collection
Container** getContainers() {
  return new Container*[6] {box1, box2, box3, box4, box5, box6};
}

// Draws the graphics on the screen
void draw() {
  // Clear screen
  tft.fillScreen(HX8357_BLACK);
  // Text Size and wrapping are standard, so they get set here ready for use across all drawing functions
  tft.setTextSize(2);
  tft.setTextWrap(true);
  // Draw graphics on dependent on what menu section is currently selected
  switch(inMenuSection) {
    case Home:
      drawMainRemote(getContainers());
      break;
    case Settings:
      drawSettings(getContainers());
      break;
    case GlobalSettings:
      drawGlobalSettings();
      break;
    case Management:
      drawOptionsList(actions, actionLength);
      break;
    case Manufacturers:
    case GlobalManufacturers:
      drawOptionsList(manufacturers, manufacturerLength);
      break;
    case Colours:
      drawColoursList(colours, colourLength);
      break;
    case Functions:
      drawOptionsList(functions, functionLength);
      break;
    case Scanning:
      drawRemoteScan();
      break;
  }
}

// Listens for touches on the screen
void listenToScreen() {
  uint16_t x, y;
  uint8_t z;
  if (ts.touched()) {
    // Read in the x, y and z values
    while (!ts.bufferEmpty()) {
      ts.readData(&x, &y, &z);
    }
    ts.writeRegister8(STMPE_INT_STA, 0xFF); // reset all ints

    // Calculate the total difference between the previous and current coordinates
    int totalDifference = lastX + lastY - x - y;
    // If there has been a z detection and a sizeable change of the difference, then
    // a touch has been made. This is to help cover double presses.
    if (z > 0 && (totalDifference > 100 || totalDifference < -100)) {
      // Listen to touch screen presses dependent on the menu section that the user is currently in
      switch (inMenuSection) {
        case Home:
          listenToTileList(x, y, getContainers(), false);
          break;
        case Settings:
          listenToTileList(x, y, getContainers(), true);
          break;
        case GlobalSettings:
          listenToGlobalSettings(x, y);
          break;
        case Management:
          listenToActionList(x, y);
          break;
        case Manufacturers:
          listenToManufacturerList(x, y, false);
          break;
        case GlobalManufacturers:
          listenToManufacturerList(x, y, true);
          break;
        case Functions:
          listenToFunctionList(x, y);
          break;
        case Colours:
          listenToColourList(x, y);
          break;
        case Scanning:
          listenRemoteScan(x, y);
          break;
      }
      // Update the previous touch coordinates
      lastX = x;
      lastY = y;
    }
  }
}

// This method listens for the user shaking the unphone and will move back a page if the shaking has occurred beyond
// some threshold
void listenToShake() {
  // Count the amount of shakes that have been over the theshold
  int shakeCount = 0;
  // Iterate over the 10 magnitudes in the stack
  for(std::list<int>::iterator num = magnitudeStack.begin(); num != magnitudeStack.end(); ++num) {
    int magnitude = *num; 
    // If the value is greater than 2 then increase the shake count
    if (magnitude > 2) {
      shakeCount++;
    }
  }
  // If the shake count is over 10, then we deduce that the device has been shaken so go back a page.
  if (shakeCount > 2) {
    switch (inMenuSection) {
      case Settings:
        inMenuSection = Home;
        break;
      case Management:
        inMenuSection = Settings;
        break;
      case GlobalSettings:
        inMenuSection = Settings;
        break;
      case Manufacturers:
        inMenuSection = Management;
        break;
      case Functions:
        inMenuSection = Management;
        break;
      case Colours:
        inMenuSection = Management;
        break;
      case Scanning:
        inMenuSection = Management;
        break;
      default:
        return;
    }

    // Ensure the pages are reset
    page = 0;
    lastPage = 0;
    // Ensure the magnitude stack has been cleared
    magnitudeStack.clear();
    // Draw the screen again
    draw();
  }
}
