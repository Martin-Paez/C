#ifdef UBIDOTS

#include "UbidotsEsp32Mqtt.h"
#include "IBroker.cpp"

class AdapterUbidots : public IBroker
{
    private:
        Ubidots *ubidots;
        // TODO AdapterUbifots me = &this;

    public:

    AdapterUbidots(const char * token){
      ubidots = new Ubidots(token);
    }

    void setCallback(void callback(char *, unsigned char *, unsigned int )){
        ubidots->setCallback(callback);
    }
    
    void publish(char* d){
        ubidots->publish(d);
    }

    bool connected(){
        ubidots->connected();
    }
    void loop(){
        ubidots->loop();
    }
    
    void setup(){
        ubidots->setup();
    }

    void reconnect() {
        ubidots->reconnect();
    }

    void subscribeLastValue(char *d, char * name) {
        ubidots->subscribeLastValue(d,name);
    }

    void connectToWifi(const char* WIFI_SSID, const char* WIFI_PASS){
        ubidots->connectToWifi(WIFI_SSID, WIFI_PASS);
    }
    
    void add(char *d, int v) {
        ubidots->add(d,v);
    }

};

#endif
