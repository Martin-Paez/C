#line 1 "c:\\Users\\54911\\Documents\\GitHub\\C\\Proyecto 9 - IoT\\Esp\\EServer\\EServer.cpp"
// En Arduino el .cpp se incluye en el .h, y no al revez 

EServer::EServer(Stream& serialx, int port, void handler(WiFiClient &, String)) 
        : server(* new WiFiServer(port))
        , Serialx(serialx) {
            this->handler = handler;
        }

bool EServer::begin() {
    Serialx.println("Inicializando el servidor HTTP ...");
    if ( WiFi.status() == WL_CONNECTED ) {
        server.begin();
        Serialx.printf("Servidor HTTP listo. Esperando clientes ...\n\n");
        return true;
    }
    Serialx.println("No se pudo inicializar el servidor. No hay conexion wifi."); 
    return false;
}

void EServer::loop() {
    WiFiClient client = server.available();
    if (!client)
        return;
    IPAddress cliIp = client.remoteIP();
    Serialx.print("\nCliente detectado. IP: ");
    Serialx.println(cliIp);
    handler(client, read(client));
    client.stop();
    Serialx.println("Esperando clientes...");
}

String EServer::read(WiFiClient &client) {
    if (!client.available()) {
        Serialx.println("No hay datos para leer");
        return "";
    }
    String req = client.readStringUntil('\r');                                      
    Serialx.printf("El cliente envio: %s\n", req.c_str());
    return req;
}

void EServer::send(WiFiClient &client, String page) {
    Serialx.println("Enviando pagina web...");
    client.print(page);
    delay(1); 
    Serialx.println("Pagina enviada.");
}
