/*
  Greenhouse_prototype_CAPE_v.1.1.ino
  Copyright (C)2017 Loup Hébert-Chartrand. All right reserved

  This code has been made to interface with Arduino-like microcontrollers,
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
  Lesser General Public License for more details.z

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

/*ROLLUP CALIBRATION - FIXOV OVERRIDE

Conditions :
 - rollup has been fully open or fully close for a while (SAFETY_DELAY)
 - rollup is not moving
Action :
 - full opening or full closing cycle
*/

//OVERRIDES ID
//#define FULL_VENTILATION    90
//#define DESHUM_CYCLE_FIXOV    93
//#define ROLLUP_CALIBRATION  97



#define WINDOV            95
#define CLOCKOV1                 96
#define CLOCKOV2                 97
#define CLOCKOV3                 98
#define RAINOV                99
#define OUTTEMP                100
#define DESHUM              106
#define PULSE              107
#define ANALOG              108
#define MAXONTIME              109

boolean condition1 = false;
boolean condition2 = false;

byte mnVent = 1;
float deshum_hot = 80;
float deshum_cold = 80;


boolean motorFailure[3] = {false,false,false};
boolean overcurrentAlarm = false;

elapsedMillis windTimer[3];
elapsedMillis safetyPositionning = 0;
boolean tensiometerInitialized = false;

void initAlarms(){
  //Add safety alarm
  greenhouse.alarm.init(MCP23008, ACT_LOW, ALARM_PIN);
  greenhouse.alarm.addSequence(1, 1000, 0);
  greenhouse.alarm.addSequence(2, 5000, 0);
  greenhouse.alarm.addSequence(3, 1000, 0);
  greenhouse.alarm.addSequence(4, 1000, 0);
  greenhouse.alarm.addSequence(5, 1000, 0);
  greenhouse.alarm.addSequence(6, 1000, 0);
}

void checkAlarms(){
  if(greenhouse.alarmEnabled.value()){
    if(greenhouse.lowTempAlarm.value() == true){
      greenhouse.alarm.below(greenhouseTemperature.value(),greenhouse.minTemp.value(), 4);
    }
    else{
      greenhouse.alarm.below(1,0,4);
    }
    if(greenhouse.highTempAlarm.value() == true){
      greenhouse.alarm.above(greenhouseTemperature.value(), greenhouse.maxTemp.value(), 5);
    }
    else{
      greenhouse.alarm.above(0,1,5);
    }
    for (byte x = 0; x < greenhouse.rollups.value(); x++){
      if(motorFailure[x] == true){
        overcurrentAlarm = true;
      }
    }
    if((motorFailure[0] == false)&&(motorFailure[1] == false)&&(motorFailure[2] == false)){
      overcurrentAlarm = false;
    }
    greenhouse.alarm.conditionalTo(sensorFailure, 1);
    greenhouse.alarm.conditionalTo(overcurrentAlarm, 2);
    greenhouse.alarm.conditionalTo(tensiomFailure, 3);
    greenhouse.alarm.checkAlarm();
  }
}

boolean rainOverride(byte ID, Rollup rollup){
  float shuttingTemp = greenhouse.coolingTemp() + rollup.stage[0].target.value();
  byte targetIncrement = rollup.overrideTarget(RAINOV);

  if((rollup.activeOverrides() > 0 && !rollup.isActive(RAINOV))||(rollup.activeOverrides() > 1 && rollup.isActive(RAINOV))){
    if((rain == true)&&(rollup.incrementCounter() >= targetIncrement)){
      return true;
    }
    else{
      return false;
    }
  }
  else{
    if((rain == true)&&(rollup.incrementCounter() >= targetIncrement)&&(greenhouseTemperature.value()>shuttingTemp)){
      return true;
    }
    else{
      return false;
    }
  }
}

boolean override40 = false;
boolean override42 = false;
boolean override44 = false;
boolean override46 = false;



