# FGSensors
Farmgate sensors library v1.0.0

## Supported sensors
* EC Isolated Digital Sensor
* PH Isolated Digital Sensor
* Temperature and Humidity Digital Sensor - SOON
* 
## Features EC and PH Sensors
* Temperature compensation
* 1, 2, 3 point calibration
* Anti interference designs

## EC & PH
```
       Option data bits offset 41 default is 0b11111111
       11 | 11 | 11 | 1 | 1
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
```
