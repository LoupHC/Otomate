/*
  GreenhouseLib_FF.cpp
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
#include "GreenhouseLib_FF.h"

//****************************************************
//******************DEVICE FUNCTIONS************************
//****************************************************************/


unsigned short OnOffDevice::_EEPROMindex = 0;
unsigned short OnOffDevice::_counter = 0;

OnOffDevice::OnOffDevice(){
    _localIndex = DEVICE_INDEX + _EEPROMindex;
    _EEPROMindex += DEVICE_INDEX_SIZE;
    _localCounter = _counter;
    _counter++;

    mod.setAddress(_localIndex);
    _localIndex += sizeof(float);

    hyst.setAddress(_localIndex);
    _localIndex += sizeof(float);

    type.setAddress(_localIndex);
    _localIndex += sizeof(byte);

    enabled.setAddress(_localIndex);
    _localIndex += sizeof(boolean);

    lock.setAddress(_localIndex);
    _localIndex += sizeof(boolean);

    lockTarget.setAddress(_localIndex);
    _localIndex += sizeof(byte);

    pulse.setAddress(_localIndex);
    _localIndex += sizeof(bool);


    tensiometer.setAddress(_localIndex);
    _localIndex += sizeof(byte);

    analogSwitch.setAddress(_localIndex);
    _localIndex += sizeof(byte);

    maxOnTime.setAddress(_localIndex);
    _localIndex += sizeof(unsigned short);

    initOverride(LOCK, 0,0, 0,0);
    _tensiometerIndex = 0;
}

void OnOffDevice::initOutput(byte outputType, boolean relayType, byte pin){
  //define opening/closing pins
  output.init(outputType, relayType, pin);
}

boolean OnOffDevice::isLock(){
  if(isActive(LOCK)||isActive(EXT_LOCK)){
    return true;
  }
  else{
    return false;
  }
}
void OnOffDevice::unlock(){
  checkOverride(LOCK, false);
  lockedAndWaiting = false;
  lock.setValue(false);
  output.stop();
  disable(LOCK);
}
void OnOffDevice::resetLockTimer(unsigned long seconds){
  lockedAndWaiting = true;
  overrideWaitingTime = seconds;
  overrideTimer = 0;
}

void OnOffDevice::checkOverrideTimer(){
  if(lockedAndWaiting == true){
    if(overrideTimer >= overrideWaitingTime*1000){
      unlock();
    }
  }
}
void OnOffDevice::keepLockInMemory(byte increment){
  lock.setValue(true);
  lockTarget.setValue(increment);
}
void OnOffDevice::lockOn(){
  enable(LOCK);
  setOverrideTarget(LOCK, true);
  checkOverride(LOCK, true);
}

void OnOffDevice::lockOff(){
  enable(LOCK);
  setOverrideTarget(LOCK, false);
  checkOverride(LOCK, true);
}

void OnOffDevice::forceStart(){
  initOverride(EXT_LOCK, 0, true, 0,0);
  checkOverride(EXT_LOCK, true);
}

void OnOffDevice::forceStop(){
  initOverride(EXT_LOCK, 0, false, 0,0);
  checkOverride(EXT_LOCK, true);
}
void OnOffDevice::unforce(){
  checkOverride(EXT_LOCK, false);
  output.stop();
}

void OnOffDevice::lockOnAndWait(unsigned long seconds){
  if(seconds != 0){
    resetLockTimer(seconds);
  }
  else{
    keepLockInMemory(1);
  }
  lockOn();
}

void OnOffDevice::lockOffAndWait(unsigned long seconds){
  if(seconds != 0){
    resetLockTimer(seconds);
  }
  else{
    keepLockInMemory(0);
  }
  lockOff();
}



void OnOffDevice::setParameters(byte typ, float modif, float hysteresis, bool enab){
  type.setValue(typ);
  hyst.setValue(hysteresis);
  mod.setValue(modif);
  enabled.setValue(enab);
  analogSwitch.setValue(A2);
  tensiometer.setValue(A5);
  maxOnTime.setValue(0);

}
/*
Or one by one...
*/
boolean OnOffDevice::override(){
  if(_activeOverride == false){
    return false;
  }
  else{
    return true;
  }
}

