#include <Arduino.h>

int rawValue;
String inputString = "";        // a string to hold incoming data
boolean stringComplete = false; // whether the string is complete
String commandString = "";

boolean isConnected = false;
boolean isStarted = false;

void getCommand()
{
  if (inputString.length() > 0)
  {
    commandString = inputString.substring(1, 5);
  }
}

void serialEvent()
{
  while (Serial.available())
  {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n')
    {
      stringComplete = true;
    }
  }
}

bool heartbeatDetected(int IRSensorPin, int delay)
{
  static int maxValue = 0;
  static bool isPeak = false;

  bool result = false;

  rawValue = analogRead(IRSensorPin);
  rawValue *= (1000 / delay);

  if (rawValue * 4L < maxValue)
  {
    maxValue = rawValue * 0.8;
  }
  if (rawValue > maxValue - (1000 / delay))
  {

    if (rawValue > maxValue)
    {
      maxValue = rawValue;
    }

    if (isPeak == false)
    {
      result = true;
    }
    isPeak = true;
  }
  else if (rawValue < maxValue - (3000 / delay))
  {
    isPeak = false;

    maxValue -= (1000 / delay);
  }
  return result;
}

int ledPin = 13;
int analogPin = 0;

void setup()
{

  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);
  
}

const int delayMsec = 60;

void loop()
{
  static int beatMsec = 0;
  int heartRateBPM = 0;

  if (stringComplete)
  {
    stringComplete = false;
    getCommand();

    if (commandString.equals("DATA"))
    {
      if (heartbeatDetected(analogPin, delayMsec))
      {
        heartRateBPM = 60000 / beatMsec;
        digitalWrite(ledPin, 1);

       
        Serial.println(heartRateBPM);

        beatMsec = 0;
      }
      else
      {
        digitalWrite(ledPin, 0);
      }
      delay(delayMsec);
      beatMsec += delayMsec;
    }

      inputString = "";
  }
}