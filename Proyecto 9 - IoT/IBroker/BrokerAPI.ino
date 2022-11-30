#include "IBroker/IBroker.h"
#include "Utils/Macros.h"

#ifdef  ESP32               
  #ifdef  UBIDOTS 
    #include "AdapterUbidots.cpp"
    AdapterUbidots broker(UBIDOTS_TOKEN);
    char * brokerName = "Ubidots";
  #endif
#endif
#ifdef UBI_FAKE                    
  #include "IBroker/FakeBroker/FakeBroker.cpp"      
  Timer loss(15000, millis); 
  FakeDevice fakeBoton(8000,"boton","presionado",(unsigned char*)"1",1,(unsigned char*)"0",1,millis);
  FakeDevice fakeDevices[1] = {fakeBoton};
  void serialStr(char* txt) { Serial.println(txt); }
  void serialInt(int n) { Serial.println(n); }
  char * brokerName = "Fake Ubi";
  FakeUbi broker(loss, fakeDevices, 1, serialStr, serialInt, millis);
#endif

Device* subjects[SUBSCRIPTIONS_BUFF];
int subjIndex=0;

void ubiSetup(void callback(char *, unsigned char *, unsigned int ))  // TODO, recibir callback
{
  Serial.println("");
  Serial.println("ubiSetup()");
  Serial.println("Conectandose al WiFi...");
  broker.connectToWifi(WIFI_SSID, WIFI_PASS);
  Serial.println("Conectado a la red WiFi exitosamente.");
  Serial.println("Configurando Broker...");
  if (callback != NULL)
    ubiSetCallback(callback);
  broker.setup();
  Serial.println("Configuracion exitosa.");
}

void ubiConnect(){
  while (!broker.connected()) {
    Serial.println("");
    Serial.println("ubiConnect()");
    Serial.println("Reconectando a ");
    Serial.println(brokerName);
    Serial.println("...");
    broker.reconnect();
    Serial.println("Conectado exitosamente");
    for(int i=0; i < SUBSCRIPTIONS_BUFF; i++)
      subscribeAllVars(subjects[i]);
  }
}

void ubiLoop()
{
  broker.loop();
}

void ubiSubscribe(Device *d)
{
  if ( subjIndex >=  SUBSCRIPTIONS_BUFF )
  {
    Serial.println("");
    Serial.println("ERROR: ubiSubscribe");
    Serial.println("Limite de suscripciones alcanzado.");
    Serial.println("Puede aumentarlo en IBroker.h.");
    return;
  }
  subjects[subjIndex++] = d;
  subscribeAllVars(d);
}

void subscribeAllVars(Device *d)
{
  for(int j=0; j < d->vSize; j++) {
    Serial.println("");
    Serial.println("susbscribeAllVars()");
    Serial.print("Suscribiendo la variable '");
    Serial.print(VARNAME(d,j));
    Serial.print("' del dispositivo '");
    Serial.print(DEVICE(d));
    Serial.println("'...");
    broker.subscribeLastValue(TOPIC(d,j));
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
    Serial.println("");
    Serial.println("ubiCheck()");
    Serial.println("Se encontraron commits para publicar");
    Serial.print("Publicando ");
    Serial.print(d->vSize);
    Serial.print(" variables del dispositivo ");
    Serial.println(DEVICE(d));
    ubiPush(d);
  }
}

void ubiPush(Device *d)
{
  Serial.println("");
  Serial.println("ubiPush()");
  Serial.println("Publicando JSON...");
  broker.publish(DEVICE(d));
  Serial.println("");
  Serial.println("JSON Publicado exitosamente.");
}

int ubiCommit(Device *d, int var)
{
  if ( NOT_PUBLISH_TIME(d,var) )
    return 0;
  Serial.println("");
  Serial.println("ubiCommit()");
  Serial.print("millis(): ");
  Serial.println(millis());
  Serial.print("Tiempo transcurrido desde la ultima publicacion : ");
  Serial.println(ELAPSED(d,var));
  Serial.print("Escribiendo JSON. Variable ");
  Serial.print(VARNAME(d,var));
  Serial.print(" = ");
  Serial.print(GET(d,var));
  Serial.print(" del dispositivo ");
  Serial.print(DEVICE(d));
  Serial.println("...");
  broker.add(TOPIC_VAL(d,var));
  Serial.println("Escritura exitosa.");
  TIME(d,var) = millis();
  return 1;
}

void ubiSetCallback(void callback(char *, unsigned char *, unsigned int )) 
{
  broker.setCallback(callback);
}
