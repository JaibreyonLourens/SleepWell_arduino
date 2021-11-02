#include <Arduino.h>

int rawValue;
  
  
bool heartbeatDetected (int IRSensorPin, int delay)
{
  static int maxValue = 0;
  static bool isPeak = false;
    
    
  bool result = false;
      
  rawValue = analogRead (IRSensorPin);
  rawValue *= (1000 / delay);
  
  
  if (rawValue * 4L <maxValue) {maxValue = rawValue * 0.8; } 
  if (rawValue> maxValue - (1000 / delay)) {
    
    if (rawValue> maxValue) {
      maxValue = rawValue;
    }
    
    if (isPeak == false) {
      result = true;
    }
    isPeak = true;
  } else if (rawValue <maxValue - (3000 / delay)) {
    isPeak = false;
   
    maxValue -= (1000 / delay);
 }
  return result;
}
  
  

int ledPin = 13;
int analogPin = 0;
  
void setup ()
{
  
  pinMode (ledPin, OUTPUT);
    
 
  Serial.begin (9600);
  Serial.println ("Heartbeat detection sample code.");
}
  
const int delayMsec = 60; 
  

  
void loop ()
{
  static int beatMsec = 0;
  int heartRateBPM = 0;
  if (heartbeatDetected (analogPin, delayMsec)) {
    heartRateBPM = 60000 / beatMsec;
    digitalWrite (ledPin, 1);
  
   
    Serial.print ("Pulse detected:");
    Serial.println (heartRateBPM);
      
    beatMsec = 0;
  } else {
    digitalWrite (ledPin, 0);
  }
  delay (delayMsec);
  beatMsec += delayMsec;
}