void externalOverrides(){
  if(digitalRead(40) == LOW){
    if(override40 == false){
      override40 = true;
      greenhouse.device[2].forceStart();
    }
  }
  else{
    if(override40 == true){
      override40 = false;
      greenhouse.device[2].unforce();
    }
  }
  if(digitalRead(42) == LOW){
    if(override42 == false){
      override42 = true;
      greenhouse.device[3].forceStart();
    }
  }
  else{
    if(override42 == true){
      override42 = false;
      greenhouse.device[3].unforce();
    }

  }
  if(digitalRead(44) == LOW){
    if(override44 == false){
      override44 = true;
      greenhouse.rollup[0].forceAt(100);
    }
  }
  else{
    if(override44 == true){
      override44 = false;
      greenhouse.rollup[0].unforce();
    }
  }
  if(digitalRead(46) == LOW){
    if(override46 == false){
      override46 = true;
      greenhouse.rollup[0].forceAt(0);
    }
  }
  else{
    if(override46 == true){
      override46 = false;
      greenhouse.rollup[0].unforce();
    }
  }
}

void checkOverrideSensors(){

  for (byte x = 0; x < greenhouse.rollups.value(); x++){
    for (byte y = 0; y < MAX_CLOCK_OV; y++){
      if(greenhouse.rollup[x].ovType(CLOCKOV1+y) == HRCONDITIONAL||greenhouse.rollup[x].ovType(CLOCKOV1+y) == HDEFCONDITIONAL){
        if(greenhouse.insideTempSensor.value() != SHT1X_TEMP){
          greenhouse.rollup[x].disable(CLOCKOV1+y);
        }
      }
    }
    if(greenhouse.rainSensor.value() == OFF_RAIN){
      greenhouse.rollup[x].disable(RAINOV);
    }

    if(greenhouse.anemometer.value() == OFF_WIND){
      greenhouse.rollup[x].disable(WINDOV);
    }
//OUTSIDE TEMP
    if(greenhouse.outsideTempSensor.value() == OFF_TEMP){
      greenhouse.rollup[x].disable(OUTTEMP);
    }
  }

  for (byte x = 0; x < greenhouse.devices.value(); x++){
    for (byte y = 0; y < MAX_CLOCK_OV; y++){
      //PROGRAM 1
      if(greenhouse.device[x].ovType(CLOCKOV1+y) == WEATHERCONDITIONAL){
        if(greenhouse.weatheradjust.value() == false){
          greenhouse.device[x].disable(CLOCKOV1+y);
        }
      }
      if(greenhouse.device[x].ovType(CLOCKOV1+y) == SUNCONDITIONAL){
        if(greenhouse.radiationSensor.value() == false){
          greenhouse.device[x].disable(CLOCKOV1+y);
        }
      }
      else if(greenhouse.device[x].ovType(CLOCKOV1+y) == SOILCONDITIONAL){
        if(greenhouse.tensiometer[greenhouse.device[x].tensiometerIndex()].value() == false){
          greenhouse.device[x].disable(CLOCKOV1+y);
        }
        if(soilMoisture[greenhouse.device[x].tensiometerIndex()].value() <= 0){
          greenhouse.device[x].disable(CLOCKOV1+y);
        }
        if(!greenhouse.tensiometer[0].value()&&!greenhouse.tensiometer[1].value()&&!greenhouse.tensiometer[2].value()){
          greenhouse.device[x].disable(CLOCKOV1+y);
        }
      }
      else if(greenhouse.device[x].ovType(CLOCKOV1+y) == HRCONDITIONAL||greenhouse.device[x].ovType(CLOCKOV1+y) == HDEFCONDITIONAL||greenhouse.device[x].ovType(CLOCKOV1+y) == UNDERDEFCONDITIONAL||greenhouse.device[x].ovType(CLOCKOV1+y) == UNDERCONDITIONAL){
        if(greenhouse.insideTempSensor.value() != SHT1X_TEMP){
          greenhouse.rollup[x].disable(CLOCKOV1+y);
        }
      }


      if(greenhouse.device[x].analogSwitch.value() == SOIL_MOISTURE1 && greenhouse.tensiometer[0].value() == false){
        greenhouse.device[x].disable(ANALOG);
      }
      else if(greenhouse.device[x].analogSwitch.value() == SOIL_MOISTURE2 && greenhouse.tensiometer[1].value() == false){
        greenhouse.device[x].disable(ANALOG);
      }
      else if(greenhouse.device[x].analogSwitch.value() == SOIL_MOISTURE3 && greenhouse.tensiometer[2].value() == false){
        greenhouse.device[x].disable(ANALOG);
      }
      else if(greenhouse.device[x].analogSwitch.value() == LIGHT_SENSOR && greenhouse.radiationSensor.value() == false){
        greenhouse.device[x].disable(ANALOG);
      }
    }
  }
  if(greenhouse.radiationSensor.value() == false){
    greenhouse.autoWeather.setValue(false);
  }
}

