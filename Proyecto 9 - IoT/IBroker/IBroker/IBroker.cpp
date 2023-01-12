class IBroker
{
    virtual void setCallback(void callback(char *, unsigned char *, unsigned int )) = 0;
    virtual void publish(char* d) = 0;
    virtual bool connected() = 0;
    virtual void loop() = 0;
    virtual void setup() = 0;
    virtual void reconnect() = 0;
    virtual void subscribeLastValue(char *d, char * name) = 0;
    virtual void connectToWifi(const char* WIFI_SSID, const char* WIFI_PASS) = 0;
    virtual void add(char *d, int v) = 0;
};