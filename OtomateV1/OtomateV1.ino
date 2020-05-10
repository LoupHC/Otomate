/*
  Greenhouse_prototype_CAPE_v.1.1.ino
  Copyright (C)2017 Loup Hébert-Chartrand. All right reserved

  This code has was made to interface with Arduino-like microcontrollers,
  for inclusion in greenhouse automation devices.

  You can find the latest version of this code at :
  https://github.com/LoupHC/controleur-CAPE


  This code is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This code is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

//***************************************************************************
//***************************************************************************
//***************************************************************************

#include "Arduino.h"

//********************LIBRARIES**************************
#include "Greenhouse_7.hardwareSettings.h"
#include "Greenhouse_2.parameters.h"
#include "Greenhouse_10.debugLines.h"

#include "GreenhouseLib.h"

Greenhouse greenhouse;

//********************POINTERS**************************

  Rollup &R1 = greenhouse.rollup[0];
  Rollup &R2 = greenhouse.rollup[1];
  Rollup &R3 = greenhouse.rollup[2];
  OnOffDevice &D1 = greenhouse.device[0];
  OnOffDevice &D2 = greenhouse.device[1];
  OnOffDevice &D3 = greenhouse.device[2];
  OnOffDevice &D4 = greenhouse.device[3];
  OnOffDevice &D5 = greenhouse.device[4];
  OnOffDevice &D6 = greenhouse.device[5];
  Timepoint &T1 = greenhouse.timepoint[0];
  Timepoint &T2 = greenhouse.timepoint[1];
  Timepoint &T3 = greenhouse.timepoint[2];
  Timepoint &T4 = greenhouse.timepoint[3];



//********************SENSORS**************************
//See "Greenhouse_sensors.h" for sensor functions
#include "Greenhouse_3.sensors.h"
//********************OVERRIDES**************************
//See "Greenhouse_overrides.h" for overrides functions
#include "Greenhouse_4.overrides.h"
//********************IMPORTS/EXPORTS**************************
//See "Greenhouse_importExport.h" for import/export functions
#include "Greenhouse_9.export.h"
#include "Greenhouse_8.import.h"
//********************INTERFACE**************************
//See "Greenhouse_interface.h" for LCD display functions
#include "Greenhouse_5.interface.h"
//********************ERRORS**************************
#include "Preprocessor_error_codes.h"

#ifdef UNIT_TEST
  #include "GreenhouseLib_Tests.h"
#endif

//***************************************************
//********************SETUP**************************
//***************************************************

void setup() {
  //start communication with serial monitor
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  delay(100);
  //start communication internal temp/humididty sensor
  Wire.begin();
  //start communication with LCD
  initLCD(20,4);
  //start communication with keypad
  #ifdef KEYPAD_DISPLAY
    keypad.begin( makeKeymap(keys) );
  #endif
  initSensors();
  getGreenhouseTemp();
  delay(1000);
  if(greenhouse.alarmEnabled.value()){
    initAlarms();
  }

  // change RTC settings
  #ifdef RTC_TIME_SET
    rtc.setTime(HOUR_SET, MINUT_SET, SECOND_SET);
  #endif
  #ifdef RTC_DATE_SET
    rtc.setDate(DAY_SET, MONTH_SET, YEAR_SET);
  #endif
  //get sensors values
  getDateAndTime();
  resetBucketDay = rightNow[3];
  resetBucketMonth = rightNow[4];
  getGreenhouseTemp();
  //getTemperatureBackup();
  getGreenhouseHum();
  greenhouse.startingTime(rightNow);
  greenhouse.startingParameters();
  Sensor::setLogHour(greenhouse.hr(), greenhouse.mn());
  loadParameters();

  lastMinutRecord = greenhouse.mn();
  lastHourRecord = greenhouse.hr();
  lastDayRecord = rightNow[3];


  //Load parameters from EEPROM or Greenhouse_parameters.h
//********************Parameters*******************

  //actual time, timepoint and targetTemp

  pinMode(40, INPUT_PULLUP);
  pinMode(42, INPUT_PULLUP);
  pinMode(44, INPUT_PULLUP);
  pinMode(46, INPUT_PULLUP);


  #ifdef UNIT_TEST
    TestRunner::setTimeout(30);
  #endif
  startRecordingToSD();
  //importParametersFromSD();


}


//***************************************************
//*********************LOOP**************************
//***************************************************
int x = 0;
unsigned long runTime = 0;

void loop() {
  x++;
  runTime = millis();
  //actual time
  getDateAndTime();
  Sensor::setLogHour(greenhouse.hr(), greenhouse.mn());

  //get data from sensors
  getGreenhouseTemp();
  getOutsideTemp();
  getGreenhouseHum();
  getOutsideHum();
  //getTemperatureBackup();
  getRain();
  getWind();
  getRadiation();
  getSoilMoisture();
  autoWeather();
  getOnTime();


  //diplay infos on LCD screen
  lcdDisplay();
  checkOverrides();
  checkOnTime();

  //timepoint and target temperatures definitions, outputs routine
  greenhouse.fullRoutine(rightNow, greenhouseTemperature.value());

  if(!D6.enabled.value()){
    checkAlarms();
  }


  #ifdef EXCEL_DATALOGGER
    dataloggingToExcel();
  #endif
    //dataloggingToSD();
  #ifdef UNIT_TEST
    TestRunner::run();
  #endif
  //Serial.println(millis()-runTime);

}
