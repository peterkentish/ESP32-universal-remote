// container.h
// core definitions and includes

#ifndef CONTAINER_H
#define CONTAINER_H

#include "unphone.h"

class Container {
    char* title;
    char* brand;
    unsigned long functionData;
    bool functionOverridden;
    uint16_t colour;
    
  public:
    void init(char* titleC, char* brandC, uint16_t colourC) {
      title = titleC;
      brand = brandC;
      colour = colourC;
    }
  
    char* getTitle();
    void setTitle(char*);

    char* getBrand();
    void setBrand(char*);

    uint16_t getColour();
    void setColour(uint16_t);
    
    unsigned long getData();
    void setData(unsigned long);
};

#endif
