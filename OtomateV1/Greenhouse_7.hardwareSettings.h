
//************************************************************
//****************HARDWARE ID*********************
//************************************************************
#define DS18B20             100//Temperature only
#define DHT11               101//Temperature and humidity
#define DHT12               102//Temperature and humidity
#define DHT22               103//Temperature and humidity
#define SHT1X               104//Temperature and humidity
#define RTC_DS3231          200//RTC
#define HYDREON_RG11        300//Rain sensor
//************************************************************
//****************INTERFACE ID*********************
//************************************************************
#define MEMORY_SETTINGS     0
#define DEFAULT_SETTINGS    1
#define EXCEL_SETTINGS      2
#define EXCEL_PROGRAMMER_ID 4444
#define EXCEL_DATALOGGER_ID 5555
//************************************************************
//****************HARDWARE SETTINGS*********************
//************************************************************
//*************SENSORS*************M*******
#define GH_TEMPERATURE      DS18B20
//#define GH_HUMIDITY
//#define OUT_TEMPERATURE     DS18B20
#define RAIN_SENSOR         HYDREON_RG11
//#define WIND_SPEED
//#define LIGHT
#define RTC                 RTC_DS3231
//*************SENSORS PINOUT*********************
#define TEMP2_DATA          4 //connect this pin to the SHT1X data line
#define TEMP2_CLOCK         5 //connect this pin to the SHT1X clock line
#define TEMP1_DATA          6 //connect this pin to the DS18B20 data line
#define TEMP1_CLOCK         7 //connect this pin to the DS18B20 data line

#define RAIN_SWITCH         3
#define WIND_DIGITAL_SENSOR 2
#define TEMP_BACKUP         A8
#define SOIL_MOISTURE3      A7
#define SOIL_MOISTURE2      A6
#define SOIL_MOISTURE1      A5
#define WIND_ANALOG_SENSOR  A3
#define LIGHT_SENSOR        A2
#define CURRENT_SENSOR2     A1
#define CURRENT_SENSOR1     A0
//*************INTERFACE*********************
#define I2CADDR_LCD         0x27
//#define I2CADDR_KEY         0x23 //Otomatillo
#define I2CADDR_KEY         0x26 //Otomate v.1
//#define I2CADDR_KEY         0x3E //Otomate v.2
#define KEYPAD_DISPLAY          //Comment to desactivate keypad interface (only main menu)

//#define COMPUTER_INTERFACE    //Uncomment to desactivate interface programming, only parameters from this sheet are kept in memory

//*************DATA IMPORT/EXPORT*********************

//#define UNIT_TEST         //Uncomment to run unit-tests

//#define EXCEL_PROGRAMMER

//#define EXCEL_DATALOGGER
#define EXPORT_DELAY        60
#define MAX_LINES_PER_FILE  10000
//*************VERSION*********************
#define FIRST_BOOT          113


//************************************************************
//*******************CONTROL PARAMETERS***********************
//************************************************************

//********************GREENHOUSE**************************
#define TIMEPOINTS            4          //# of timepoints
#define ROLLUPS               3          //# of rollups
#define STAGES                4          //# of cool stages (for rollups)
#define DEVICES               6          //# of devices
//********************PINOUT**************************

#define ROLLUP1_OPENING_PIN   0    //connect this pin to the opening relay (west motor)
#define ROLLUP1_CLOSING_PIN   1    //connect this pin to the closing relay (west motor)
#define ROLLUP2_OPENING_PIN   2    //connect this pin to the opening relay (east motor)
#define ROLLUP2_CLOSING_PIN   3    //connect this pin to the closing relay (east motor)
#define DEVICE1_PIN           4    //connect this pin to the heater relay
#define DEVICE2_PIN           5    //connect this pin to the heater relay
#define DEVICE3_PIN           6  //Connect this pin to the fan relay
#define ALARM_PIN             7    //Connect this pin to the safety buzzer

//*************COORD GÉOGRAPHIQUES*********************
#define TIMEZONE              -5
#define LATITUDE              46.00
#define LONGITUDE             -72.00

//TIME AND DATE ARE SET AT FIRST UPLOAD ONLY
//For subsequent RTC programming :
//1. Uncomment (by erasing the "//") the line #define RTC_TIME_SET and #define RTC_DATE_SET and upload to set time
//2. Put back the comment markers("//") and upload again
//ALWAYS SET TO WINTER TIME

//#define RTC_TIME_SET
#define HOUR_SET            11
#define MINUT_SET           19
#define SECOND_SET          0

//#define RTC_DATE_SET
#define DAY_SET             26
#define MONTH_SET           10
#define YEAR_SET            2020
