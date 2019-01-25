// This method draws the main remote tiles on the screen
void drawMainRemote(Container** containers) {
  uint16_t startingY = border; // Initialise Y with a slight border
  uint16_t buttonHeight = HX8357_TFTHEIGHT / rows - border; // Calculate button height based on rows
  uint16_t buttonWidth = HX8357_TFTWIDTH / columns - border; // Calculate button width based on columns
  
  // Iterate through the rows and columns
  for (int i = 0; i < rows; i++) {
    uint16_t startingX = border; // Initialise X with a slight border
    for (int j = 0; j < columns; j++) {
      tft.setCursor(startingX + 4, startingY + buttonHeight - 20); // Set cursor to bottom side of container

      uint16_t colour = containers[i*columns+j]->getColour(); // Get colour from container
      tft.fillRect(startingX, startingY, buttonWidth, buttonHeight, colour); // Draw a rectangle
      // Update text colour based on background colour
      if (colour == HX8357_WHITE || colour == HX8357_YELLOW || colour == HX8357_GREEN) {
        tft.setTextColor(HX8357_BLACK);
      } else {
        tft.setTextColor(HX8357_WHITE);
      }
      
      tft.print(containers[i*columns+j]->getTitle()); // Print the title on the tile
      startingX += buttonWidth + border; // Shift the x coordinate ready for the adjacent tile
    }
    startingY += buttonHeight + border; // Shift the y coordinate ready for the adjacent tile
  }
}

// This method draws the setting tiles on the screen
void drawSettings(Container** containers) {
  uint16_t startingY = border; // Initialise Y with a slight border
  uint16_t buttonHeight = HX8357_TFTHEIGHT / rows - border; // Calculate button height based on rows
  uint16_t buttonWidth = HX8357_TFTWIDTH / columns - border; // Calculate button width based on columns
  // Iterate through the rows and columns
  for (int i = 0; i < rows; i++) {
    uint16_t startingX = border; // Initialise X with a slight border
    for (int j = 0; j < columns; j++) {
      tft.setCursor(startingX + 4, startingY + buttonHeight - 20); // Set cursor to bottom side of container
      tft.setTextColor(HX8357_BLACK); // Set text to be black

      char* title = containers[i*columns+j]->getTitle();
      char* settings = "Settings";
      if (title != settings) { // Check if title is NOT equal to settings and if so allow user to manage the container
        tft.fillRect(startingX, startingY, buttonWidth, buttonHeight, HX8357_YELLOW);
        tft.print("Manage");
      } else { // Otherwise allow the user to navigate to the next page of settings
        tft.setTextColor(HX8357_WHITE);
        tft.print("Next");
      }
      startingX += buttonWidth + border; // Shift the x coordinate ready for the adjacent tile
    }
    startingY += buttonHeight + border; // Shift the y coordinate ready for the adjacent tile
  }
}

// This method draws the additional global settings on the screen
void drawGlobalSettings() {
  uint16_t startingX = border; // Initialise X with a slight border
  uint16_t startingY = border; // Initialise Y with a slight border

  // Insert/Draw the battery status in the top section of the screen
  tft.setTextColor(HX8357_WHITE);
  tft.setCursor(startingX+border, startingY + buttonHeight/3);
  tft.print("Battery Status:");
  tft.setCursor(startingX+border, startingY + buttonHeight/1.8);
  tft.print(getBatteryPercentage());
  tft.setCursor(startingX+border+30, startingY + buttonHeight/1.8);
  tft.print("%");

  startingY += buttonHeight + border; // Shift downwards by the list button height ready for next button

  // Insert/Draw the change manufacturer for all tiles button
  tft.setCursor(startingX + border, startingY + buttonHeight/3);
  tft.setTextColor(HX8357_BLACK);
  tft.fillRect(startingX, startingY, buttonWidth-border, buttonHeight, HX8357_WHITE);
  tft.print("Change manufacturer \n for all");

  startingY += buttonHeight + border; // Shift downwards by the list button height ready for next button

  // Insert/Draw the check for update button
  tft.setCursor(startingX + border, startingY + buttonHeight/2 - border);
  tft.setTextColor(HX8357_WHITE);
  tft.fillRect(startingX, startingY, buttonWidth-border, buttonHeight, HX8357_RED);
  tft.print("Check for OTA update");
}

// This method gets the battery percentage from pin 13 which we read was generally the pin used
// for measure battery voltage. Works out as a percentage from 3000 (0%) to 4200 (100%).
int getBatteryPercentage() {
  float val = analogRead(25); // get a reading from the adc pin connected to battery
  float avolt = (val/4095)*2.2 * 2; // adc is set to 12 bits and range is 0-2.2v and multiply by 2 for potential divider
  if (avolt > 4.2) {
    return 100;
  } else {
    return (avolt-minBatt)*100/(maxBatt-minBatt);
  }
}

