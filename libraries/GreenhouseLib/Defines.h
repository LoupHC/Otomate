
#include "Arduino.h"

#ifndef Defines_h
#define Defines_h


//#define DEBUG_ROLLUP
//#define DEBUG_ROLLUP1_TIMING
//#define DEBUG_ROLLUP2_TIMING
//#define DEBUG_ROLLUP3_TIMING
//#define DEBUG_ROLLUP1_POSITION
//#define DEBUG_ROLLUP2_POSITION
//#define DEBUG_ROLLUP1_STATE
//#define DEBUG_ROLLUP2_STATE
//#define DEBUG_ROLLUP1_CYCLE
//#define DEBUG_DEVICE
//#define DEBUG_HEATER
//#define DEBUG_EEPROM
//#define DEBUG_TIMEPOINTS
//#define DEBUG_PROGRAM
//#define DEBUG_SOLARCALC
//#define DEBUG_CLOCK
//#define DEBUG_SETUP
//#define DEBUG_RAMPING
//#define DEBUG_ALARM

//#define TEST_MODE

//Don't change these values unless you know what you are doing

#define MAX_ROLLUPS 3
#define MAX_STAGES 6
#define MAX_TIMEPOINTS 4
#define MAX_DEVICES 6
#define MAX_OVERRIDES 8
#define MAX_TENSIOMETERS 3

#define MAX_CLOCK_OV    3
#define MAX_FLOAT_OV    2
#define MAX_BOOL_OV     3

#define MAX_ALARM_SEQUENCES 8

//EEPROM indexation
#define ROLLUP_INDEX            10
#define DEVICE_INDEX            250
#define TIMEPOINT_INDEX         500
#define GREENHOUSE_INDEX        2000
#define SENSOR_INDEX            2060
#define CLOCKOV_INDEX           2100
#define FLOATOV_INDEX           2800
#define BOOLOV_INDEX            3400

#define SENSOR_INDEX_SIZE       4
#define ROLLUP_INDEX_SIZE       70
#define DEVICE_INDEX_SIZE       20
#define TIMEPOINT_INDEX_SIZE    24
#define CLOCKOV_INDEX_SIZE      12
#define FLOATOV_INDEX_SIZE      17
#define BOOLOV_INDEX_SIZE       10

//Generic vocabulary
#define OFF_VAL      255

//Output type
#define ACT_HIGH     1
#define ACT_LOW      0
#define IO           1
#define MCP23008     0

//Timepoints vocabulary
#define HOUR          2
#define MINUT         1
#define SR            0
#define CLOCK         1
#define SS            2

//weather settings
#define SUN           100
#define MIX           50
#define CLOUD         1
#define AUTO_WEATHER  255

//Device type
#define FANTYPE       0
#define HEATERTYPE    1
#define VALVTYPE      2



//Sensor types
#define DS18B20_TEMP  0
#define SHT1X_TEMP    1
#define OFF_TEMP      2

#define OFF_RAIN      3
#define RG11_BUCKET   2
#define NO_CONTACT    1
#define NC_CONTACT    0

#define SPARKFUN_WIND 0
#define ANALOG_WIND   1
#define OFF_WIND      2

//Calibration type
#define OFFSET        0
#define SLOPE         1

//Generic override address
#define LOCK          50
#define EXT_LOCK          51

//Override comparators
#define ABOVE         1
#define BELOW         2

//Clock override types
#define SUNCONDITIONAL      207
#define ON_PULSE            206
#define UNDERDEFCONDITIONAL 205
#define HDEFCONDITIONAL     204
#define UNDERCONDITIONAL    203
#define HRCONDITIONAL       202
#define SOILCONDITIONAL     201
#define WEATHERCONDITIONAL  200
#define ON                  1
#define OFF                 0

//Limit values
#define MOD_MINIMUM 0
#define MOD_MAXIMUM 15
#define HEATER_MOD_MINIMUM -15
#define HEATER_MOD_MAXIMUM 0
#define MAX_TEMPERATURE 50
#define MIN_TEMPERATURE -10
#define HYST_MINIMUM 0
#define HYST_MAXIMUM 5
#define RAMPING_MINIMUM 0
#define RAMPING_MAXIMUM 180
#define MIN_STAGES 1
#define STAGE_TARGET_MINIMUM 0
#define STAGE_TARGET_MAXIMUM 100
#define MIN_ROTATION_TIME 1
#define MAX_ROTATION_TIME 500
#define MIN_PAUSE_TIME 0
#define MAX_PAUSE_TIME 240
#define MIN_LATITUDE -90
#define MAX_LATITUDE 90
#define MIN_LONGITUDE -180
#define MAX_LONGITUDE 180
#define MIN_TIMEZONE -12
#define MAX_TIMEZONE 14
#define OFFSET_MINIMUM -10
#define OFFSET_MAXIMUM 10
#define SLOPE_MINIMUM 0
#define SLOPE_MAXIMUM 10

//language
#define ENGLISH 0
#define FRENCH 1



#endif
