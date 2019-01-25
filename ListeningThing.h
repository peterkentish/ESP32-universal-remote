// ListeningThing.h
// core definitions and includes

#ifndef LISTENINGTHING_H
#define LISTENINGTHING_H

Container* selectedContainer; //the currently selected button
//the multiplier for scaling LCD to touchscreen
uint16_t horizontalMultiplier = (TS_MINX - TS_MAXX) / HX8357_TFTWIDTH;
uint16_t verticalMultiplier = (TS_MAXY - TS_MINY) / HX8357_TFTHEIGHT;

//sizes of buttons 
uint16_t touchButtonHeight = (HX8357_TFTHEIGHT /4-border)*verticalMultiplier;
uint16_t touchButtonWidth = (HX8357_TFTWIDTH - border)*horizontalMultiplier;

// This listens to presses on the tile screen
void listenToTileList(uint16_t x, uint16_t y, Container** containers, bool inManage);
// This listens for touch press on the global settings screen.
void listenToGlobalSettings(uint16_t x, uint16_t y);
// This listens for touch press on the action select screen
void listenToActionList(uint16_t x, uint16_t y);
// This listens for touch press on the manufacturer select screen
void listenToManufacturerList(uint16_t x, uint16_t y, bool global);
// This listens for touch press on the function select screen
void listenToFunctionList(uint16_t x, uint16_t y);
// This listens for touch press on the colour select screen
void listenToColourList(uint16_t x, uint16_t y);
// This listens for touch presses on the remote scan screen
void listenRemoteScan(uint16_t x, uint16_t y);
#endif
