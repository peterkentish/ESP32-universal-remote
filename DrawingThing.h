// DrawingThing.h
// core definitions and includes

#ifndef DRAWINGTHING_H
#define DRAWINGTHING_H

// The pages that can be navigated to.
enum Pages { Home, Settings, GlobalSettings, Management, Manufacturers, Colours, Functions, Scanning, GlobalManufacturers };

// The maximum voltage of the battery
const float maxBatt = 4.2;
// The minimum voltage of the battery
const float minBatt = 3.0;

// The number of columns to display when displaying tiles
const uint8_t columns = 2;
// The number of rows to display when displaying tiles
const uint8_t rows = 3;

// The actions the user can carry out on a tile/container
const uint8_t actionLength = 4;
const char *actions[actionLength] { "Change manufacturer", "Change colour", "Change tile label", "Scan action from remote" };

// The colours available for a user to select when modifying a tile
const uint8_t colourLength = 4;
const uint16_t colours[colourLength] { HX8357_WHITE, HX8357_RED, HX8357_BLUE, HX8357_GREEN };

// The manufacturers available for a user to select when modifying a tile
const uint8_t manufacturerLength = 7;
const char *manufacturers[manufacturerLength] { "Sony", "Panasonic", "LG", "JVC", "Sharp", "Samsung", "Technika"};
const char* customManufacturer = "Custom";

// The functions available to put on a tile when modifying it
const uint8_t functionLength = 9;
const char *functions[functionLength] { "Power", "Chan up", "Chan down", "Vol up", "Vol down", "Source", "Guide", "Media", "Custom" };

uint8_t page = 0; // The current page being looked at
uint8_t lastPage = 0; // The index of the last item that was iterated over in a list.

uint8_t listRows = 4; // The number of items to display when viewing items in list form.
uint8_t border = 8; // The border to be given when rendering anything
uint16_t buttonHeight = HX8357_TFTHEIGHT / listRows - border; // The height of list buttons
uint16_t buttonWidth = HX8357_TFTWIDTH - border; // The width of list buttons

// This method draws the main remote tiles on the screen
void drawMainRemote(Container**);
// This method draws the setting tiles on the screen
void drawSettings(Container**);
// This method draws the additional global settings on the screen
void drawGlobalSettings();
// This method draws the options given in a list
void drawOptionsList(const char**, uint8_t);
// This method draws the colours given in a list
void drawColoursList(const uint16_t*, uint8_t);
// This method draws the IR remote scanning screen
void drawRemoteScan();
#endif
