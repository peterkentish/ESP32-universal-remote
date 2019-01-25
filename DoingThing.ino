// This method completes an action based on which tile has been selected and whether
// it has been selected for management
void doTileAction(Container* container, bool inManage) {
  // a char* with the string set. This string is used multiple times so felt best to store in a variable
  char* settings = "Settings"; 
  // Get the title of the selected container
  char* title = container->getTitle();
  // If the user is set to manage and the box has a title of settings, selecting
  // this box will move to the global settings menu
  if (inManage && title == settings) {
    inMenuSection = GlobalSettings;
  } else if (inManage) { // If in management an any other box has been selected, go to the management screen
    inMenuSection = Management;
    selectedContainer = container;
  } else if (title == settings) { // If not in manage, and settings is pressed go to settings
    inMenuSection = Settings;
  } else { // Otherwise do the command on the container
    doCommand(container);
  }
  // Redraw
  draw();
}

// This method completes an global settings action needs to be carried out
void doGlobalSettingsAction(uint8_t function) {
  // Switch based on the function integer given
  switch (function) {
    // If 0 the user has requested to manage the manufacturer of all tiles
    case 0:
      inMenuSection = GlobalManufacturers;
      break;
    // If 1 the user has requested to check for an ota update
    case 1:
      int highestAvailableVersion = -1;
      // Didn't quite have chance to fully implement, simply gets whether the latest version at the moment
      if (checkOTAStatus(&highestAvailableVersion)) {
        doOTAUpdate(highestAvailableVersion);
      }
      break;
  }
  // Redraw
  draw();
}

// This method occurs when the user has selected an action to be carried out on
// some container
void doActionListAction(uint8_t actionNumber) {
  // Switch based on the integer given by the listening function
  switch (actionNumber) {
    // 0 or 1 correspond to page management
    case 0:
    case 1:
      managePages(actionNumber);
      break;
    // Otherwise they correspond to the page to be redirected to
    default:
      switch (lastPage-2+actionNumber) {
        case 0:
          inMenuSection = Manufacturers;
          break;
        case 1:
          inMenuSection = Colours;
          break;
        case 2:
          inMenuSection = Functions;
          break;
        case 3:
          inMenuSection = Scanning;
          break;
      }
  }
  // If the page is navigating away from management, clear the page storage
  if (inMenuSection != Management) {
    page = 0;
    lastPage = 0;
  }
  // Redraw
  draw();
}

// This method occurs when some manufacturer has been selected and needs to be applied
// to some container(s)
void doManufacturerListAction(uint8_t actionNumber, bool global) {
  // Switch based on the integer given by the listening function
  switch (actionNumber) {
    // 0 or 1 correspond to page management
    case 0:
    case 1:
      managePages(actionNumber);
      break;
    // Otherwise they correspond to a manufacturer in the list
    default:
      char* brand = const_cast<char*>(manufacturers[lastPage-2+actionNumber]);
      // If a global update, then update all otherwise update just the selected container
      if (global) {
        updateContainers(brand);
      } else {
        selectedContainer->setBrand(brand);
      }
      
      inMenuSection = Home; // return to the home page
      break;
  }
  // If the page is navigating away from manufacturers or global manufacturers, clear the page storage
  if (inMenuSection != Manufacturers && inMenuSection != GlobalManufacturers) {
    page = 0;
    lastPage = 0;
  }
  // Redraw
  draw();
}

// This method updates each of the tiles with the newly selected brand.
void updateContainers(char* newBrand) {
  box1->setBrand(newBrand);
  box2->setBrand(newBrand);
  box3->setBrand(newBrand);
  box4->setBrand(newBrand);
  box5->setBrand(newBrand);
  box6->setBrand(newBrand);
}

// This method occurs when some colour has been selected by the user and needs to be
// applied to some container
void doColourListAction(uint8_t actionNumber) {
  // Switch based on the integer given by the listening function
  switch (actionNumber) {
    // 0 or 1 correspond to page management
    case 0:
    case 1:
      managePages(actionNumber);
      break;
    // Otherwise they correspond to a colour in the list
    default:
      selectedContainer->setColour(colours[lastPage-2+actionNumber]);
      
      inMenuSection = Home; // return to the home page
      break;
  }
  // If the page is navigating away from colours, clear the page storage
  if (inMenuSection != Colours) {
    page = 0;
    lastPage = 0;
  }
  // Redraw
  draw();
}

// This method occurs when some function/title has been selected by the user and needs
// to be applied to some container
void doFunctionListAction(uint8_t actionNumber) {
  // Switch based on the integer given by the listening function
  switch (actionNumber) {
    // 0 or 1 correspond to page management
    case 0:
    case 1:
      managePages(actionNumber);
      break;
    // Otherwise they correspond to a function in the list
    default:
      selectedContainer->setTitle(const_cast<char*>(functions[lastPage-2+actionNumber]));
      
      inMenuSection = Home; // return to the home page
      break;
  }
  // If the page is navigating away from functions, clear the page storage
  if (inMenuSection != Functions) {
    page = 0;
    lastPage = 0;
  }
  // Redraw
  draw();
}

// This method occurs when a remote IR signal has been detected and confirmed by the user
void doRemoteFunctionUpdate() {
    // Update the container with the global variables
    selectedContainer->setData(learntCode);
    selectedContainer->setBrand(const_cast<char*>(convertBrand(remoteMake)));
    
    inMenuSection = Home; // return to the home page
    
    //Redraw
    draw();
}

// This method converts a decode type to a string in the manufacturers list
const char* convertBrand(decode_type_t type) {
  switch (type) {
    case SONY:
      return manufacturers[0];
    case PANASONIC:
      return manufacturers[1];
    case LG:
      return manufacturers[2];
    case JVC:
      return manufacturers[3];
    case SHARP:
      return manufacturers[4];
    case SAMSUNG:
      return manufacturers[5];
    case NEC:
      return manufacturers[6];
    default:
      return customManufacturer;
  }
}

// This method manages the pages across a number of displays on screen. It updates
// the page and lastpage variables
void managePages(uint8_t actionNumber) {
  switch (actionNumber) {
    // previous page
    case 0:
      // If the page is greater than 1, then it only had a maximum of 2 items on it
      if (page > 1) {
        lastPage -= 2; // remove 2 from index
        page--; // decrease page
      }
      // If the page is greater than 0, then it had a maximum of 3 items on it
      else if (page > 0) {
        lastPage -= 3; // remove 3 from index
        page--; // decrease page
      }
      break;
    // next page
    case 1:
      // If first page, increase index by 3
      if (page == 0) {
        lastPage += 3;
      // Beyond first page, increase index by just 2
      } else {
        lastPage += 2;
      }
      page++; // increase page
      break;
  }
}
