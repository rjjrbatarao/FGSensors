#include <Arduino.h>
#include <Wire.h>
#include <FGSensors.h>


FGSensors ec_sensor(0x09);


void setup() {

  Serial.begin(115200);
  ec_sensor.begin();

  /**
	Make sure to change the 0x09 above with 0x23
	with any examples you will use after
	
	Note: same process can be done with ph sensor
	you hve 0x01 - 0xff address selection please choose
	different address if it conflicts with other
	iic address
  */
  ec_sensor.fgSetAddress(0x23); // address 0x23
	

}


void loop() {
	ec_sensor.fgScan();
	delay(1000);
}