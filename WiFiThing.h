/////////////////////////////////////////////////////////////////////////////
// MyWIFIThing.h
// COM3505 lab assessment: WiFi selection header
/////////////////////////////////////////////////////////////////////////////

#ifndef WIFI_THING_H 
#define WIFI_THING_H

// Constants ////////////////////////////////////////////////////////////////
//const char *templatePage[] = {    // we'll use Ex07 templating to build pages
//  "<html><head><title>",                                                //  0
//  "default title",                                                      //  1
//  "</title>\n",                                                         //  2
//  "<meta charset='utf-8'>",                                             //  3
//  "<meta name='viewport' content='width=device-width, initial-scale=1.0'>\n"
//  "<style>body{background:#FFF; color: #000; font-family: sans-serif;", //  4
//  "font-size: 150%;}</style>\n",                                        //  5
//  "</head><body>\n",                                                    //  6
//  "<h2>Welcome to Thing!</h2>\n",                                       //  7
//  "<!-- page payload goes here... -->\n",                               //  8
//  "<!-- ...and/or here... -->\n",                                       //  9
//  "\n<p><a href='/'>Home</a>&nbsp;&nbsp;&nbsp;</p>\n",                  // 10
//  "</body></html>\n\n",                                                 // 11
//};
#include <ESPWebServer.h> // simple webserver
void initWebServer();
void startAP();

String apSSID;                  // SSID of the AP
ESPWebServer webServer(80); // a simple web server

typedef struct { int position; const char *replacement; } replacement_t;

void getHtml(String& html, const char *[], int, replacement_t [], int);

// getting the length of an array in C can be complex...
// https://stackoverflow.com/questions/37538/how-do-i-determine-the-size-of-my-array-in-c

#define ALEN(a) ((int) (sizeof(a) / sizeof(a[0]))) // only in definition scope!
#define GET_HTML(strout, boiler, repls) \
  getHtml(strout, boiler, ALEN(boiler), repls, ALEN(repls));
#endif
