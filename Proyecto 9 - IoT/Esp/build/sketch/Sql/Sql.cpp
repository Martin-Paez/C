#line 1 "c:\\Users\\54911\\Documents\\GitHub\\C\\Proyecto 9 - IoT\\Esp\\Sql\\Sql.cpp"
// En Arduino Sql.h incluye Sql.cpp 

Sql::Sql() : Sql(DB_USER, DB_PASS, DB, dbIp, PORT, Serial) {}

Sql::Sql(Stream &Serialx) : Sql(DB_USER, DB_PASS, DB, dbIp, PORT, Serialx) {}

Sql::Sql(char* user, char* pass, char* db, char* ip, int port, Stream& Serialx)
    : Serialx(Serialx) {
    conn = new MySQL_Connection(new WiFiClient());
    if (WiFi.status() == WL_CONNECTED)
        connect(user, pass, db, dbIp, port);
}

bool Sql::cmd(char* query) {
    try {
        sql->execute(query);
        Serialx.printf("Se envio: %s\n", query);
    } catch(exception e){
        return false;
    }
    return true;
}

bool Sql::connect() {
    return connect(DB_USER, DB_PASS, DB, dbIp, PORT);
}

bool Sql::connect(char* user, char* pass, char* db, char* ip, int port) {
    Serialx.printf("Estableciendo conexion con la base de datos ");
    try {
        conn->connect(*calcIp(ip), port, user, pass, db);
        sql = new MySQL_Cursor(conn);
    } catch(exception e){
        Serialx.println("\nConexion fallida.");
        return false;
    }
    return true;
}

IPAddress* Sql::calcIp(string ip) {
    ip += ".";
    int c=0, n[4];
    do {
        string s = ip.substr(0, ip.find("."));
        n[c] = atoi(s.c_str());
        ip = ip.substr(s.length() + 1, ip.length());
    } while(++c < 4);
    Serialx.printf("(IP: %d.%d.%d.%d)\n",n[0],n[1],n[2],n[3]);
    return new IPAddress(n[0],n[1],n[2],n[3]);
}

Sql::~Sql(){
    Close();
}

void Sql::Close() {
    delete sql;
    conn->close();
    delete conn;
}
