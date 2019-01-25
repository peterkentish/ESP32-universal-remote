// DoingThing.h
// core definitions and includes

#ifndef DOINGTHING_H
#define DOINGTHING_H

// This method completes an action based on which tile has been selected and whether
// it has been selected for management
void doTileAction(Container*, bool);
// This method completes an global settings action needs to be carried out
void doGlobalSettingsAction(uint8_t);
// This method occurs when the user has selected an action to be carried out on
// some container
void doActionListAction(uint8_t);
// This method occurs when some manufacturer has been selected and needs to be applied
// to some container(s)
void doManufacturerListAction(uint8_t, bool);
// This method occurs when some colour has been selected by the user and needs to be
// applied to some container
void doColourListAction(uint8_t);
// This method occurs when some function/title has been selected by the user and needs
// to be applied to some container
void doFunctionListAction(uint8_t);
// This method occurs when a remote IR signal has been detected and confirmed by the user
void doRemoteFunctionUpdate();
#endif
