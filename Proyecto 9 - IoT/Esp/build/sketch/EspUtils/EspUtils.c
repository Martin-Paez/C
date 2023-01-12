#line 1 "c:\\Users\\54911\\Documents\\GitHub\\C\\Proyecto 9 - IoT\\Esp\\EspUtils\\EspUtils.c"
// En Arduino Db.h incluye Db.cpp

bool initSerial(int timeout) {      
  Serial.begin(115200);
  while(! Serial && --timeout > 0)
    delay(1);
  if ( timeout < 0 )
    return false;
  delay(100);
  Serial.printf("\n*******************\nConexion serial exitosa.\n");
  return true;
}

bool connectWifi(int timeout) {
    return connectWifiWith(_SSID, _WIFI_PASS, timeout);
}

bool connectWifiWith(char* ssid, char* pass, int timeout) {
  if ( WiFi.status() == WL_CONNECTED ) {
    Serial.print("Se intenta reconectar Wifi, pero no es necesario");
    return true;
  }
  Serial.print("Estableciendo conexion Wifi...");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED && timeout > 0) {
    delay(timeout -= 500);
    Serial.print(" . ");
  }
  if ( WiFi.status() == WL_CONNECTED ) {
    Serial.printf("\nConexion Wifi exitosa. IP: %s\n", WiFi.localIP().toString().c_str());
    return true;
  }
  Serial.printf("\nConexion Wifi fallida.\n");
  return false;
}

String readCmd(RunT &exe, bool showHelp) {
  return readCmd(Serial, exe, showHelp);
}

String readCmd(bool showHelp) {
  return readCmd(Serial, showHelp);
}

String readCmd(Stream &Serialx, bool showHelp) {
  if (Serialx.available() <= 0)
    return "";
  const char* c = "reset";
  String s = Serialx.readString();
  s.trim();
  Serialx.printf("> %s\n", s);
  if (s == "reset") {
    ESP.restart();
    return "";
  } else if (showHelp)
      Serialx.printf("Comandos: \n  %s\n", c);
  return s;
}

String readCmd(Stream &Serialx, RunT &exe, bool showHelp) {
  String s = readCmd(Serialx, showHelp);
  const char* c = {"stop", "play"};
  if (s == "")
    return s;
  if (s == c[0]) {
    exe.pause();
    return "";
  } else if (s == c[1]) {
    exe.play();
    return "";
  } else if (showHelp)
      Serialx.printf("  %s\n  %s\n", c[0], c[1]);
  return s;
}