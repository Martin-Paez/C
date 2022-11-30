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