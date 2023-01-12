#line 1 "c:\\Users\\54911\\Documents\\GitHub\\C\\Proyecto 9 - IoT\\Esp\\EspUtils\\EspUtils.h"
#ifndef _ESP_UTILS_H
#define _ESP_UTILS_H

  #define _SSID      "FIBERTEL  2.4"
  #define _WIFI_PASS "0045640770a"
  
  #include "..\RunT\RunT.h"
  #include <WiFi.h>       // 26-6%

  bool initSerial(int timeout = 1000);          // < 0-1%
  bool connectWifi(int timeout = 5000);
  bool connectWifiWith(char* ssid, char* pass, int timeout = 5000);
  String readCmd(bool showHelp=true);
  String readCmd(Stream &Serialx, bool showHelp=true);
  String readCmd(RunT &exe, bool showHelp=true);
  String readCmd(Stream &Serialx, RunT &exe, bool showHelp=true);

  #include "EspUtils.c"  // En Arduino es necesario

#endif
