// container.cpp
// core library

#include "container.h"
#include "IRCodes.h"

//getters and setters for the Title field.
//This field is essentially the button function
char* Container::getTitle() {
  return title;
}
void Container::setTitle(char* value) {
  title = value;
  if (title != "Custom" && title != "Media") {
    functionOverridden = false; // Clear override if a default title has been selected
  }
}

//getters and setters for the brand
//this is the make that defines the sending protocol
//for the IR
char* Container::getBrand() {
  return brand;
}
void Container::setBrand(char* value) {
  brand = value;
}

//getters and setters for the colour
uint16_t Container::getColour() {
  return colour;
}
void Container::setColour(uint16_t value) {
  colour = value;
}

/* getter for the button IR signal
 * It uses IRCodes.cpp, along with the 
 * brand to get the correct function code
 * for the function currently occupying this container
 */
unsigned long Container::getData() {
  if (functionOverridden) {
    return functionData;
  }
  
  if (title=="Power"){
    return getPowerCode(brand);
  }
  else if (title=="Vol up"){
    return getVolUpCode(brand);
  }
  else if (title=="Vol down"){
    return getVolDownCode(brand);
  }
  else if (title=="Chan up"){
    return getChanUpCode(brand);
  }
  else if (title=="Chan down"){
    return getChanDownCode(brand);
  }
  else if (title=="Guide"){
    return getGuideCode(brand);
  }
  else if (title=="Media"){
    return getMediaCode(brand);
  }
  else if (title=="Source"){
    return getSourceCode(brand);
  }
}

void Container::setData(unsigned long value) {
  functionData = value;
  functionOverridden = true;
}
