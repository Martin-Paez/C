// En Arduino el .cpp se incluye en el .h, y no al revez 

RunT::RunT(int period, void func(), bool paused): f(func) {
    this->period = period;
    last = 0;
    running = ! paused;
}

void RunT::loop(){   
    if (running && (last + period < millis()) ) {
        f();
        last = millis();
    }
}

void RunT::pause() {
    running = false;
}

void RunT::play() {
    running = true;
}
