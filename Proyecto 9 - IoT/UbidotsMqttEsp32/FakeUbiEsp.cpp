class FakeUbiEsp
{
  public:  // mas simple
    bool simulateConnectionLoss = false;
    int lossFrequency = 10000;
    int timer = 0;
    long unsigned int (*millis)();
    bool simulateCallback = false;
    int callBackFrequency = 5000;
    int callBackTimer = 0;
    void (*callback)(char *, unsigned char *, unsigned int );
      
  public:
    FakeUbiEsp(long unsigned int (*millis)(), bool simulateConnectionLoss) {
      this->millis = millis;
      this->simulateConnectionLoss = simulateConnectionLoss;
    }
    void connectToWifi(const char* WIFI_SSID, const char* WIFI_PASS){}
    void setCallback(void callback(char *, unsigned char *, unsigned int )) {
      this->callback = callback;
    }
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
      if ( simulateCallback 
      && millis() - callBackTimer > callBackFrequency ) {
        callBackTimer = millis();
        unsigned char b[] = {'1'}; // si o si, no hay que enviar solo {1}
        callback("presionado",b,1);
      }
      return true;
    }
};