boolean OnOffDevice::isOn(){
  if(output.isActive()){
    return true;
  }
  else{
    return false;
  }
}

void OnOffDevice::desactivateDevice(){
  enabled.setValue(false);
  unlock();
}
void OnOffDevice::activateDevice(){
  enabled.setValue(true);
}
bool OnOffDevice::isActivated(){
  return enabled.value();
}

void OnOffDevice::asignTensiometer(byte tensiom){
  tensiometer.setValue(tensiom);
}
byte OnOffDevice::tensiometerIndex(){
  return tensiometer.value();
}


void OnOffDevice::EEPROMGet(){
  #ifdef DEBUG_EEPROM
    Serial.println(F("-------------------"));
    Serial.print(F("--------DEVICE "));
    Serial.print(_localCounter);
    Serial.println(F("--------"));
    Serial.print(F("Address: "));
    Serial.print(enabled.address());
    Serial.print(F(" - Value :"));
    Serial.print(enabled.value());
    Serial.println(F(" - (Enabled)"));
    Serial.print(F("Address: "));
    Serial.print(hyst.address());
    Serial.print(F(" - Value :"));
    Serial.print(hyst.value());
    Serial.println(F(" - (Hysteresis)"));
    Serial.print(F("Address: "));
    Serial.print(mod.address());
    Serial.print(F(" - Value :"));
    Serial.print(mod.value());
    Serial.println(F("   - (Mod)"));
    Serial.print(F("Address: "));
    Serial.print(type.address());
    if(type.value() == FANTYPE){    Serial.println(F(" - Type : Fan"));    }
    else if(type.value() == HEATERTYPE){    Serial.println(F(" - Type : Heater"));    }
  #endif
}

unsigned short OnOffDevice::nb(){
  return _localCounter;
}

unsigned short OnOffDevice::EEPROMIndexBegin(){
  return DEVICE_INDEX + (DEVICE_INDEX_SIZE*_localCounter);
}

unsigned short OnOffDevice::EEPROMIndexEnd(){
  return _localIndex;
}
/*
Start or stop the device when a certain temperature is reached
Adjust to an external target temperature (Mode VAR_TEMP)
*/

void OnOffDevice::routine(float target, float temp){
  if (isActivated()){
    if(type.value() == FANTYPE){
      fanRoutine(target, temp);
    }
    else if(type.value() == HEATERTYPE){
      heaterRoutine(target, temp);
    }
  }
}

void OnOffDevice::valvRoutine(){
  if (isActivated()){
    if(lock.value() == true){
      if(lockTarget.value() == 0){
        lockOff();
      }
      else{
        lockOn();
      }
    }
    checkOverrideTimer();
    _activeOverride = activeOverride();
    if(activeOverride() == true){
      if((bool)overrideTarget() == true){
        output.start();
      }
      else if((bool)overrideTarget() == false){
        output.stop();
      }
    }
    else{
      output.stop();
    }
  }
}

void OnOffDevice::fanRoutine(float target, float temp){
  if(lock.value() == true){
    if(lockTarget.value() == 0){
      lockOff();
    }
    else{
      lockOn();
    }
  }
  checkOverrideTimer();
  if(activeOverride() == true){
    if((bool)overrideTarget() == true){
      output.start();
    }
    else if((bool)overrideTarget() == false){
      output.stop();
    }
  }
  else{
      float activationTemp = target + mod.value();
      if (temp < (activationTemp - hyst.value())) {
        	output.stop();
      }
      else if (temp > activationTemp) {
        	output.start();
      }
  }
}

void OnOffDevice::heaterRoutine(float target, float temp){
  if(lock.value() == true){
    if(lockTarget.value() == 0){
      lockOff();
    }
    else{
      lockOn();
    }
  }
    checkOverrideTimer();
    if(activeOverride() == true){
      if((bool)overrideTarget() == true){
        output.start();
      }
      else if((bool)overrideTarget() == false){
        output.stop();
      }
    }
    else{
        float activationTemp = target + mod.value();
        if (temp > (activationTemp + hyst.value())) {
          	output.stop();
        }
        else if (temp < activationTemp) {
          	output.start();
        }
    }
  }
