bool hasTime = false;

int alarmHours = ALARM_HOURS;
int alarmMinutes = ALARM_MINUTES;
bool alarmWeekOnly = ALARM_WEEK_ONLY == 1;

int sleepHours = SLEEP_HOURS;
int sleepMinutes = 0;


void startTimeUdpListen(){
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
}

// Get NTP response
void updateTime() {
  if (!time(nullptr)) {
    // No UDP response yet
  } else {
    hasTime = true;
  }
}

bool hasTimeUpdated(){
  return hasTime;
}

bool isDoSleep(){
  time_t t = time(nullptr);

  int hours = localHours(t);
  int minutes = minute(t);
   
  if(hours == sleepHours && minutes == sleepMinutes && minutes < sleepMinutes + 1){
    return true;
  }
  
  return false;
}


// Alarm timer
bool isDoAlarm(){
  time_t t = time(nullptr);

  int hours = localHours(t);
  int minutes = minute(t);
  int day = weekday(t); // 1 = sunday
   
  if(hours == alarmHours){
    if(minutes >= alarmMinutes && minutes < alarmMinutes + 1){

      // Only on week day (2 - 5)
      if((alarmWeekOnly && day > 1 && day < 6) || !alarmWeekOnly){      
        return true;
      }
    }
  }
  return false;
}

int localHours(time_t t){
  int hours = hour(t);

  if(hours == 0){
    hours = 24;
  }
  
  return hours - TIMEZONE_HOURS_MINUS ;
}

void setAlarm(int hours, int minutes, boolean weekOnly){
  alarmHours = hours;
  alarmMinutes = minutes;
  alarmWeekOnly = weekOnly;
}

void setSleepTime(int hours, int minutes){
  sleepHours = hours;
  sleepMinutes = minutes;
}

int getAlarmHours(){
  return alarmHours;
}

int getAlarmMinutes(){
  return alarmMinutes;
}

bool isAlarmWeekOnly(){
  return alarmWeekOnly;
}

int getSleepHours(){
  return sleepHours;
}

int getSleepMinutes(){
  return sleepMinutes;
}

