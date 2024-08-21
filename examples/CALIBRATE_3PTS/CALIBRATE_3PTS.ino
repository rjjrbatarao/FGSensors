#include <Arduino.h>
#include <Wire.h>
#include <FGSensors.h>


FGSensors ec_sensor(0x09);
FGSensors ph_sensor(0x08);

void setup() {

  Serial.begin(115200);
  ec_sensor.begin();
  ph_sensor.begin();
  /**
    *3 POINT CALIBRATION (default)
		Note: place buffer on ice cold water, proceed calibration if its temperature is 25c.
		Prepare your 3 buffer solutions, 84, 1413 and 12880us/cm
		clean your probe with distilled water
		put it on buffer solution shake lightly and record the value. 
		uncomment each point corresponds to the buffer value and 
		place it on the fgSetCalibration second parameter ie. the 788.0f.
		repeat this with other buffer solution then click upload;
		
		Same process are used for PH sensor, use 4 , 7 and 10 buffer
		solutions
	
	*2 POINT CALIBRATION
		it uses pt_1 and pt_2
		same process for calibration
	*1 POINT CALIBR	ATION
		it uses pt_0 as calibration factor
		to get calibration factor uncomment the ec_value
		prepare known buffer solution ie 1413
		get the reading of ec_value on the known buffer solution 
		cal_factor = ec_value / 1413; 
  */
  //ec_sensor.fgSetCalibration(CAL_PT_0_START, 788.0f); // 84us/cm
  //ec_sensor.fgSetCalibration(CAL_PT_1_START, 11363.0f); // 1413us/cm
  //ec_sensor.fgSetCalibration(CAL_PT_2_START, 47619.0f); //12880us/cm
  
  //ph_raw.fgSetCalibration(CAL_PT_0_START, 1.0f); // 4.01
  //ph_raw.fgSetCalibration(CAL_PT_1_START, 1.5f); // 6.86
  //ph_raw.fgSetCalibration(CAL_PT_2_START, 3.0f); // 9.18
}


void loop() {
	if (ec_sensor.fgUpdate(1000)) {
		//float ec_value = ec_sensor.fgReadValue(SENS_VALUE_START);
		float ec_raw = ec_sensor.fgReadValue(RAW_VALUE_START);
		//Serial.print("ec:");
		//Serial.print(ec_value);		
		Serial.print("ec_raw:");
		Serial.print(ec_raw);
	}
	
	if (ph_sensor.fgUpdate(1000)) {
		//float ph_value = ec_sensor.fgReadValue(SENS_VALUE_START);// for 1 point calibration
		float ph_raw = ec_sensor.fgReadValue(RAW_VALUE_START);
		//Serial.print(", ph:");
		//Serial.print(ph_value);
		Serial.print(", ph_raw:");
		Serial.println(ec_raw);
	}
	
}