#ifndef _IUBIDOTS_H_
#define _IUBIDOTS_H_

/* ---------------------------------  WIFI ------------------------------- */

/* ---- El Esp32 CAM lo necesita incluir ---------------------------------

  #include "WiFi.h"               
*/

  const char *WIFI_SSID = "TeleCentro-7c8d";      
  const char *WIFI_PASS = "Q2MHVWHR2MRJ";  

  
/* -------------------------------- UBIDOTS ------------------------------- */

  const char *UBIDOTS_TOKEN = "BBFF-FmazbkPOSL3W0HgnKOAIF6nV5M9JGF";  

  //#define COMPILE_ESP32

  #ifdef  COMPILE_ESP32               // Para programar con el esp32
    #include "UbidotsEsp32Mqtt.h"     // Con la API provista por ubidots
    Ubidots ubidots(UBIDOTS_TOKEN);
  #else                               // Para simular con arduino
    #include "FakeUbiEsp.cpp"      
    Timer loss(15000, millis); 
    FakeDevice fakeBoton(8000,"boton","presionado","1",1,"0",1,millis);
    FakeDevice fakeDevices[1] = {fakeBoton};
    void serialStr(char* txt) { Serial.println(txt); }
    void serialInt(int n) { Serial.println(n); }
    FakeUbiEsp ubidots(loss, fakeDevices, 1, serialStr, serialInt, millis);
  #endif


/* -------------------------  MEMORIA RESERVADA --------------------------- */

  // Cantidad maxima de dispositivos suscriptos en simultaneo
  #define SUBSCRIPTIONS_BUFF   1

/* ------------------------------  INTERFAZ ------------------------------- */

  #include "Device.h"

  void ubiSetup(void callback(char *, unsigned char *, unsigned int ));
  
  void ubiConnect();
  void ubiLoop();
  
  void ubiSubscribe(Device *d);
  void ubiSetCallback(void callback(char *, unsigned char *, unsigned int ));
  
  void ubiCheck(Device *d);
  
  int ubiCommit(Device *d, int var);
  void ubiPush(Device *d);

#endif
