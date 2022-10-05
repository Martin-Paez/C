#include "IUbidots.h"
#include "Macros.h"

Device* subjects[SUBSCRIPTIONS_BUFF];
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
    for(int i=0; i < SUBSCRIPTIONS_BUFF; i++)
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
  if ( subjIndex >=  SUBSCRIPTIONS_BUFF )
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

void ubiCheck(Device *d)
{
  int total = 0;
  for(int i=0; i < d->vSize; i++)
    total += ubiCommit(d, i);
  if (total > 0) 
  {
    Serial.print("Publicando ");
    Serial.print(d->vSize);
    Serial.print(" variables del dispositivo ");
    Serial.println(DEVICE(d));
    ubiPush(d);
  }
}

void ubiPush(Device *d)
{
  Serial.println("Publicando JSON...");
  ubidots.publish(DEVICE(d));
  Serial.println("Publicado exitosamente.");
}

int ubiCommit(Device *d, int var)
{
  if ( NOT_PUBLISH_TIME(d,var) )
    return 0;
  Serial.print("Tiempo transcurrido : ");
  Serial.println(ELAPSED(d,var));
  Serial.print("Escribiendo JSON. Variable ");
  Serial.print(NAME(d,var));
  Serial.print(" = ");
  Serial.print(GET(d,var));
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
