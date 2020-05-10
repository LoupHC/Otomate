/*
  GreenhouseLib.h
  Copyright (C)2017 Loup Hébert-Chartrand. All right reserved

  You can find the latest version of this code at :
  https://github.com/LoupHC/GreenhouseLib

  This library is free software; you can redistribute it and/or
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

#include "Arduino.h"
#include "TimeLord.h"
#include "Parameters.h"
#include "Defines.h"
#include "GreenhouseLib_overrides.h"
#include "GreenhouseLib_devices.h"
#include "GreenhouseLib_rollups.h"
#include "GreenhouseLib_FF.h"
#include "GreenhouseLib_timing.h"
#include "GreenhouseLib_sensors.h"



#ifndef GreenhouseLib_h
#define GreenhouseLib_h





class Greenhouse
{
  public:
    Greenhouse();
    ~Greenhouse();
    void initGreenhouse(short timezone, float latitude, float longitude, byte timepoints, byte rollups,byte devices, bool dn, bool wa);
    void setNow(byte rightNow[]);
    void setWeather(byte weather);
    void solarCalculations(byte rightNow[]);
    void startingTime(byte rightNow[]);
    void startingParameters();
    void EEPROMGet();
    void testRollups(boolean state);

    TimeLord myLord;
    #if MAX_TIMEPOINTS >= 1
      Timepoint timepoint[MAX_TIMEPOINTS];
    #endif
    #if MAX_ROLLUPS >= 1
      Rollup rollup[MAX_ROLLUPS];
    #endif
    #if MAX_DEVICES >= 1
      OnOffDevice device[MAX_DEVICES];
    #endif


    //Confirm timepoint, cooling/heating temperatures, routine for each outputs
    void fullRoutine(byte rightNow[], float greenhouseTemperature);
    void fullRoutine(byte rightNow[6], float* coolingTemp, float* heatingTemp);

/*
    void enableOverride(unsigned short ID, boolean state);
    boolean enabled(unsigned short ID);
*/
    byte rightNow(byte index);
    byte hr();
    byte mn();
    byte weather();
    byte nowTimepoint();
    float heatingTemp();
    float coolingTemp();
    boolean isRamping();

    boolParameter  dif;
    boolParameter  prenight;
    boolParameter  daynight;
    boolParameter  weatheradjust;
    byteParameter weatherP;
    shortParameter timezone;         //time zone of your location (Most of Quebec : -5)
    floatParameter longitude;
    floatParameter latitude;
    byteParameter timepoints;        //# of timepoints
    byteParameter rollups;           //# of rollups
    byteParameter devices;              //# of devices

    byteParameter insideTempSensor;
    byteParameter outsideTempSensor;
    byteParameter radiationSensor;
    byteParameter rainSensor;
    byteParameter rainSetpoint;
    byteParameter anemometer;
    byteParameter tensiometer[MAX_TENSIOMETERS+1];
    byteParameter minTensiometerValue;
    byteParameter maxTensiometerValue;
    byteParameter alarmEnabled;
    boolParameter autoWeather;
    byteParameter hourOffset;
    uShortParameter sunSetpoint;
    boolParameter energySavingMode;
    byteParameter language;


    Alarm alarm;
    boolParameter lowTempAlarm;
    floatParameter minTemp;
    boolParameter highTempAlarm;
    floatParameter maxTemp;


  private:
    byte  _overrideProgramCounter;
    byte _rightNow[6];      //actual time
    byte _sunTime[6];       //for sunrise, sunset calculation
    float _heatingTemp;     //reference temperature for heating devices
    float _coolingTemp;     //reference temperature for cooling devices
    float _newHeatingTemp;     //reference temperature for heating devices
    float _newCoolingTemp;     //reference temperature for cooling devices
    float _coolingTempStep;
    float _heatingTempStep;
    boolean _ramping;

    byte _timepoint;        //actual timepoint
    byte _lastTimepoint;


    byte _alarmPin;
    boolean _alarmEnabled;
    boolean _alarmRelayType;
    boolean _alarmIsTriggered;
    float _alarmMin;
    float _alarmMax;

    void initTimeLord(int timezone, float latitude, float longitude);
    void setSunrise(byte rightNow[]);
    void setSunset(byte rightNow[]);
    void selectActualProgram();
    void checkProgramSuccession();
    void setTempCible();
    void updateTempTargets();
    void startRamping();
    boolean otherRollupsAreMoving(byte exception);


    int _localIndex;


};

#endif
