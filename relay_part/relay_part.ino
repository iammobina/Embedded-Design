#include <TimeLib.h>
#include <SD.h>

int relay = 8;
int LED_behaviour[24] = {0 ,0, 0, 0, 0, 0, 0, 20, 15, 7, 0, 0, 0, 0, 5, 10, 30, 80, 170, 200, 250, 255, 255, 255};
int relay_behaviour[24] = {0 ,0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
double current_hour = 0;
double current_minute = 0;
int relay_status = 0;
int seconds=0, minutes=59, hours=3;
//----------------------------------------------

int set_default_brightness()
{
//   current_hour = hour();
//   current_minute = minute();
   double min_percent = current_minute / 60;
   return (LED_behaviour[(int)current_hour] + min_percent * (LED_behaviour[(int)current_hour + 1] - LED_behaviour[(int)current_hour]));
}


void set_relay_status()
{
   relay_status = relay_behaviour[(int)hours];
  Serial.println("*******");
  Serial.println(relay_status);
     if(relay_status == 1)
       digitalWrite(relay,LOW);
     else
       digitalWrite(relay,HIGH);
  
    delay(100);  
}

void setup()
{
  Serial.begin(115200);
  delay(100);
  pinMode(relay, OUTPUT);

  set_relay_status();
}

void loop() 
{

  time_t t=now();
  seconds=second(t);
  Serial.println(minute(t));
  Serial.println(hour(t));

  minutes +=minute(t);
  if(minutes == 60)
  {
  hours ++;
  }

  set_relay_status();
}
