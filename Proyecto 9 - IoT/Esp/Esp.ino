#include "WsCmd\WsCmd.h"
#include "EspUtils\EspUtils.h"
#include "RunT\RunT.h"
#include "Sql\Sql.h" 
#include "EServer\EServer.h"

void dbTasks();

CREATE_CMD(Serialx, _ws_serial, 8080)

Sql sql(Serialx);
EServer server(Serialx, 80);
RunT db(1000, dbTasks);

void setup()
{
  initSerial();
  while(! connectWifi());
  server.begin(httpTasks);
  _ws_serial.begin(create__ws_serial_event);
  while(! sql.connect());  
  sql.cmd("delete from ldr;");
}

class ISensor {
  public:
    virtual float read() = 0;
}

#include "DHT.h"
class TemperatureSensor : ISensor {
  private:
    Dht &dht;
  public:
    DHTSensor(Dht &dht) : this->dht(dht) {}

    float read() {
      return dht.readTemperature();
    }
}

class HumiditySensor : ISensor {
  private:
    Dht &dht;
  public:
    DHTSensor(Dht &dht) : this->dht(dht) {}

    float read() {
      return dht.readHumidity();
    }
}

void loop() {
  
  UbiBroker broker();
  Dht dht_temp(D4,DHT11);
  Dht dht_hum(D3,DHT22);
  

  Device temp(broker, "dht", "temperatura", new TemperatureSensor(dht_temp));
  Device hum(broker, "dht", "humedad", new HumiditySensor(dht_hum));
  Device ldr(broker, "ldr", "intensidad", new LdrIntensity(D8));

  ldr.loop();
  cmdTasks(Serialx);
  cmdTasks(Serial);
  server.loop();
  db.loop();  
  Serialx.loop();
}

void dbTasks() {
  sql.cmd("insert into ldr(val, time) values (1,2)");
}

void httpTasks(WiFiClient &client, String req) {
  if (req.indexOf("/Cmd")  != -1)
    server.send(client, cmdPage());
}

void cmdTasks(Stream &serial) {
  String s = readCmd(serial, db);
  if ( s == "1" )
    ;
}
