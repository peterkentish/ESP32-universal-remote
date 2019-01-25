/* 
 IRCodes.cpp
 functionality for getting address codes

Each function works in a similar way. It takes the
make/brand of the remote as a parameter and runs through
a list of the brands it has the codes for. It returns 
the codes that match the current brand.

These are called from the object Container.cpp
which represent buttons on the remote.

Each function represents a type of button which
can be pressed, e.g power or volume up.

The codes were obtained from remotes listed on
http://lirc.sourceforge.net/remotes
*/

#include "IRCodes.h"
unsigned long getPowerCode(char* type) {
  if (type=="Sony") return 0xa90;
  if (type=="Panasonic") return 0x0100BCBD;
  if (type=="LG") return 0x20DF10EF;
  if (type=="JVC") return 0xC5E8;
  if (type=="Sharp") return 0x41A2;
  if (type=="Samsung") return 0xE0E040BF;
  if (type=="Technika") return 0xFE50AF;
  return 0xF0F;
}
unsigned long getChanDownCode(char* type) {
  if (type=="Sony") return 0x890;
  if (type=="Panasonic") return 0x0100ACAD;
  if (type=="LG") return 0x20DF807F;
  if (type=="JVC") return 0xCD06;
  if (type=="Sharp") return 0x4122;
  if (type=="Samsung") return 0xE0E008F7;
  if (type=="Technika") return 0xFE3AC5;
  return 0xF0F;
}
unsigned long getChanUpCode(char* type) {
  if (type=="Sony") return 0x090;
  if (type=="Panasonic") return 0x01002C2D;
  if (type=="LG") return 0x20DF00FF;
  if (type=="JVC") return 0xCDF6;
  if (type=="Sharp") return 0x4222;
  if (type=="Samsung") return 0xE0E048B7;
  if (type=="Technika") return 0xFEF807;
  return 0xF0F;
}
unsigned long getVolDownCode(char* type) {
  if (type=="Sony") return 0xC90;
  if (type=="Panasonic") return 0x01008485;
  if (type=="LG") return 0x20DFC03F;
  if (type=="JVC") return 0xC5F8;
  if (type=="Sharp") return 0x42A2;
  if (type=="Samsung") return 0xE0E0D02F;
  if (type=="Technika") return 0xFEFA05;
  return 0xF0F;
}
unsigned long getVolUpCode(char* type) {
  if (type=="Sony") return 0x490;
  if (type=="Panasonic") return 0x01000405;
  if (type=="LG") return 0x20DF40BF;
  if (type=="JVC") return 0xC578;
  if (type=="Sharp") return 0x40A2;
  if (type=="Samsung") return 0xE0E0E01F;
  if (type=="Technika") return 0xFE7887;
  return 0xF0F;
}
unsigned long getSourceCode(char* type) {
  if (type=="Sony") return 0xA50;
  if (type=="Panasonic") return 0x0100A0A1;
  if (type=="LG") return 0x20DF40BF;
  if (type=="JVC") return 0xC1C0;
  if (type=="Sharp") return 0x4322;
  if (type=="Samsung") return 0xE0E0807F;
  if (type=="Technika") return 0xFE50AF;
  return 0xF0F;
}
unsigned long getGuideCode(char* type) {
  if (type=="Sony") return 0x070;
  if (type=="Panasonic") return 0x01000405;
  if (type=="LG") return 0x20DF40BF;
  if (type=="JVC") return 0xC53C;
  if (type=="Sharp") return 0x458E;
  if (type=="Samsung") return 0xE0E0F20D;
  if (type=="Technika") return 0xFE50AF;
  return 0xF0F;
}

unsigned long getMediaCode(char* type) {
  if (type=="Sony") return 0x5D0;
  if (type=="Panasonic") return 0x01006061;
  if (type=="LG") return 0x20DF40BF;
  if (type=="JVC") return 0xC5D0;
  if (type=="Sharp") return 0x40A2;
  if (type=="Samsung") return 0xE0E0E21D;
  if (type=="Technika") return 0xFE50AF;
  return 0xF0F;
}
unsigned long getCustomCode(char* type) {

}
