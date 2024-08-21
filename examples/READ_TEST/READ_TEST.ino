#include <Arduino.h>
#include <Wire.h>
#include <FGSensors.h>


FGSensors ec_sensor(0x09);


void setup() {

  Serial.begin(115200);
  ec_sensor.begin();

}


void loop() {
	/*
		check FGSensors.h for all registers you can read or write
	*/
	if (ec_sensor.fgUpdate(1000)) {
		uint8_t iic_address = ec_sensor.fgReadValue(SENS_ADDR_START);
		float ec_value = ec_sensor.fgReadValue(SENS_VALUE_START);
		float ec_raw = ec_sensor.fgReadValue(RAW_VALUE_START);
		float cal_temp = ec_sensor.fgReadValue(CAL_TEMP_START);
		float cal_pt0 = ec_sensor.fgReadValue(CAL_PT_0_START);
		float cal_pt1 = ec_sensor.fgReadValue(CAL_PT_1_START);
		float cal_pt2 = ec_sensor.fgReadValue(CAL_PT_2_START);
		Serial.print("ec:");
		Serial.print(ec_value);
		Serial.print(", raw:");
		Serial.print(ec_raw);
		Serial.print(", cal_temp:");
		Serial.print(cal_temp);
		Serial.print(", cal_pt0:");
		Serial.print(cal_pt0);
		Serial.print(", cal_pt1:");
		Serial.print(cal_pt1);
		Serial.print(", cal_pt2:");
		Serial.println(cal_pt2);		
	}
}