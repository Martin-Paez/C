#include "Ubidots.h"
#include "UbiNeyenMartin.h"

void setup() {
  Serial.begin(115200);   
  ubiSetup(callback); 
  ubiSubscribe(&boton);
  pinMode(34,INPUT);
  pinMode(12,OUTPUT);
  analogReadResolution(12);
  digitalWrite(12,HIGH);
 
}

void loop() {
  ubiConnect();
  ubiRefresh(&boton);
  ubiRefresh( LDR(analogRead(34) * (100.0 / 4096.0) ) );
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
      BOTON(FALSE);
      FREQUENCY(&boton,0) = BOTON_FREQ;
      break;
  }
}
