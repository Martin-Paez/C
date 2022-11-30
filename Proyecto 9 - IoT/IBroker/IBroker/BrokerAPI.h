#ifndef _IUBIDOTS_H_
#define _IUBIDOTS_H_
// El Esp32 CAM necesita incluir Wifi.h

/* ---------------------------------  WIFI -------------------------------- */
  const char *WIFI_SSID = "TeleCentro-7c8d";      
  const char *WIFI_PASS = "Q2MHVWHR2MRJ";

/* -------------------------------- UBIDOTS ------------------------------- */
  #define ARDUINO_UNO  
  #define UBI_FAKE    // Para simular con arduino
  //#define ESP32
  const char *UBIDOTS_TOKEN = "BBFF-FmazbkPOSL3W0HgnKOAIF6nV5M9JGF";
  //#define UBIDOTS
  //#define MOSQUITTO
  //const char *MOSQUITTO_TOKEN = "";

/* -------------------------  MEMORIA RESERVADA --------------------------- */

  // Cantidad maxima de dispositivos suscriptos en simultaneo
  #define SUBSCRIPTIONS_BUFF   1

/* ------------------------------  INTERFAZ ------------------------------- */

  #include "../Utils/Device.h"

  void ubiSetup(void callback(char *, unsigned char *, unsigned int ));
  
  void ubiConnect();
  void ubiLoop();
  
  void ubiSubscribe(Device *d);
  void ubiSetCallback(void callback(char *, unsigned char *, unsigned int ));
  
  void ubiCheck(Device *d);
  
  int ubiCommit(Device *d, int var);
  void ubiPush(Device *d);

#endif
