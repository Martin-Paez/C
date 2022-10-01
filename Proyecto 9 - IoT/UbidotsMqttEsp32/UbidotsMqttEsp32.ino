#include "Ubidots.h"
#include "UbiNeyenMartin.h"

void setup() {
  Serial.begin(115200);   
  ubiSetup(callback); 
  ubiSubscribe(&boton);
}

void loop() {
  ubiConnect();
  ubiPublish(&boton);
  ubiPublish( LDR(/*analogRead(13) * (100.0 / 4096.0)*/ 50) );
  ubiLoop();
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Mensaje de Ubidots [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
    Serial.print((char)payload[i]);
  Serial.println();

  switch((char)payload[0])
  {
    case '0':
      FREQUENCY(&boton,0) = STOP;
      break;
    case '1':
      BOTON(FALSE)
      FREQUENCY(&boton,0) = 5000;
      break;
  }
}
