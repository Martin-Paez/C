#include "../../Utils/Timer.cpp"

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