// This method draws the options given in a list
void drawOptionsList(const char** options, uint8_t optionLength) {
  uint16_t startingX = border; // Initialise X with a slight border
  uint16_t startingY = border; // Initialise Y with a slight border

  uint8_t showOptionRows = 4; // The number of rows to be shown as part of pagination
  
  bool showPreviousButton = page > 0; // Show previous button if beyond page 0
  bool showNextButton = optionLength > lastPage + 2; // Show next if the available length is beyond the index being rendered
  tft.setTextColor(HX8357_WHITE); // Set text colour to white for previous button
  if (showPreviousButton) { // If previous is to be shown
    tft.setCursor(startingX+buttonWidth/2, startingY + buttonHeight/2); // Render kinda in middle
    tft.print("^");
    startingY += buttonHeight + border; // Shift downwards by the list button height ready for next button
    showOptionRows--; // Decrease number of options to be shown
  }
  if (showNextButton) {
    showOptionRows--; // Decrease number of options to be shown
  }

  // Iterate through rows to be shown
  for (int i = 0; i < showOptionRows; i++) {
    if (optionLength > lastPage + i) {
      tft.setCursor(startingX + border, startingY + buttonHeight/2); // Move cursor to middle left
      tft.setTextColor(HX8357_BLACK);
      tft.fillRect(startingX, startingY, buttonWidth, buttonHeight, HX8357_WHITE);
      tft.print(options[lastPage+i]); // Display options at paginated index
    }
    startingY += buttonHeight + border; // Shift downwards by the list button height ready for next button
  }

  if (showNextButton) { // If next button is shown
    // Render as a white v on the screen
    tft.setTextColor(HX8357_WHITE);
    tft.setCursor(startingX+buttonWidth/2, startingY + buttonHeight/2);
    tft.print("v");
  }
}

void drawColoursList(const uint16_t* options, uint8_t optionLength) {
  uint16_t startingX = border; // Initialise X with a slight border
  uint16_t startingY = border; // Initialise Y with a slight border

  uint8_t showOptionRows = 4; // The number of rows to be shown as part of pagination
  
  bool showPreviousButton = page > 0; // Show previous button if beyond page 0
  bool showNextButton = optionLength > lastPage + 2; // Show next if the available length is beyond the index being rendered
  tft.setTextColor(HX8357_WHITE); // Set text colour to white for previous button
  if (showPreviousButton) { // If previous is to be shown
    tft.setCursor(startingX+buttonWidth/2, startingY + buttonHeight/2); // Render kinda in middle
    tft.print("^");
    showOptionRows--; // Decrease number of options to be shown
    startingY += buttonHeight + border; // Shift downwards by the list button height ready for next button
  }
  if (showNextButton) { // If next is to be shown
    showOptionRows--; // Decrease number of options to be shown
  }

  // Iterate through rows to be shown
  for (int i = 0; i < showOptionRows; i++) {
    // Render each of the options as an on screen options
    if (optionLength > lastPage + i) {
      tft.setCursor(startingX + border, startingY + buttonHeight/2); // Move the cursor to the middle left

      // Render colour option as displayed on tile page
      switch (options[lastPage+i]) {
        case HX8357_WHITE:
          tft.setTextColor(HX8357_BLACK);
          tft.fillRect(startingX, startingY, buttonWidth, buttonHeight, HX8357_WHITE);
          tft.print("White");
          break;
        case HX8357_RED:
          tft.setTextColor(HX8357_WHITE);
          tft.fillRect(startingX, startingY, buttonWidth, buttonHeight, HX8357_RED);
          tft.print("Red");
          break;
        case HX8357_BLUE:
          tft.setTextColor(HX8357_WHITE);
          tft.fillRect(startingX, startingY, buttonWidth, buttonHeight, HX8357_BLUE);
          tft.print("Blue");
          break;
        case HX8357_GREEN:
          tft.setTextColor(HX8357_BLACK);
          tft.fillRect(startingX, startingY, buttonWidth, buttonHeight, HX8357_GREEN);
          tft.print("Green");
          break;
      }
      startingY += buttonHeight + border; // Shift downwards by the list button height ready for next button
    }
  }
  
  if (showNextButton) { // If next button is shown
    // Render as a white v on the screen
    tft.setTextColor(HX8357_WHITE);
    tft.setCursor(startingX+buttonWidth/2, startingY + buttonHeight/2);
    tft.print("v");
  }
}

// This method draws the IR remote scanning screen
void drawRemoteScan() {
  uint16_t startingX = border; // Initialise X with a slight border
  uint16_t startingY = border; // Initialise Y with a slight border

  // Draw instructions at top of screen
  tft.setTextColor(HX8357_WHITE);
  tft.setCursor(startingX+border, startingY + buttonHeight/2);
  tft.print("Point remote at receiver");

  // Draw found to indicate what the IR receiver as detected
  startingY += buttonHeight + border; // Shift downwards by the list button height ready for next button
  tft.setCursor(startingX+border, startingY + buttonHeight/2);
  tft.print("Found:");

  // Draw the value as learnt by the receiver as HEX
  startingY += buttonHeight + border; // Shift downwards by the list button height ready for next button
  tft.setCursor(startingX+buttonWidth/4, startingY + buttonHeight/2);
  tft.print(learntCode, HEX);

  // Draw the confirm button at the bottom of the screen
  startingY += buttonHeight + border; // Shift downwards by the list button height ready for next button
  tft.drawRect(startingX, startingY, buttonWidth, buttonHeight, HX8357_WHITE);
  tft.setCursor(startingX + buttonWidth/2 - border, startingY + buttonHeight/2 - border);
  tft.print("Confirm");
}
