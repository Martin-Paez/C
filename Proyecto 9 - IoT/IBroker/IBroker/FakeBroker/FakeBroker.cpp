#include "../IBroker.cpp"
#include "../../Utils/Timer.cpp"
#include "FakeDevice.cpp"

class FakeBroker : public IBroker
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
    FakeBroker(Timer connectionLoss, FakeDevice *fd, int fdLen,
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
    
    bool ucmps(unsigned char* a, char* b){
      return cmps(reinterpret_cast<char *>(a),b);
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
        if ( ! ucmps(fd[i].responseMsg,"") && cmps(d,fd[i].device) ) {
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
          if ( ! ucmps(fd[i].requestMsg,"") && fd[i].isTime() ) {
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
      serialStr("Tiempo transcurrido desde la ultima operacion :");
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
