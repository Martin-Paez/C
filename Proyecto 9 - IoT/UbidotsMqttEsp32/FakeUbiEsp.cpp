class FakeUbiEsp
{
  public:  // mas simple
    bool simulateConnectionLoss = false;
    int lossFrequency = 10000;
    int timer = 0;
    long unsigned int (*millis)();
    bool simulateCallback = true;
    int callBackFrequency = 3000;
    int callBackTimer = 0;
    void (*callback)(char *, unsigned char *, unsigned int );
    char requestMsg = '1';
    char responseMsg = '0';
    char device[6] = "boton";
    char var[11] = "presionado";
    int varIndex = 0;
      
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
    bool cmps(char* a, char* b){
      int i=0;
      while( a[i] != '\0' && b[i] != '\0' && b[i] == a[i++]);
      if ( i > 0 && a[--i] == b[i] ) 
        return true;
      return false;
    }
    void publish(char* d) {
      if ( cmps(d,device) ) {
        unsigned char b[] = {responseMsg}; 
        callback(var,b,1);
        callBackTimer = millis();
        timer = millis();
      }
    }
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
        unsigned char b[] = {requestMsg};
        callback(var,b,1); // no usar {resquestMsg} directamente
      }
      return true;
    }
};
