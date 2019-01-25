// IRCodes.h
// header file for getting the appropriate IR codes
// for the function and manufacturer being called

#ifndef IRCODES_H
#define IRCODES_H


unsigned long getPowerCode(char*);
unsigned long getChanDownCode(char*);
unsigned long getChanUpCode(char*);
unsigned long getVolUpCode(char*);
unsigned long getVolDownCode(char*);
unsigned long getSourceCode(char*);
unsigned long getGuideCode(char*);
unsigned long getMediaCode(char*);
unsigned long getCustomCode(char*);
    
    


#endif
