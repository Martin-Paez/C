class Timer
{
  public :
    bool playing = true;
    int frequency;
    long unsigned int (*millis)();
    int timer;

  public :
    Timer() {
      
    }
    Timer(int frequency, long unsigned int (*millis)()){
      this->frequency = frequency;
      this->millis = millis;
      reset();
    }

    int elapsed() {
      return millis() - timer;
    }

    bool next() {
      bool ok = isTime();
      if (ok)
        reset();
      return ok;
    }
    
    bool isTime(){
      return playing && elapsed() > frequency;;
    }
    
    void reset(){
      timer = millis();
    }

    void turnOn(){
      playing = true;
      reset();
    }
};

class FakeDevice : public Timer
{
  public :
    unsigned char* requestMsg; 
    unsigned char* responseMsg; 
    unsigned int reqLen;
    unsigned int resLen;
    char *device;
    char *var;
    
  public :
    FakeDevice (int frequency, char* device, char* var,
                unsigned char *requestMsg,
                unsigned int reqLen,
                unsigned char *responseMsg,
                unsigned int resLen,
                long unsigned int (*millis)()) 
    : Timer(frequency, millis)
    {
      this->var = var;
      this->device = device;
      this->requestMsg = requestMsg;
      this->responseMsg = responseMsg;
      this->timer = false;
      this->reqLen = reqLen;
      this->resLen = resLen;
    }
};

class FakeUbiEsp
{
  private :
    void (*callback)(char *, unsigned char *, unsigned int );
    Timer t;
    int fdLen;
    FakeDevice *fd;
    void (*serialStr)(char *);  
    void (*serialInt)(int n);
    long unsigned int (*millis)();
    
  public:
    FakeUbiEsp(Timer connectionLoss, FakeDevice *fd, int fdLen,
               void (*serialStr)(char *), void (*serialInt)(int n),
               long unsigned int (*millis)()) {
      this->t = connectionLoss;
      this->fd = fd;
      this->fdLen = fdLen;
      this->serialStr = serialStr;
      this->serialInt = serialInt;
      this->millis = millis;
    }
    
    void setCallback(void callback(char *, unsigned char *, unsigned int )) {
      this->callback = callback;
      for (int i=0; i<fdLen; i++)
        fd[i].turnOn();
    }
    
    bool cmps(char* a, char* b){
      int i=0;
      while( a[i] != '\0' && b[i] != '\0' && b[i] == a[i++]);
      if ( i > 0 && a[--i] == b[i] ) 
        return true;
      return false;
    }
    
    void publish(char* d) {
      for (int i=0; i<fdLen; i++)
        if ( fd[i].responseMsg != "" && cmps(d,fd[i].device) ) {
          serialStr("");
          serialStr("FakeUbi>");
          serialStr("publish()");
          serialStr("Respondiendo publicacion de :");
          serialStr(fd[i].device);
          callback(fd[i].var,fd[i].responseMsg,fd[i].resLen);
        }
      return;
    }
    
    bool connected() {
      if ( t.isTime() ) {                   
        printElapsed(t,"desconeccion","connected()");
        t.reset();
        return false;
      }
      return true;
    }
    
    void loop() {
      for (int i=0; i<fdLen; i++)
          if ( fd[i].requestMsg != "" && fd[i].isTime() ) {
            printElapsed(fd[i],"callback","loop()");
            serialStr(fd[i].device);
            callback(fd[i].var,fd[i].requestMsg,fd[i].reqLen);
            fd[i].reset();
          }
    }

    void printElapsed(Timer t, char *tag, char* method) {
      serialStr("");
      serialStr("FakeUbi :");
      serialStr(method);
      serialStr("millis(): ");
      serialInt(millis());
      serialStr("Tiempo transcurrido :");
      serialInt(t.elapsed());
      serialStr("desde la operacion :");
      serialStr(tag);
    }
    
    void setup() {}
    void reconnect() {}
    void subscribeLastValue(char *d, char * name) {}
    void connectToWifi(const char* WIFI_SSID, const char* WIFI_PASS){}
    void add(char *d, int v) {}
};
