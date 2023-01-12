#ifndef _DB_H
#define _DB_H

    #define DB_USER   "root"
    #define DB_PASS   "1234"
    #define DB        "iot"
    #define PORT      3306
    char* dbIp = "192.168.0.6";

    #include "..\EspUtils\EspUtils.h"
    #include <MySQL_Connection.h>       // 12-2%
    #include <MySQL_Cursor.h>
    #include <string>
    using namespace std;

    class Sql
    {        
        private:
            
            MySQL_Connection* conn;
            MySQL_Cursor* sql;
            Print& Serialx;

            IPAddress* calcIp(string ip);
            void Close();
            
        public:
            
            Sql();
            Sql(Stream& out);
            Sql(char* user, char* pass, char* db, char* ip, int port, Stream& out);
            bool cmd(char* query);
            bool connect(char* user, char* pass, char* db, char* ip, int port);
            bool connect();
            ~Sql();
    };

    #include "Sql.cpp"  // En Arduino es necesario

#endif