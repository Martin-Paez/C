class FakeUbiEsp
{
  private:
    bool connection = false;
  
  public:
    void connectToWifi(const char* WIFI_SSID, const char* WIFI_PASS){}
    void setCallback(void callback(char *, unsigned char *, unsigned int )) {}
    void setup() {}
    void reconnect() {}
    void loop() {}
    void subscribeLastValue(char *d, char * name) {}
    void publish(char* d) {}
    void add(char *d, int v) {}
    bool connected() {
      /*this->connection = not this->connection;
      return this->connection;*/
      return true;
    }
};
