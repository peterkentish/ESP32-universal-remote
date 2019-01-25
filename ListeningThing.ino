// This listens to presses on the tile screen
void listenToTileList(uint16_t x, uint16_t y, Container** containers, bool inManage) {
  uint16_t startingY = TS_MAXY - border * verticalMultiplier; // Initialise Y with a slight border
  uint16_t buttonHeight = (HX8357_TFTHEIGHT / rows - border) * verticalMultiplier; // Calculate button height based on rows
  uint16_t buttonWidth = (HX8357_TFTWIDTH / columns - border) * horizontalMultiplier; // Calculate button height based on columns

  // Iterate through the rows and columns
  for (int i = 0; i < rows; i++) {
    uint16_t startingX = TS_MINX - border * horizontalMultiplier; // Initialise X with a slight border
    for (int j = 0; j < columns; j++) {
      // If the touch coordinate falls in the container, do an action with that container
      if (x < startingX && x > startingX - buttonWidth - border &&
          y < startingY && y > startingY - buttonHeight - border) {
        doTileAction(containers[i*columns+j], inManage);
        return;
      }
      startingX -= buttonWidth - border * horizontalMultiplier; // Shift the x coordinate ready for the adjacent tile
    }
    startingY -= buttonHeight - border; // Shift the y coordinate ready for the adjacent tile
  }
}

// This listens for touch press on the global settings screen.
void listenToGlobalSettings(uint16_t x, uint16_t y) {
  uint16_t startingX = TS_MINX - border * horizontalMultiplier; // Initialise X with a slight border
  uint16_t startingY = TS_MAXY - border * verticalMultiplier - touchButtonHeight - border; // Initialise Y with a slight border 1 button down

  // If it falls in top button, then do an action with code 0
  if (y < startingY && y > startingY - touchButtonHeight - border) {
    doGlobalSettingsAction(0);
    return;
  }
  startingY -= touchButtonHeight - border; // Shift the y coordinate ready for the adjacent row

  // If it falls in lower button, then do an action with code 1
  if (y < startingY && y > startingY - touchButtonHeight - border) {
    doGlobalSettingsAction(1);
    return;
  } 
}

// This listens for touch press on the colour select screen
void listenToActionList(uint16_t x, uint16_t y) {
  uint16_t startingX = TS_MINX - border * horizontalMultiplier; // Initialise X with a slight border
  uint16_t startingY = TS_MAXY - border * verticalMultiplier; // Initialise Y with a slight border
  uint8_t showActionRows = 4;
  
  bool showPreviousButton = page > 0;
  bool showNextButton = actionLength > lastPage + 2;
  if (showNextButton) {
    showActionRows--;
  }

  // If the previous button is shown and the y value falls within it, move to the previous page of colours
  if (showPreviousButton) {
    if (y < startingY && y > startingY - touchButtonHeight - border) {
      doActionListAction(0);
      return;
    }
    showActionRows--; 
    startingY -= touchButtonHeight - border; // Shift the y coordinate ready for the adjacent row
  }
  
  for (int i = 0; i < showActionRows; i++) {
    // If the touch coordinate falls in the container, do an action with that container
    if (x < startingX && x > startingX - touchButtonWidth - border &&
        y < startingY && y > startingY - touchButtonHeight - border) {
      doActionListAction(i+2);
      return;
    }
    
    startingY -= touchButtonHeight - border; // Shift the y coordinate ready for the adjacent row
  }
  // If the next button is shown and the y value falls within it, move to the next page of actions
  if (showNextButton && y < startingY) {
    doActionListAction(1);
    return;
  }
}

