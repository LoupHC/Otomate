/*
  GreenhouseLib.cpp
  Copyright (C)2017 Loup Hébert-Chartrand. All right reserved
  You can find the latest version of this code at :
  https://github.com/LoupHC/GreenhouseLib
  This library is free software{} you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation{} either
  version 2.1 of the License, or (at your option) any later version.
  This code is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY{} without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library{} if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "GreenhouseLib_sensors.h"
unsigned short Sensor::_EEPROMindex = 0;

Sensor::Sensor(){

  _localIndex = SENSOR_INDEX + _EEPROMindex;
  _EEPROMindex += SENSOR_INDEX_SIZE;

  clearRecords();
  _smoothing = 1;
  _calibrationType = OFFSET;
  calibration.setAddress(_localIndex);
  //averageDay.init(6);
  //averageDay.init(24);

}

Sensor::~Sensor(){}

void Sensor::setCalibrationType(byte type){
  if(type == OFFSET){
    _calibrationType = OFFSET;
  }
  else if(type == SLOPE){
    _calibrationType = SLOPE;
  }
}

void Sensor::setSmoothing(float smoothing){
  _smoothing = smoothing;
}

float Sensor::value(){
  return _value;
}

void Sensor::setValue(float value){
  _value = value;
  if(_logMn%10 == 0){
    int remainder = _logMn%10;
    int roundedIndex = (_logMn-remainder)/10;
    _average1h[roundedIndex] = _value;
  }
  if(_logMn == 0){
    _average24h[_logHr] = hourAverage();
  }
}
void Sensor::registerValue(float value){
  if(_calibrationType == OFFSET){
      _value = ((_smoothing*value) + (1-_smoothing)*_value)+calibration.value();
  }
  else if(_calibrationType == SLOPE){
    _value = ((_smoothing*value) + (1-_smoothing)*_value)*calibration.value();
  }
  if(_logMn%10 == 0){
    int remainder = _logMn%10;
    int roundedIndex = (_logMn-remainder)/10;
    _average1h[roundedIndex] = _value;
  }
  if(_logMn == 0){
    _average24h[_logHr] = hourAverage();
  }
}
int Sensor::hourAverage(){
  float sumOfData = 0;
  float dataCount = 0;
  float average = 0;

  for (int x = 0; x < 6; x++){
    if(_average1h[x] != OFF_VAL){
      sumOfData += (float)_average1h[x];
      dataCount++;
    }
  }

  if(dataCount == 0){
    return (int)_value;
  }
  else{
    average = sumOfData/dataCount;
    return (int) average;
  }
}

int Sensor::hourAverage(byte hour){
  return _average24h[hour];
}

int Sensor::logHr(int index){
  int log = _logHr + index;
  if(log < 0){
    log = 24+log;
  }
  return log;
}
//get the average ofevery values recorded in average24h
int Sensor::average12h(){
  float sumOfData = 0;
  float dataCount = 0;
  float average = 0;

  for (int x = 0; x < 12; x++){
    if(_average24h[logHr(-x)] != OFF_VAL){
      sumOfData += (float)_average24h[logHr(-x)];
      dataCount++;
    }
  }
  if(dataCount == 0){
    return (int)_value;
  }
  else{
    average = sumOfData/dataCount;
    return (int) average;
  }
}
//get the average ofevery values recorded in average24h
int Sensor::average24h(){
  float sumOfData = 0;
  float dataCount = 0;
  float average = 0;

  for (int x = 0; x < 24; x++){
    if(_average24h[x] != OFF_VAL){
      sumOfData += (float)_average24h[x];
      dataCount++;
    }
  }
  if(dataCount == 0){
    return (int)_value;
  }
  else{
    average = sumOfData/dataCount;
    return (int) average;
  }
}

//check if some data is missing for 24h average
boolean Sensor::someDataMissing(){
  for (int x = 0; x <24; x++){
    if(_average24h[x] == OFF_VAL){
      return true;
    }
  }
  return false;
}

//get the lowest value of every values recorded in average24h
int Sensor::min24h(){

  float minimum = _value;

  for (int x = 0; x < 6; x++){
    if((_average1h[x] < minimum)&&(_average1h[x] != OFF_VAL)){
      minimum = _average1h[x];
    }
  }
  for (int x = 0; x <24; x++){
    if((_average24h[x] < minimum)&&(_average24h[x] != OFF_VAL)){
      minimum = _average24h[x];
    }
  }

  int returnv = (int) minimum;
  return returnv;
}

//get the highest value of every values recorded in average24h
int Sensor::max24h(){

  float maximum = _value;

  for (int x = 0; x < 6; x++){
    if((_average1h[x] > maximum)&&(_average1h[x] != OFF_VAL)){
      maximum = _average1h[x];
    }
  }for (int x = 0; x < 24; x++){
    if((_average24h[x] > maximum)&&(_average24h[x] != OFF_VAL)){
      maximum = _average24h[x];
    }
  }

  int returnv = (int) maximum;
  return returnv;
}


void Sensor::clearRecords(){
    for(int x = 0; x < 24; x++){
      _average24h[x] = OFF_VAL;
    }
    for(int x = 0; x < 6; x++){
      _average1h[x] = OFF_VAL;
    }
    oneSecondTimer = 0;
}

//record new value every 10 minuts

byte Sensor::_logHr = 0;
byte Sensor::_logMn = 0;

void Sensor::setLogHour(byte hour, byte mn){
  _logHr = hour;
  _logMn = mn;
}


float Humidity::relativeToAbsoluteConversion(float humidity, float temperature){
  return ((6.112 * pow(2.71828,((17.67 * temperature)/(temperature + 243.5))) * humidity * 2.1674) / (273.15 + temperature));
}
//absolute humidity in g/m3
float Humidity::absolute(float temp){
  float absoluteH = relativeToAbsoluteConversion(_value, temp);
  return absoluteH;
}
//humidity deficit in g/m3
float Humidity::deficit(float temp){
  float Hdeficit = relativeToAbsoluteConversion(100, temp)-relativeToAbsoluteConversion(_value, temp);
  return Hdeficit;
}
float Humidity::dew(float temp){
  float gamma = log(_value / 100) + ((17.62 * temp) / (243.5 + temp));
  float dp = 243.5 * gamma / (17.62 - gamma);
  return dp;
}

Radiation::Radiation(){
}

unsigned long Radiation::dayJoules(){
  unsigned long joules = 0;
  if(_logHr > 0){
    for (int x = 0; x < _logHr; x++){
      if(hourAverage(x) != OFF_VAL){
        float computeJoules = hourAverage(x)*0.36;//0.36 : 3600 seconds / 10000cm2
        joules += computeJoules;
      }
    }
  }
  return joules;
}

byte Radiation::lastRadiationLog(byte hourOffset){
  if((_logHr > (Timepoint::sunRise[2] + hourOffset))&&(_logHr <= (Timepoint::sunSet[2]- hourOffset))){
    return _logHr;
  }
  else{
    return Timepoint::sunSet[2]- hourOffset;
  }
}

byte Radiation::autoWeatherRatio(byte hourOffset, unsigned short sunSetpoint){
  unsigned long sumOfData = 0;
  byte dataCount = 0;
  unsigned long computed;

  //moyenne des données échantillonnées
  for (int x = Timepoint::sunRise[2]+hourOffset+1; x <= lastRadiationLog(hourOffset); x++){
    if(hourAverage(x) != OFF_VAL){
      if(hourAverage(x) >= sunSetpoint){
        sumOfData += 100;
        dataCount++;
      }
      else{
        sumOfData += 1;
        dataCount++;
      }
    }
  }
  //Si l'échantillonage a commencé
  if(dataCount != 0){
    if(_logHr == Timepoint::sunRise[2] + hourOffset){
      unsigned long logHrWeather = 1;
      if(hourAverage() >= sunSetpoint){
        logHrWeather = 100;
      }
      else{
        logHrWeather = 1;
      }
      return (logHrWeather + sumOfData/dataCount)/2;
    }
    else{
      return sumOfData/dataCount;
    }
  }
  else{
    if(value() >= sunSetpoint){
      return 100;
    }
    else{
      return 1;
    }
  }
}


Lux::Lux(){
  _cloudLux = 20000;
  _sunLux = 30000;
  _averagePeriod = 4;
  _hourOffset = 2;
}
unsigned long Lux::lux(){
  return (unsigned long)value()*10;
}
unsigned long Lux::luxHourAverage(byte x){
  unsigned long average;
  if(hourAverage(x) != OFF_VAL){
    average = (unsigned long)hourAverage(x)*10;
  }
  else{
    average = 0;
  }
  return average;
}
void Lux::registerLux(unsigned long lux){
  unsigned long computeLux = lux/10;
  int value = (int)computeLux;//lux is divided by 100 to fit in an int
  if(oneSecondTimer >= 1000){
    registerValue(value);
    oneSecondTimer = 0;
  }
}
unsigned long Lux::wattPerMeterSquare(){
  return lux()*0.0079;//conversion constant from lux to w/m2 (for direct sunlight)
}
unsigned long Lux::wattPerMeterSquare(unsigned long value){
  return value*0.0079;//conversion constant from lux to w/m2 (for direct sunlight)
}


unsigned long Lux::averageDailyLux(){

  unsigned long sumOfData = 0;
  int dataCount = 0;

  for (int x = Timepoint::sunRise[2]+_hourOffset; x <= Timepoint::sunSet[2]-_hourOffset; x++){
    if(hourAverage(x) != OFF_VAL){
      if(isOnEdgesOfRadiationLog(x, 0)){
        sumOfData += adjustedLuxValue(luxHourAverage(x),1);
        dataCount++;
      }
      else if(isOnEdgesOfRadiationLog(x, 1)){
        sumOfData += adjustedLuxValue(luxHourAverage(x),0.9);
        dataCount++;
      }
      else if(isOnEdgesOfRadiationLog(x, 2)){
        sumOfData += adjustedLuxValue(luxHourAverage(x),0.8);
        dataCount++;
      }
      else{
        sumOfData += adjustedLuxValue(luxHourAverage(x),0.7);
        dataCount++;
      }
    }
  }


  if(dataCount != 0){
    return sumOfData/dataCount;
  }
  else{
    return 0;
  }
}

unsigned long Lux::averageLuxUntilNow(){

    unsigned long sumOfData = 0;
    byte dataCount = 0;
    unsigned long computed;

    for (int x = Timepoint::sunRise[2]+_hourOffset; x <= lastRadiationLog(); x++){
      if(hourAverage(x) != OFF_VAL){
        if(isOnEdgesOfRadiationLog(x, 0)){
          sumOfData += ((averageDailyLux() + adjustedLuxValue(luxHourAverage(x),1))/2);
          dataCount++;
        }
        else if(isOnEdgesOfRadiationLog(x, 1)){
          sumOfData += adjustedLuxValue(luxHourAverage(x),0.9);
          dataCount++;
        }
        else if(isOnEdgesOfRadiationLog(x, 2)){
          sumOfData += adjustedLuxValue(luxHourAverage(x),0.8);
          dataCount++;
        }
        else{
          sumOfData += adjustedLuxValue(luxHourAverage(x),0.7);
          dataCount++;
        }
      }
    }
    if(dataCount != 0){
      return sumOfData/dataCount;
    }
    else{
      return 0;
    }
}

unsigned long Lux::adjustedLuxValue(unsigned long lux, float adjust){

  float returnLux;

  if (lux >40000){
    return 40000;
  }
  else{
    returnLux = (float)lux*adjust;
    return (unsigned long)returnLux;
  }
}

byte Lux::lastRadiationLog(){
  if((_logHr > (Timepoint::sunRise[2] + _hourOffset))&&(_logHr <= (Timepoint::sunSet[2]- _hourOffset))){
    return _logHr;
  }
  else{
    return Timepoint::sunSet[2]- _hourOffset;
  }
}

boolean Lux::isOnEdgesOfRadiationLog(byte hour, int edgePos){
  if((isBetween((Timepoint::sunRise[2]+_hourOffset+edgePos), 0, hour,0, (Timepoint::sunRise[2]+_hourOffset+edgePos+1), 0))||(isBetween((Timepoint::sunSet[2]-_hourOffset-edgePos),0, hour,0, (Timepoint::sunSet[2]-_hourOffset-edgePos+1),0))){
    return true;
  }
  else{
    return false;
  }
}

byte Lux::autoWeatherRatio(){

  float luxIncrementForWeatherSettings= (( float)_sunLux - (float)_cloudLux)/100;
  byte weatherR = 1;

  for(byte x = 1; x < 100; x++){
    if((float)averageLuxUntilNow() >= ((float)_cloudLux + (luxIncrementForWeatherSettings * x ))){
      weatherR = x;
    }
  }
  return weatherR;
}

//joules/cm2/24h
unsigned long Lux::dayJoules(){
  unsigned long joules = 0;
  if(_logHr > Timepoint::sunRise[2]){
    for (int x = Timepoint::sunRise[2]; x < _logHr; x++){
      if(hourAverage(x) != OFF_VAL){
        float computeJoules = wattPerMeterSquare((unsigned long)hourAverage(x)*10)*0.36;//0.36 : 3600 seconds / 10000cm2
        joules += computeJoules;
      }
    }
  }
  return joules;
}

Current::Current(){
  for(int x = 0; x < 15; x++){
    _currentRead[x] = 0;
  }
  _timer = 0;
}

Current::~Current(){
}


void Current::registerValue(float value){
  /*if(_timer >= 100){
    for (int x = 15; x > 0; x--){
      _currentRead[x] = _currentRead[x-1];
    }
    _currentRead[0] = (0.8*value) + (1-0.8)*currentRead[0];
    _timer = 0;
  }*/
  _currentRead[0] = (0.1*value) + (1-0.1)*_currentRead[0];
}

