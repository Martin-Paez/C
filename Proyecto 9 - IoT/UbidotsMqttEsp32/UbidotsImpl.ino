//#include "Ubidots.h"
#include "Macros.h"

//#include "WiFi.h"
#include "UbidotsEsp32Mqtt.h"
Ubidots ubidots(UBIDOTS_TOKEN);

//#include "FakeUbiEsp.cpp"
//FakeUbiEsp ubidots;

Device* subjects[SUBJECTS_BUFF];
int subjIndex=0;

void ubiSetup(void callback(char *, unsigned char *, unsigned int ))  // TODO, recibir callback
{
  Serial.println("Conectandose al WiFi...");
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  Serial.println("Conectado a la red WiFi exitosamente.");
  Serial.println("Configurando Broker...");
  if (callback != NULL)
    ubiSetCallback(callback);
  ubidots.setup();
  Serial.println("Configuracion exitosa.");
}

void ubiConnect(){
  while (!ubidots.connected()) {
    Serial.println("Reconectando a Ubidots...");
    ubidots.reconnect();
    Serial.println("Conectado exitosamente");
    for(int i=0; i < SUBJECTS_BUFF; i++)
      subscribeAllVars(subjects[i]);
    Serial.println("Suscripcion exitosa.");
  }
}

void ubiLoop()
{
  ubidots.loop();
}

void ubiSubscribe(Device *d)
{
  if ( subjIndex >  SUBJECTS_BUFF )
  {
    Serial.println("Limite de suscripciones alcanzado.");
    Serial.println("Puede aumentarlo en Ubidots.h.");
    return;
  }
  subjects[subjIndex++] = d;
  subscribeAllVars(d);
}

void subscribeAllVars(Device *d)
{
  for(int j=0; j < d->vSize; j++) {
    Serial.print("Suscribiendo la variable '");
    Serial.print(NAME(d,j));
    Serial.print("' del dispositivo '");
    Serial.print(DEVICE(d));
    Serial.println("'...");
    ubidots.subscribeLastValue(TOPIC(d,j));
    Serial.println("Suscripcion exitosa");
  }
}

void ubiPublish(Device *d)
{
  int total = 0;
  for(int i=0; i < d->vSize; i++)
    total += ubiAdd(d, i);
  if (total > 0) 
  {
    Serial.print("Publicando ");
    Serial.print(d->vSize);
    Serial.print(" variables del dispositivo ");
    Serial.println(DEVICE(d));
    ubiCommit(d);
  }
}

void ubiCommit(Device *d)
{
  Serial.println("Publicando JSON...");
  ubidots.publish(DEVICE(d));
  Serial.println("Publicado exitosamente.");
}

int ubiAdd(Device *d, int var)
{
  if ( NOT_PUBLISH_TIME(d,var) )
    return 0;
  Serial.print("Tiempo transcurrido : ");
  Serial.println(ELAPSED(d,var));
  Serial.print("Escribiendo JSON. Variable ");
  Serial.print(NAME(d,var));
  Serial.print(" = ");
  Serial.print(VAL(d,var));
  Serial.print(" del dispositivo ");
  Serial.print(DEVICE(d));
  Serial.println("...");
  ubidots.add(TOPIC_VAL(d,var));
  Serial.println("Escritura exitosa.");
  TIME(d,var) = millis();
  return 1;
}

void ubiSetCallback(void callback(char *, unsigned char *, unsigned int )) 
{
  ubidots.setCallback(callback);
}
