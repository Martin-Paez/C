#ifndef _HTTP_CMD_H
#define _HTTP_CMD_H

    #include <list>
    #include <algorithm>
    #include <unordered_set>
    using namespace std;

    #include <WebSocketsServer.h>

    typedef void (*WebSocketServerEvent)(uint8_t num, WStype_t type, uint8_t * payload, size_t length);

    #define CREATE_EVENT_FUNC(name) \
        WebSocketServerEvent create_##name##_event() {    \
            return [](uint8_t num, WStype_t type, uint8_t * payload, size_t length) { \
                name.webSocketEvent(num, type, payload, length); \
            }; \
        }      \

    #define CREATE_WsSerial(name, port)         \
        WsSerial &name = * new WsSerial(port);  \
        CREATE_EVENT_FUNC(name)                 \

    #define CREATE_CMD(cmdName, wsSerialName, port) \
        CREATE_WsSerial(wsSerialName, port)         \
        Cmd & cmdName = wsSerialName;               \

    class Cmd : public Stream {
        public:
            virtual void loop() = 0;
    };

    class SerialDeco : public Cmd {
        public:
            void loop();
    };

    class WsSerial : public Cmd {

        private:

            WebSocketsServer &webSocket;
            unordered_set<int> connections;
            String outBuff;
            int OUT_BUFF_SIZE = 512;
            String inBuff;
            int IN_BUFF_SIZE = 4096;
            
            void addConnection(int n);
            void remConnection(int n);
            void dataArrived(String data);
            void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
            size_t write(uint8_t data) override;

        public:

            WsSerial(int port);
            void begin(WebSocketServerEvent create_name_event());
            int read();
            void loop();
            
            void flush();
            int available();
            int peek();
            
    };

    String cmdPage();

    #include "WsCmd.cpp"        // En Arduino es necesario aca
    #include "cmdPage.cpp"      // En Arduino es necesario aca

#endif

