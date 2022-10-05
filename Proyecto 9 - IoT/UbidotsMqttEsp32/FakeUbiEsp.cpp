class FakeUbiEsp
{
  public:  // mas simple
    bool simulateConnectionLoss = false;
    int lossFrequency = 10000;
    int timer = 0;
    long unsigned int (*millis)();
      
  public:
    FakeUbiEsp(long unsigned int (*millis)(), bool simulateConnectionLoss) {
      this->millis = millis;
      this->simulateConnectionLoss = simulateConnectionLoss;
    }
    void connectToWifi(const char* WIFI_SSID, const char* WIFI_PASS){}
    void setCallback(void callback(char *, unsigned char *, unsigned int )) {}
    void setup() {}
    void reconnect() {}
    void loop() {}
    void subscribeLastValue(char *d, char * name) {}
    void publish(char* d) {}
    void add(char *d, int v) {}
    bool connected() {
      if ( simulateConnectionLoss 
      && millis() - timer > lossFrequency ) {
        timer = millis();
        return false;
      }
      return true;
    }
};
