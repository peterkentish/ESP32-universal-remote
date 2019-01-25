// SendingThing.h
// function prototypes and includes

#ifndef SENDINGTHING_H
#define SENDINGTHING_H

IRsend irsend;
void doCommand(Container);
void doSonyCommand(unsigned long);
void doSamsungCommand(unsigned long);
void doPanasonicCommand(unsigned long);
void doJVCCommand(unsigned long);
void doNECCommand(unsigned long);
void doSharpCommand(unsigned long);
#endif
