// For RGB Band
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

// For Wifi control
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <time.h>
#include <TimeLib.h>

// RGB shield
#define RGB_PIN D4
#define RGB_BAND_PIXEL_COUNT 7

#define BUZZER_PIN D2
#define BUTTON_PIN D3


// For time management
#define TIMEZONE_HOURS_MINUS 1

#define ALARM_HOURS 7
#define ALARM_MINUTES 15
#define ALARM_WEEK_ONLY 1

#define SLEEP_HOURS 23

// Wifi params
const char *ssid = "****";
const char *password = "****";

IPAddress timeServerIP; // time.nist.gov NTP server address
const char* ntpServerName = "time.nist.gov";
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
const unsigned int udpLocalPort = 2390;      // local port to listen for UDP packets

// Active Wifi IP
IPAddress localIp;


// Various delay
int actionDelay = 500;
int shortestDelay = 25;

// And loop control
int actionStep = 0;
int val = 0;   
int prevVal = 0;

void setup() { 

  pinMode(BUTTON_PIN, INPUT);
  startLight();
  setLightAction('1');
  doLightAction();
  setupWifi();
  startTimeUdpListen();
  startBuzzer();
  doBuzz();
}

void loop() {
  
  val = digitalRead(BUTTON_PIN);     // read the input pin

  // Button control
  if(val != prevVal){
    revertLight();
    prevVal = val;
    delay(50);  
  }

  if(actionStep >= actionDelay){
    stopBuzz();
    doLightAction();
    
    if(!hasTimeUpdated()){
      updateTime();
    } else {
      if(isDoSleep()){
        if(!isSleepStarted()){
          startSleep();
        }
      } else {
        resetSleep();
      }
      
      if(isDoAlarm()){
        if(!isAlarmStarted()){
          startAlarm();
        }
      } else {
        resetAlarm();
      }
    }
    
    actionStep = 0;
  } else {
    actionStep += shortestDelay;
  }
  
  doLightShortAction();

  delay(shortestDelay);

  loopWifi();
}
