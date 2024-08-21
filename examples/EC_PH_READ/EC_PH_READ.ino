/**
 * EC and PH Read example
 */

#include <Arduino.h>
#include <Wire.h>
#include "FGSensors.h"


FGSensors ec_sensor(0x09);
FGSensors ph_sensor(0x08);

void setup() {

  Serial.begin(115200);
  ec_sensor.begin();
  ph_sensor.begin();
  
}

void loop() {
  if (ec_sensor.fgUpdate(1000)) {
    ec_sensor.fgSetCalibration(CAL_TEMP_START, 26.1);
    float ec_value = ec_sensor.fgReadValue(SENS_VALUE_START);
    float ec_raw = ec_sensor.fgReadValue(RAW_VALUE_START);
    Serial.print("ec:");
    Serial.print(ec_value);
    Serial.print(", raw:");
    Serial.print(ec_raw);
  }
  if (ph_sensor.fgUpdate(1000)) {
    ph_sensor.fgSetCalibration(CAL_TEMP_START, 26.1);
    float ph_value = ph_sensor.fgReadValue(SENS_VALUE_START);
    float ph_raw = ph_sensor.fgReadValue(RAW_VALUE_START);
    Serial.print(", ph:");
    Serial.print(ph_value);
    Serial.print(", raw:");
    Serial.println(ph_raw);
  }
}