#line 1 "c:\\Users\\54911\\Documents\\GitHub\\C\\Proyecto 9 - IoT\\Esp\\WsCmd\\WsCmd.cpp"
// En Arduino el .cpp se incluye en el .h, y no al revez 

WsSerial::WsSerial(int port)
: webSocket(* new WebSocketsServer(port)) {}

void WsSerial::begin(WebSocketServerEvent createWsEvent()) {
    webSocket.begin();
    webSocket.onEvent(createWsEvent());
}

void WsSerial::webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            printf("Desconectado. ID: [%u] \n", num);
            remConnection(num);
            break;
        case WStype_CONNECTED:
            addConnection(num);
            printf("Conectado. ID: [%u]\n", num);
            break;
        case WStype_TEXT:
            std::string ss(reinterpret_cast<const char *>(payload), length);
            String s = ss.c_str();
            s.trim();
            if ( s.length() > 0 )
                dataArrived(s);
    }
}

size_t WsSerial::write(uint8_t data) {
    Serial.write(data);
    outBuff += (char)data;
    if ( outBuff.length() >= OUT_BUFF_SIZE )
        flush();
    return 1;
}

void WsSerial::loop() {
    if (outBuff.length() > 0)
        flush();
    webSocket.loop();
}

void WsSerial::flush() {
    for (int n : connections)
        webSocket.sendTXT(n, outBuff);
    outBuff = "";
}

void WsSerial::addConnection(int n) {
    this->connections.insert(n);
}

void WsSerial::remConnection(int n) {
    connections.erase(n);
}

int WsSerial::available() {
    return inBuff.length();
}

int WsSerial::read() {
    if (inBuff.length() == 0)
        return -1;
    int i = inBuff.charAt(0);
    inBuff = inBuff.substring(1);
    return i;
}

int WsSerial::peek() {
    if (inBuff.length() > 0) 
        return inBuff.c_str()[0];
    return  String("").c_str()[0];
}

void WsSerial::dataArrived(String data) {
    int n = data.length();
    if (n > IN_BUFF_SIZE) {
        Serial.println("WebSocket: Demasiados datos, se descarta el mensaje");
        return;
    }else if (inBuff.length() + n >= IN_BUFF_SIZE) {   
        Serial.printf("Buffer de entrada lleno. Se descartan %d bytes", n);
        inBuff = inBuff.substring(n);
    }
    inBuff += data;
}

void SerialDeco::loop() {}
