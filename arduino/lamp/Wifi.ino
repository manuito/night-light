// Web server
ESP8266WebServer server ( 80 );

const char* WEB_PAGE = "<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>NightLight home</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Hello from NightLight!</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <p><h3>Commands:</h3><ul><li><a href=\"/status\">See status</a></li>\
    <li><a href=\"/action?L=1\">Use actions</a></li>\
    <li><a href=\"/action?OP=2\">Set option</a></li></ul></p>\
  </body>\
</html>";

const char* STATUS_JSON = "{\
  \"on\":%d,\
  \"light\":%d,\
  \"time\":\"%02d:%02d:%02d\",\
  \"timeUpd\":%d,\
  \"alarm\":\"%02d:%02d/%d\",\
  \"sleep\":\"%02d:%02d\",\
  \"sleepAct\":%d\
}";

// Status action - specific response
void handleStatus() {
  
  time_t t = time(nullptr);
  int hours = localHours(t);
  int minutes = minute(t);
  int sec = second(t); 

  int onRes = isLightOn() ? 1 : 0;
  int onTime = hasTimeUpdated() ? 1 : 0;
  int alWeek = isAlarmWeekOnly() ? 1 : 0;
  int sleepActive = isSleepStarted() ? 1 : 0; 
  
  char temp[400];

  // Write Json status
  snprintf (temp, 400, STATUS_JSON, 
    onRes,
    getLightAction(),
    hours, minutes, sec,
    onTime,
    getAlarmHours(), getAlarmMinutes(), alWeek,
    getSleepHours(), getSleepMinutes(), sleepActive
  );
  
  server.send ( 200, "application/json", temp );
}

// Standard actions - standard result
void handleAction() {

  String result = "ACK";
  
  // Light action
  if (server.hasArg("L")) {
    char action = server.arg("L").charAt(0);
    setLightAction(action);
  }

  // Switch
  if (server.hasArg("S")) {
    revertLight();
    result = isLightOn() ? "ON" : "OFF";
  }

  // Set alarm
  if (server.hasArg("A")) {
    String argTime = server.arg("A");
    int hours = (argTime.charAt(0) - 48) * 10 + (argTime.charAt(1) - 48);
    int minutes = (argTime.charAt(2) - 48) * 10 + (argTime.charAt(3) - 48);
    bool week = argTime.charAt(4) != 48;
    setAlarm(hours, minutes, week);
  }

  // Set sleep
  if (server.hasArg("R")) {
    String argTime = server.arg("R");
    int hours = (argTime.charAt(0) - 48) * 10 + (argTime.charAt(1) - 48);
    int minutes = (argTime.charAt(2) - 48) * 10 + (argTime.charAt(3) - 48);
    setSleepTime(hours, minutes);
  }
  
  server.send(200, "application/json", "{ \"result\":\"" + result + "\" }");
}

void handleRoot() {
  char temp[400];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  // Write Web page
  snprintf ( temp, 400, WEB_PAGE, hr, min % 60, sec % 60 );
  
  server.send ( 200, "text/html", temp );
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send ( 404, "text/plain", message );
}

void setupWifi () {
  WiFi.mode ( WIFI_STA );
  WiFi.begin ( ssid, password );

  // Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
  }

  localIp = WiFi.localIP() ;

  if ( MDNS.begin ( "esp8266" ) ) {
  }

  server.on ( "/", handleRoot );
  server.on ( "/action", handleAction );
  server.on ( "/status", handleStatus );
  server.onNotFound ( handleNotFound );
  server.begin();
}

void loopWifi () {
  server.handleClient();
}


