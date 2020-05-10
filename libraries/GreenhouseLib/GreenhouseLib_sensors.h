/*
  GreenhouseLib_FF.h
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
#include "elapsedMillis.h"
#include "Parameters.h"
#include "Defines.h"
#include "GreenhouseLib_timing.h"
#include "Average.h"



#ifndef GreenhouseLib_sensors_h
#define GreenhouseLib_sensors_h

class Sensor
{
  public:
    Sensor();
    ~Sensor();
    float value();
    int average24h();
    int average12h();
    int hourAverage();
    int hourAverage(byte hour);
    int min24h();
    int max24h();
    void registerValue(float value);
    void setValue(float value);
    void setSmoothing(float smoothing);
    void clearRecords();
    boolean someDataMissing();
    static void setLogHour(byte hour, byte mn);
    void setCalibrationType(byte type);
    floatParameter calibration;


  protected:
    byte _calibrationType;
    void setAverage24h();
    void setAverage12h();
    int logHr(int index);
    static byte _logHr;
    static byte _logMn;
    float _value;
    float _smoothing;
    int _average24hVal;
    int _average12hVal;
    int  _average24h[24];
    int  _average1h[6];
    int _recordCount;
    elapsedMillis oneSecondTimer;
    void  updateLastHourAverage();

    static unsigned short _EEPROMindex;
    unsigned short _localIndex;
};

class Temperature : public Sensor{};

class Humidity : public Sensor
{
  public:
    float absolute(float temp);
    float deficit(float temp);
    float dew(float temp);
  private:
    float relativeToAbsoluteConversion(float humidity, float temp);
};

class Current
{
  public:
    Current();
    ~Current();
    float average();
    void registerValue(float value);


  private:
    float _currentRead[15];
    float _value;
    elapsedMillis _timer;


};
class Radiation : public Sensor
{
  public:
    Radiation();
    unsigned long luxHourAverage(byte hour);
    unsigned long dayJoules();
    byte autoWeatherRatio(byte hourOffset, unsigned short sunSetpoint);

  private :
    byte lastRadiationLog(byte hourOffset);
    friend class Timepoint;
};

class Lux : public Sensor
{
  public:
    Lux();
    void registerLux(unsigned long lux);
    unsigned long lux();
    unsigned long luxHourAverage(byte hour);
    unsigned long wattPerMeterSquare();
    unsigned long wattPerMeterSquare(unsigned long value);
    unsigned long dayJoules();
    byte autoWeatherRatio();
    //byte weatherRatio(unsigned long lux);
    unsigned long averageDailyLux();
    unsigned long averageLuxUntilNow();
    //unsigned long averageFromPreviousLogs(byte previousLogs);
    //unsigned long computedLux();
  private:
    unsigned long adjustedLuxValue(unsigned long lux, float adjust);
    boolean isOnEdgesOfRadiationLog(byte hour, int edgePos);
    byte lastRadiationLog();
    void setLuxSetpoints();
    unsigned long _sunLux;
    unsigned long _cloudLux;
    byte _averagePeriod;
    byte _hourOffset;
    friend class Timepoint;
};

class OnTime{
  public:
    OnTime();
    void record(byte hr,byte mn,  bool state);
    unsigned long onTime24h();
    boolean someDataMissing();
    unsigned short onTimeSinceMidnight();
  private:
    unsigned short _onTime[24];
    unsigned short _offTime;
    unsigned short _onTimeSinceMidnight;
    byte _lastMinRecorded;
    //void eraseRecords();
    byte lastHour(byte hour);
    //byte _hr;
};

#endif
