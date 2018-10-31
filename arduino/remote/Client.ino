#define SERVICE_PORT 80  

void callClient(String host, String action){
  WiFiClient client;
  if (!client.connect(host, SERVICE_PORT)) {
    return;
  }

  // We now create a URI for the request
  String query = "GET /action?";
  query += action;
  query += " HTTP/1.1\r\nHost: ";
  query += host;
  query += "\r\nConnection: close\r\n\r\n";

  // This will send the request to the server
  client.print(query);
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      client.stop();
      return;
    }
  }
}