void checkOverrides(){
  externalOverrides();
  checkOverrideSensors();

  for (byte x = 0; x < greenhouse.rollups.value(); x++){
    for (byte y = 0; y < MAX_CLOCK_OV; y++){
      if(greenhouse.rollup[x].ovType(CLOCKOV1+y) == HRCONDITIONAL){
            if(greenhouse.rollup[x].routinePosition( greenhouseTemperature.value(), greenhouse.coolingTemp()) <= greenhouse.rollup[x].overrideTarget(CLOCKOV1+y)){
              greenhouse.rollup[x].checkOverride(greenhouse.rollup[x].id(CLOCKOV1+y),greenhouse.hr(), greenhouse.mn(), greenhouseHumidity.value());
            }
            else{
              //disable
              greenhouse.rollup[x].checkOverride(greenhouse.rollup[x].id(CLOCKOV1+y),greenhouse.hr(), greenhouse.mn(), 0);
            }

      }
      else if(greenhouse.rollup[x].ovType(CLOCKOV1+y) == HDEFCONDITIONAL){
          if(greenhouse.rollup[x].routinePosition( greenhouseTemperature.value(), greenhouse.coolingTemp()) <= greenhouse.rollup[x].overrideTarget(CLOCKOV1+y)){
            greenhouse.rollup[x].checkOverride(greenhouse.rollup[x].id(CLOCKOV1+y),greenhouse.hr(), greenhouse.mn(), greenhouseHumidity.deficit(greenhouseTemperature.value()));
          }
          else{
            //disable
            greenhouse.rollup[x].checkOverride(greenhouse.rollup[x].id(CLOCKOV1+y),greenhouse.hr(), greenhouse.mn(), 100);
          }
        
      }
      else{
        greenhouse.rollup[x].checkOverride(greenhouse.rollup[x].id(CLOCKOV1+y),greenhouse.hr(), greenhouse.mn());
      }
    }
    greenhouse.rollup[x].checkOverride(RAINOV, rainOverride(RAINOV, greenhouse.rollup[x]));

//WIND OVERRIDE
    if(!greenhouse.rollup[x].isActive(WINDOV)){
      greenhouse.rollup[x].checkOverride(WINDOV, (float)windSpeed);
      windTimer[x] = 0;
    }
    else{
      //every two minuts
      if(windTimer[x] >= 120000){
        greenhouse.rollup[x].checkOverride(WINDOV, (float)windSpeed);
        windTimer[x] = 0;
      }
    }
//OUTSIDE TEMP
  if(outSensorFailure == false){
    greenhouse.rollup[x].checkOverride(OUTTEMP, outsideTemperature.value());
  }
  else{
    greenhouse.rollup[x].checkOverride(OUTTEMP, (float)60);
  }

//OVERCURRENT
    if(greenhouse.rollup[x].overCurrentWarning() == true){
      motorFailure[x] = true;
    }
    else{
      motorFailure[x] = false;
    }
//SAFETY CLOSE-OPENING
    /*
    if((safetyPositionning > 3600000)&&(!greenhouse.device[x].isLock())){//every hour
      if((greenhouse.rollup[x].incrementCounter() == 0)){
        greenhouse.rollup[x].lockCloseAndWait(greenhouse.rollup[x].rotationDown.value());
      }
      else if((greenhouse.rollup[x].incrementCounter() == 100)){
        greenhouse.rollup[x].lockOpenAndWait(greenhouse.rollup[x].rotationUp.value());
      }
      safetyPositionning = 0;
    }*/
  }

  for (byte x = 0; x < greenhouse.devices.value(); x++){
    for (byte y = 0; y < MAX_CLOCK_OV; y++){
      //PROGRAM 1
      if(greenhouse.device[x].ovType(CLOCKOV1+y) == WEATHERCONDITIONAL){
        greenhouse.device[x].checkOverride(greenhouse.device[x].id(CLOCKOV1+y),greenhouse.hr(), greenhouse.mn(), greenhouse.weather());
      }
      else if(greenhouse.device[x].ovType(CLOCKOV1+y) == SOILCONDITIONAL){
        if(greenhouse.device[x].type.value() == VALVTYPE){
          greenhouse.device[x].checkOverride(greenhouse.device[x].id(CLOCKOV1+y),greenhouse.hr(), greenhouse.mn(), soilMoisture[greenhouse.device[x].tensiometerIndex()].value());
        }
      }
      else if(greenhouse.device[x].ovType(CLOCKOV1+y) == SUNCONDITIONAL){
        if(greenhouse.device[x].type.value() == VALVTYPE){
          greenhouse.device[x].checkOverride(greenhouse.device[x].id(CLOCKOV1+y),greenhouse.hr(), greenhouse.mn(), radiation.value()/10);
        }
      }
      else if(greenhouse.device[x].ovType(CLOCKOV1+y) == UNDERDEFCONDITIONAL){
        greenhouse.device[x].checkOverride(greenhouse.device[x].id(CLOCKOV1+y),greenhouse.hr(), greenhouse.mn(), greenhouseHumidity.deficit(greenhouseTemperature.value()));
      }
      else if(greenhouse.device[x].ovType(CLOCKOV1+y) == UNDERCONDITIONAL){
        greenhouse.device[x].checkOverride(greenhouse.device[x].id(CLOCKOV1+y),greenhouse.hr(), greenhouse.mn(), greenhouseHumidity.value());
      }
      else if(greenhouse.device[x].ovType(CLOCKOV1+y) == HDEFCONDITIONAL){
        greenhouse.device[x].checkOverride(greenhouse.device[x].id(CLOCKOV1+y),greenhouse.hr(), greenhouse.mn(), greenhouseHumidity.deficit(greenhouseTemperature.value()));
      }
      else if(greenhouse.device[x].ovType(CLOCKOV1+y) == HRCONDITIONAL){
        greenhouse.device[x].checkOverride(greenhouse.device[x].id(CLOCKOV1+y),greenhouse.hr(), greenhouse.mn(), greenhouseHumidity.value());
      }
      else{
        greenhouse.device[x].checkOverride(greenhouse.device[x].id(CLOCKOV1+y),greenhouse.hr(), greenhouse.mn());
      }
    }
    greenhouse.device[x].checkOverride(ANALOG, (float)((float)analogRead(greenhouse.device[x].analogSwitch.value())* (5.0 / 1023.0)));

    if(greenhouse.device[x].isActive(LOCK)){
      greenhouse.device[x].checkOverride(LOCK, true);
    }
  }
}


void checkOnTime(){
  for (byte x = 0; x < greenhouse.devices.value(); x++){
    if(greenhouse.device[x].onTime.onTimeSinceMidnight() >= greenhouse.device[x].maxOnTime.value() && greenhouse.device[x].maxOnTime.value() != 0){
      greenhouse.device[x].checkOverride(MAXONTIME, true);
    }
    else{
      greenhouse.device[x].checkOverride(MAXONTIME, false);
    }
  }
}
