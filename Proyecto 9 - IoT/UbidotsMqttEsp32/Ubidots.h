#ifndef _UBIDOTS_H_
#define _UBIDOTS_H_

  const char *UBIDOTS_TOKEN = "BBFF-FmazbkPOSL3W0HgnKOAIF6nV5M9JGF";  
  const char *WIFI_SSID = "TeleCentro-7c8d";      
  const char *WIFI_PASS = "Q2MHVWHR2MRJ";  

  #include "Device.h"
  
    #define SUBJECTS_BUFF   1

  void ubiSetup(void callback(char *, unsigned char *, unsigned int ));
  
  void ubiConnect();
  void ubiLoop();
  
  void ubiSubscribe(Device *d);
  void ubiSetCallback(void callback(char *, unsigned char *, unsigned int ));
  
  void ubiRefresh(Device *d);
  
  int ubiAdd(Device *d, int var);
  void ubiCommit(Device *d);

#endif