// This listens for touch press on the manufacturer select screen
void listenToManufacturerList(uint16_t x, uint16_t y, bool global) {
  uint16_t startingX = TS_MINX - border * horizontalMultiplier; // Initialise X with a slight border
  uint16_t startingY = TS_MAXY - border * verticalMultiplier; // Initialise Y with a slight border
  uint8_t showManufacturerRows = 4;
  
  bool showPreviousButton = page > 0;
  bool showNextButton = manufacturerLength > lastPage + 2;
  if (showNextButton) {
    showManufacturerRows--;
  }

  // If the previous button is shown and the y value falls within it, move to the previous page of manufacturers
  if (showPreviousButton) {
    if (y < startingY && y > startingY - touchButtonHeight - border) {
      doManufacturerListAction(0, false);
      return;
    }
    showManufacturerRows--;
    startingY -= touchButtonHeight - border; // Shift the y coordinate ready for the adjacent row
  }

  // Iterate over each of the rows
  for (int i = 0; i < showManufacturerRows; i++) {
    // If the touch coordinate falls in the container, do an action with that container
    if (x < startingX && x > startingX - touchButtonWidth - border &&
        y < startingY && y > startingY - touchButtonHeight - border) {
      doManufacturerListAction(i+2, global);
      return;
    }
    
    startingY -= touchButtonHeight - border; // Shift the y coordinate ready for the adjacent row
  }
  // If the next button is shown and the y value falls within it, move to the next page of manufacturers
  if (showNextButton && y < startingY) {
    doManufacturerListAction(1, false);
    return;
  }
}

// This listens for touch press on the function select screen
void listenToFunctionList(uint16_t x, uint16_t y) {
  uint16_t startingX = TS_MINX - border * horizontalMultiplier; // Initialise X with a slight border
  uint16_t startingY = TS_MAXY - border * verticalMultiplier; // Initialise Y with a slight border
  uint8_t showFunctionRows = 4;
  
  bool showPreviousButton = page > 0;
  bool showNextButton = functionLength > lastPage + 2;
  if (showNextButton) {
    showFunctionRows--;
  }

  // If the previous button is shown and the y value falls within it, move to the previous page of functions
  if (showPreviousButton) {
    if (y < startingY && y > startingY - touchButtonHeight - border) {
      doFunctionListAction(0);
      return;
    }
    showFunctionRows--; 
    startingY -= touchButtonHeight - border; // Shift the y coordinate ready for the adjacent row
  }

  // Iterate over each of the rows
  for (int i = 0; i < showFunctionRows; i++) {
    // If the touch coordinate falls in the container, do an action with that container
    if (x < startingX && x > startingX - touchButtonWidth - border &&
        y < startingY && y > startingY - touchButtonHeight - border) {
      doFunctionListAction(i+2);
      return;
    }
    
    startingY -= touchButtonHeight - border; // Shift the y coordinate ready for the adjacent row
  }
  // If the next button is shown and the y value falls within it, move to the next page of functions
  if (showNextButton && y < startingY) {
    doFunctionListAction(1);
    return;
  }
}

// This listens for touch press on the colour select screen
void listenToColourList(uint16_t x, uint16_t y) {
  uint16_t startingX = TS_MINX - border * horizontalMultiplier; // Initialise X with a slight border
  uint16_t startingY = TS_MAXY - border * verticalMultiplier; // Initialise Y with a slight border
  uint8_t showColourRows = 4;
  
  bool showPreviousButton = page > 0;
  bool showNextButton = colourLength > lastPage + 2;
  if (showNextButton) {
    showColourRows--;
  }

  // If the previous button is shown and the y value falls within it, move to the previous page of colours
  if (showPreviousButton) {
    if (y < startingY && y > startingY - touchButtonHeight - border) {
      doColourListAction(0);
      return;
    }
    showColourRows--; 
    startingY -= touchButtonHeight - border; // Shift the y coordinate ready for the adjacent row
  }

  // Iterate over each of the rows
  for (int i = 0; i < showColourRows; i++) {
    // If the touch coordinate falls in the container, do an action with that container
    if (x < startingX && x > startingX - touchButtonWidth - border &&
        y < startingY && y > startingY - touchButtonHeight - border) {
      doColourListAction(i+2);
      return;
    }
    
    startingY -= touchButtonHeight - border; // Shift the y coordinate ready for the adjacent row
  }
  // If the next button is shown and the y value falls within it, move to the next page of colours
  if (showNextButton && y < startingY) {
    doColourListAction(1);
    return;
  }
}

// This listens for touch presses on the remote scan screen
void listenRemoteScan(uint16_t x, uint16_t y) {
  uint16_t startingX = TS_MINX - border * horizontalMultiplier; // Initialise X with a slight border
  uint16_t startingY = TS_MAXY - border * verticalMultiplier - 3*(touchButtonHeight - border); // Initialise Y with a slight border 3 buttons down
  
  // If the touch coordinate falls in the container, do the remote function update.
  if (y < startingY) {
    doRemoteFunctionUpdate();
  }
}
