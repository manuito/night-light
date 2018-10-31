#define SAT_MAX 253

#define G_RAD  100
#define B_RAD  200

#define LAST_LIGHT 8

// RGB light
NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart800KbpsMethod> strip(RGB_BAND_PIXEL_COUNT, RGB_PIN);

bool lightOn = true;

bool lightSwap = false;
int lightCpt = 0;
int lightCurrentAction;
int lightPreviousAction;

bool alarmActive = false;
bool sleepActive = false;

RgbColor warmWhite = RgbColor(SAT_MAX, SAT_MAX - G_RAD, SAT_MAX - B_RAD);

void startLight(){
    strip.Begin();
}

void setLightOff(){
   lightPreviousAction = lightCurrentAction;
   lightCurrentAction = 1;
   lightOn = false;
   doNothing();
}

void setLightOn(){
    lightCurrentAction = lightPreviousAction;
    lightOn = true;
}

void revertLight(){

  // If ON : stop
  if(lightOn){
     setLightOff();
  } 

  // Else start
  else {
    setLightOn();
  }
}

void setLightAction(byte action){

  // If "n" => Next light
  if(action == 110){
    lightCurrentAction = lightCurrentAction + 1;

    if(lightCurrentAction > LAST_LIGHT){
      lightCurrentAction = 0;
    }
  }

  // Direct set of light with number
  else {
    lightCurrentAction = action - 48;
  }
  
  lightCpt = 0; 
  lightSwap = false;
  doNothing();
}

void setAll(RgbColor color){
  for(int i = 0; i < RGB_BAND_PIXEL_COUNT; i++){
     strip.SetPixelColor(i,color);
  }
}

void doLightShortAction(){
  if(lightOn){
   switch (lightCurrentAction) {
      case 4:
        doRedFadding();
        break;
      case 7:
        doYellowFadding();
        break;
    }
  }
}

void doLightAction(){
  if(lightOn){
   switch (lightCurrentAction) {
    case 0:
      break;
    case 1:
      doFullLight();
      break;
    case 2:
      doFullYellow();
      break;
    case 3:
      doFullRed();
      break;
    case 5:
      doPulseWhite();
      break;
    case 6:
      doChristmas();
      break;
    case 8: 
      doMax();
      break;
    case 9:
      doLightAlarm();
      break;
   }
  }
}

void resetSleep(){
  sleepActive = false;
}

void startSleep(){

  // If ON : stop
  if(lightOn){
     setLightOff();
  }
  doNothing();
  sleepActive = true;
}

bool isSleepStarted(){
  return sleepActive;
}

void resetAlarm(){
  if(alarmActive){
    lightCurrentAction = 1;    
  }
  alarmActive = false;
}

void startAlarm(){
  lightOn = true;
  alarmActive = true;
  setLightAction('9');
}

bool isAlarmStarted(){
  return alarmActive;
}

void doLightAlarm(){
  if(lightSwap){
    doNothing();
    stopBuzz();
  } else {
    doMax();
    doBuzz();
  }
  lightSwap = !lightSwap;
}

void doNothing(){
 setAll(RgbColor(0, 0, 0));
 strip.Show();
}

void doFullLight(){
  setAll(warmWhite);
  strip.Show();
}

void doFullYellow(){
  setAll(RgbColor(SAT_MAX, SAT_MAX - G_RAD, 0));
  strip.Show();
}

void doFullRed(){
  setAll(RgbColor(SAT_MAX, 0, 0));
  strip.Show();
}

void doRedFadding(){

  // Fade red
  int r = lightCpt;
  int g = 0;
  int b = 0;
  
  if(lightCpt > SAT_MAX || lightCpt == 0){
    lightSwap = !lightSwap;
  }

  if(lightSwap){
    lightCpt++;
  } else {
    lightCpt--;
  }
  
  setAll(RgbColor(r,g,b));
  strip.Show();
}

void doPulseWhite(){

  lightCpt = lightCpt + 1;

  if(lightCpt > RGB_BAND_PIXEL_COUNT - 2){
    lightCpt = 0;
  }

  setAll(RgbColor(0, 0, 0));
  strip.SetPixelColor(0,warmWhite);
  strip.SetPixelColor(lightCpt + 1, warmWhite);
  
  strip.Show();
}

void doChristmas(){
  
  if(lightSwap){
    strip.SetPixelColor(0, RgbColor(SAT_MAX, 0, 0));
    strip.SetPixelColor(2, RgbColor(0, SAT_MAX, 0));
    strip.SetPixelColor(4, RgbColor(SAT_MAX, SAT_MAX, 0));
    strip.SetPixelColor(6, RgbColor(0, 0, SAT_MAX));
    
    strip.SetPixelColor(1, RgbColor(0, 0, 0));
    strip.SetPixelColor(3, warmWhite);
    strip.SetPixelColor(5, RgbColor(0, 0, 0));
  } else {
    strip.SetPixelColor(1, RgbColor(0, 0, SAT_MAX));
    strip.SetPixelColor(3, RgbColor(SAT_MAX, SAT_MAX, 0));
    strip.SetPixelColor(5, RgbColor(0, SAT_MAX, 0));
    
    strip.SetPixelColor(0, warmWhite);
    strip.SetPixelColor(2, RgbColor(0, 0, 0));
    strip.SetPixelColor(4, RgbColor(0, 0, 0));
    strip.SetPixelColor(6, warmWhite);
  }
  strip.Show();
  lightSwap = !lightSwap;
}

void doYellowFadding(){

  if(lightCpt < 150){
    lightCpt = 150;
    lightSwap = true;
  }
  
  if(lightCpt >= SAT_MAX){
    lightSwap = false;
  }

  if(lightSwap){
    lightCpt++;
  } else {
    lightCpt--;
  }
  
  // Fade yellow
  int r = lightCpt;
  int g = lightCpt - G_RAD;
  int b = 0;
  
  setAll(RgbColor(r,g,b));
  strip.Show();
}

void doMax(){
  setAll(RgbColor(SAT_MAX,SAT_MAX,SAT_MAX));
  strip.Show();
}

int caped(int val, int maxVal){
  if( val > maxVal) {
    return maxVal - (val - maxVal) ; 
  }
  return val;
}

bool isLightOn(){
  return lightOn;
}

int getLightAction(){
  return lightCurrentAction;
}

