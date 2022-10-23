#include "IUbidots.h"
#include "UbiNeyenMartin.h"

#define VCC_ON   pinMode(VCC,OUTPUT);\
                 digitalWrite(VCC, HIGH);

void setup() {
  Serial.begin(115200);
  #ifdef COMPILE_ESP32
    analogReadResolution(RESOLUTION);
  #endif
  pinMode(LDR_PIN,INPUT);
  VCC_ON;
  ubiSetup(callback); 
  ubiSubscribe(&boton);
}

void loop() {
  ubiConnect();
  ubiCheck(&boton);
  ubiCheck(&ldr);
  ubiLoop();
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.println("");
  Serial.println("callback()");
  Serial.print("millis(): ");
  Serial.println(millis());
  Serial.print("Mensaje de Ubidots [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
    Serial.print((char)payload[i]);
  Serial.println();

  switch((char)payload[0])
  {
    case '0': // Actualizo estado de Boton
      Serial.println("Desabilitando publicacion del boton");
      DISABLE_PUBLISH(BOTON(FALSE), PRESIONADO);
      break;
    case '1': // Preparo publicacion off
      Serial.println("Habilitando publicacion del boton");
      ENABLE_PUBLISH(BOTON(FALSE), PRESIONADO);
      break;
  }
}
