//SendingThing.cpp
// A set of functions for sending IR signals from the remote.


/*DoCommand(Container)
 *This function takes in a container object, which has a field 
 *for brand and a field for getting the button IR data.
 *It then uses the brand to determine which sending protocol
 *to use, and it sends the button data over IR using this protocol.
 */

void doCommand(Container* container){
  char* brand = container->getBrand();
  unsigned long functionData = container->getData();
   if(brand=="Sony"){
    doSonyCommand(functionData);
   }else if (brand=="Samsung"){
    doSamsungCommand(functionData);
   }else if (brand=="LG" || brand=="Toshiba" || brand=="Technika"){
    doNECCommand(functionData);
   }else if (brand=="Panasonic"){
      doPanasonicCommand(functionData);
   }else if (brand=="JVC"){
      doJVCCommand(functionData);
   }else if (brand=="Sharp"){
    doSharpCommand(functionData);
   }else if (brand=="Custom"){
    doCustomCommand(functionData);
   }
}
/*
 * Below are the functions for each type of device.
 * Some have specific protocols, such as Sony's repeat.
 * JVC has an interesting header protocol involving sending
 * a header and then sending non header data.
 */
void doSonyCommand(unsigned long dataToSend){
  for (int i = 0; i < 3; i++) {
    irsend.sendSony(dataToSend, 12);
    delay(40);
  }
  delay(1000); 
}
void doSamsungCommand(unsigned long dataToSend){
  irsend.sendSAMSUNG(dataToSend, 32);
  delay(100); //5 second delay between each signal burst
}
void doPanasonicCommand(unsigned long dataToSend){
  irsend.sendPanasonic(0x4004,dataToSend);
  delay(100); //5 second delay between each signal burst
}

void doJVCCommand(unsigned long dataToSend){
  irsend.sendJVC(dataToSend, 16,false);
  for (int i = 0; i<2;i++){
    irsend.sendJVC(dataToSend, 16,true);
  }
  delay(100); //5 second delay between each signal burst
}

void doNECCommand(unsigned long dataToSend){
  //LG does not need repeats
  irsend.sendNEC(dataToSend, 32);
  delay(100); //5 second delay between each signal burst
}


void doSharpCommand(unsigned long dataToSend){
  //sendSharp function includes necessary repeats
  irsend.sendSharp(0x4004, dataToSend);
  delay(100); //5 second delay between each signal burst
}

void doCustomCommand(unsigned long dataToSend) {
  // Unknown manufacturer.
}
