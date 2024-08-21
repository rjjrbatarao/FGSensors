#include <Arduino.h>
#include <Wire.h>
#include <FGSensors.h>

FGSensors ec_sensor(0x09);


void setup() {

  Serial.begin(115200);
  ec_sensor.begin();

  /**
       Option data bits offset 41 default is 0b11111111
       00 | 00 | 00 | 0 | 0
       1  |  2 |  3 | 4 | 5
       toggle bits for options
       1: 2 bit option for calibration type
       2: 2 bit option for software filter level
       3: 2 bit option for software averaging 
       4: 1 bit option for enable disable temperature compensation
       5: 1 bit reserved keep it 1
       
       1: calibration type
         00 - 1 point calibration without calibration factor
         01 - 1 point calibration uses cal_pt0 as calibration factor
         10 - 2 point calibration
         11 - 3 point calibration - default
       2: filter treshold
         00 - 0 - disabled
         01 - 5
         10 - 10
         11 - 20 - default
       3: number of average samples
         00 - 0 - disabled
         01 - 25
         10 - 50
         11 - 100 - default
       4: temperature compensation
         1 - enable - default
         0 - disable
  */
  uint8_t new_option = 0x10111111; // change calibration type to 2 point calibration

  ec_sensor.fgSetOption(new_option); // changing option bits are rare unless for special cases
  
}


void loop() {

}