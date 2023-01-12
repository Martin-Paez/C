#line 1 "c:\\Users\\54911\\Documents\\GitHub\\C\\Proyecto 9 - IoT\\Esp\\RunT\\RunT.cpp"
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
