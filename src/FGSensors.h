#ifndef __FG_SENSORS_H_
#define __FG_SENSORS_H_

/**

==========================================================================

   Farm Gate Sensors library
   Library version 1.0.0
   Created by: rjjrbatarao
   License MIT

==========================================================================

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
       2: filter change treshold
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

#include <Arduino.h>
#include <Wire.h>
#include "FGCrc.h"

#define IIC_REGISTER_LENGTH 44
#define IIC_OPTION_BYTE 0b11111111
#define IIC_DEBUG
// #define IIC_OLD_IMPL
#define IIC_MIN_INTERVAL 1000

#ifdef IIC_DEBUG
#define DEBUG(...) printf(__VA_ARGS__)
#else
#define DEBUG(...)
#endif

#if defined(ESP8266)
#define I2C_SDA 5
#define I2C_SCL 4
#elif defined(ESP32)
#define I2C_SDA 21
#define I2C_SCL 22
#else
#define I2C_SDA 4
#define I2C_SCL 5
#endif

/**
   @brief available definitions to use

*/
#define SENS_ADDR_START 0   // read | write
#define SENS_VALUE_START 1  // read only
#define RAW_VALUE_START 5   // read only
#define CAL_TEMP_START 9    // read | write
#define COEFF_TEMP_START 13 // read | write
#define ERR_MEAS_START 17   // read | write
#define ERR_ESTM_START 21   // read  only
#define PROC_VAR_START 25   // read | write
#define CAL_PT_0_START 29   // read | write
#define CAL_PT_1_START 33   // read | write
#define CAL_PT_2_START 37   // read | write
#define OPT_BYTE_START 41   // read | write

class FGSensors
{
public:
  /**
   * @brief Construct a new FGSensors object
   *
   * @param address
   * @param wire
   */
  FGSensors(byte address, TwoWire *wire = &Wire);
  /**
   * @brief Destroy the FGSensors object
   *
   */
  ~FGSensors();
  /**
   * @brief
   *
   */
  void begin();
  /**
   * @brief
   *
     set sensor iic valid address 0x01 - 0xff
  */
  int fgSetAddress(uint8_t new_option);
  /**
   * @brief
   *
   * @param new_option
   * @return * int
   */
  int fgSetOption(uint8_t new_option);
  /**
   * @brief
   *
   * @param offset
   * @param new_value
   * @return int
   */
  int fgSetCalibration(uint8_t offset, float new_value);
  /**
   * @brief
   *
   * @param interval
   * @return true
   * @return false
   */
  bool fgUpdate(uint16_t interval);
  /**
   * @brief
   *
   * @param offset
   * @return float
   */
  float fgReadValue(uint8_t offset);
  /**
   * @brief
   *
   */
  void fgScan();

private:
  TwoWire *_wire = NULL;
  uint8_t _sensor_register[IIC_REGISTER_LENGTH] = {0x00};
  uint8_t _data_bytes[4] = {0x00};
  uint8_t _data_byte = 0x00;
  float _data_float = 0.0f;
  bool _read_available = false;
  uint32_t _last_timeout = 0;
  byte _sensor_address = 0x00;
  int fgWriteByte(uint8_t offset, byte new_data);
  int fgWriteBytes(uint8_t offset, uint8_t *new_data);
  uint8_t fgReadByte(uint8_t offset);
  bool fgIICReady();
  float fgReadBytes(uint8_t offset);
};

#endif //__FG_SENSORS_H_