float Current::average(){
  /*float sumOfData = 0;

  for (int x = 0; x < 15; x++){
      sumOfData += _currentRead[x];
  }*/
  float value = _currentRead[0];//sumOfData/(float)15;
  return value;
}


OnTime::OnTime(){
  for(int x = 0; x < 24;x++){
    _onTime[x] = 0;
  }
}

unsigned long OnTime::onTime24h(){
  unsigned long onTime = 0;
  for(int x = 0; x < 24;x++){
    onTime += (unsigned long)_onTime[x];
  }
  return onTime;
}

unsigned short OnTime::onTimeSinceMidnight(){
  return _onTimeSinceMidnight;
}

byte OnTime::lastHour(byte hour){

  if(hour == 0){
    return 23;
  }
  else{
    return 0;
  }
}

void OnTime::record(byte hr, byte mn,  bool state){

//on since midnight
  if(mn != _lastMinRecorded){
    if((hr == 1)&&(mn == 0)){
      _onTimeSinceMidnight = 0;
    }
    if(state == true){
      _onTimeSinceMidnight ++;
    }
//on 24h
    if(mn == 0){
      _onTime[hr] = 0;
    }
    if(state == true){
      if(mn == 0){
        _onTime[lastHour(hr)]++;
      }
      else{
        _onTime[hr] ++;
      }
    }
  }
  _lastMinRecorded = mn;
}
