/*
  AirQuality Demo V1.0.
  connect to A0 to start testing. it will needs about 20s to start 
* By: http://www.seeedstudio.com
*/
#include"AirQuality.h"
#include"Arduino.h"
#include"AirQualityHelper.h"

#define DEBUG_STREAM SerialUSB
#define INPUT_PIN A2

AirQuality airqualitysensor;
AirQualityHelper airQualityHelper;
int current_quality =-1;

void setup()
{
  airQualityHelper.configureTC3a();
  
  DEBUG_STREAM.begin(9600);
  airqualitysensor.init(INPUT_PIN);
}

void loop()
{
  current_quality = airqualitysensor.slope();
  if (current_quality >= 0)// if a valid data returned.
  {
    if (current_quality == 0)
      DEBUG_STREAM.println("High pollution! Force signal active");
    else if (current_quality == 1)
      DEBUG_STREAM.println("High pollution!");
    else if (current_quality == 2)
      DEBUG_STREAM.println("Low pollution!");
    else if (current_quality == 3)
      DEBUG_STREAM.println("Fresh air");
  }
}

// TC3 ISR
void TC3_Handler()
{
    if (TC3->COUNT8.INTFLAG.bit.OVF) {
      
      // Reset interrupt flag
      TC3->COUNT8.INTFLAG.bit.OVF = 1;

      if(airqualitysensor.counter == 2) { //set 2 seconds as a detected duty 
        airqualitysensor.last_vol = airqualitysensor.first_vol;
        airqualitysensor.first_vol = analogRead(airqualitysensor.readPin);
        airqualitysensor.counter = 0;
        airqualitysensor.timer_index = 1;
      }
      else
      {
        airqualitysensor.counter++;
      }
    }
  }






