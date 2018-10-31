#include <ESP8266WiFi.h>
#include <WiFiClient.h>

// The lamp to control
String driveLight = "192.168.0.**";

// Status LED
#define LED_STARTED D7

const char *ssid = "****";
const char *password = "****";

// sleep for this many seconds
const int sleepSeconds = 5;


void setup() {
  
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(D0, WAKEUP_PULLUP);

  pinMode(LED_STARTED, OUTPUT);
  digitalWrite(LED_STARTED, HIGH);  

  startWifi();
  callClient(driveLight, "S=1");
  
  ESP.deepSleep(0);
}

void loop() {
}
