#include "GreenhouseLib_overrides.h"




//Override manager

void OverrideManager::OverridesEEPROMGet(){
/*
  hyst.getInEEPROM();
  rotationUp.getInEEPROM();
  rotationDown.getInEEPROM();
  pause.getInEEPROM();
*/
  for(unsigned short x = 0; x < MAX_CLOCK_OV; x++){/*
    stage[x].mod.getInEEPROM();
    stage[x].target.getInEEPROM();*/

    #ifdef DEBUG_EEPROM
      Serial.print(F("-------CLOCK  "));
      Serial.print(clockOv[x].ID.value());
      Serial.println(F("------"));
      Serial.print(F("Address: "));
      Serial.print(clockOv[x].enabled.address());
      Serial.print(F(" - Value :"));
      Serial.print(clockOv[x].enabled.value());
      Serial.println(F(" - (enabled) "));
      Serial.print(F("Address: "));
      Serial.print(clockOv[x].priority.address());
      Serial.print(F(" - Value :"));
      Serial.print(clockOv[x].priority.value());
      Serial.println(F(" - (priority) "));
      Serial.print(F("Address: "));
      Serial.print(clockOv[x].hrStart.address());
      Serial.print(F(" - Value :"));
      Serial.print(clockOv[x].hrStart.value());
      Serial.println(F(" - (hrStart) "));
      Serial.print(F("Address: "));
      Serial.print(clockOv[x].mnStart.address());
      Serial.print(F(" - Value :"));
      Serial.print(clockOv[x].mnStart.value());
      Serial.println(F(" - (mnStart) "));
      Serial.print(clockOv[x].hrStop.address());
      Serial.print(F(" - Value :"));
      Serial.print(clockOv[x].hrStop.value());
      Serial.println(F(" - (hrStop) "));
      Serial.print(F("Address: "));
      Serial.print(clockOv[x].mnStop.address());
      Serial.print(F(" - Value :"));
      Serial.print(clockOv[x].mnStop.value());
      Serial.println(F(" - (mnStop) "));
      Serial.print(clockOv[x].target.address());
      Serial.print(F(" - Value :"));
      Serial.print(clockOv[x].target.value());
      Serial.println(F(" - (target) "));
      Serial.print(F(" - Value :"));
      Serial.print(clockOv[x].pulseOnTime.value());
      Serial.println(F(" - (pulseOn) "));
      Serial.print(F(" - Value :"));
      Serial.print(clockOv[x].pulseOffTime.value());
      Serial.println(F(" - (pulseOff) "));
    #endif
  }
}

bool OverrideManager::isActive(byte id){
  if((isEnabled(id)) && (isInitialized(id))){
    for(int x = 0; x < MAX_CLOCK_OV;x++){
      if(clockOv[x].ID.value() == id){
        return clockOv[x].isActive();
      }
    }
    for(int x = 0; x < MAX_FLOAT_OV;x++){
      if(floatOv[x].ID.value() == id){
        return floatOv[x].isActive();
      }
    }
    for(int x = 0; x < MAX_BOOL_OV;x++){
      if(boolOv[x].ID.value() == id){
        return boolOv[x].isActive();
      }
    }

  }
  return false;
}

bool OverrideManager::isEnabled(byte id){
  if(isInitialized(id)){
    for(int x = 0; x < MAX_CLOCK_OV;x++){
      if(clockOv[x].ID.value() == id){
        return clockOv[x].enabled.value();
      }
    }
    for(int x = 0; x < MAX_FLOAT_OV;x++){
      if(floatOv[x].ID.value() == id){
        return floatOv[x].enabled.value();
      }
    }
    for(int x = 0; x < MAX_BOOL_OV;x++){
      if(boolOv[x].ID.value() == id){
        return boolOv[x].enabled.value();
      }
    }
  }
  return false;
}

bool OverrideManager::isInitialized(byte id){
  for(int x = 0; x < MAX_CLOCK_OV;x++){
    if(clockOv[x].ID.value() == id){
      return true;
    }
  }
  for(int x = 0; x < MAX_FLOAT_OV;x++){
    if(floatOv[x].ID.value() == id){
      return true;
    }
  }
  for(int x = 0; x < MAX_BOOL_OV;x++){
    if(boolOv[x].ID.value() == id){
      return true;
    }
  }
  return false;
}
bool OverrideManager::isPulsing(byte id){
  for(int x = 0; x < MAX_CLOCK_OV;x++){

    if((clockOv[x].ID.value() == id)&&(clockOv[x].pulseOff.value() != 0)){
      return true;
    }
  }
  for(int x = 0; x < MAX_FLOAT_OV;x++){
    if((floatOv[x].ID.value() == id)&&(floatOv[x].pulseOff.value() != 0)){
      return true;
    }
  }
  for(int x = 0; x < MAX_BOOL_OV;x++){
    if((boolOv[x].ID.value() == id)&&(boolOv[x].pulseOff.value() != 0)){
      return true;
    }
  }
  return false;
}

void OverrideManager::enable(byte id){
  for(int x = 0; x < MAX_CLOCK_OV;x++){
    if(clockOv[x].ID.value() == id){
      clockOv[x].enabled.setValue(true);
    }
  }
  for(int x = 0; x < MAX_FLOAT_OV;x++){
    if(floatOv[x].ID.value() == id){
      floatOv[x].enabled.setValue(true);
    }
  }
  for(int x = 0; x < MAX_BOOL_OV;x++){
    if(boolOv[x].ID.value() == id){
      boolOv[x].enabled.setValue(true);
    }
  }
}
void OverrideManager::disable(byte id){
  for(int x = 0; x < MAX_CLOCK_OV;x++){
    if(clockOv[x].ID.value() == id){
      clockOv[x].enabled.setValue(false);
      clockOv[x].desactivate();
    }
  }
  for(int x = 0; x < MAX_FLOAT_OV;x++){
    if(floatOv[x].ID.value() == id){
      floatOv[x].enabled.setValue(false);
      floatOv[x].desactivate();
    }
  }
  for(int x = 0; x < MAX_BOOL_OV;x++){
    if(boolOv[x].ID.value() == id){
      boolOv[x].enabled.setValue(false);
      boolOv[x].desactivate();
    }
  }
}

bool OverrideManager::idIsUnique(byte id){
  for(int x = 0; x < MAX_CLOCK_OV;x++){
    if(clockOv[x].ID.value() == id){
      return false;
    }
  }
  for(int x = 0; x < MAX_FLOAT_OV;x++){
    if(floatOv[x].ID.value() == id){
      return false;
    }
  }
  for(int x = 0; x < MAX_BOOL_OV;x++){
    if(boolOv[x].ID.value() == id){
      return false;
    }
  }
  return true;
}

byte OverrideManager::indexPosition(byte id){
  for(int x = 0; x < MAX_CLOCK_OV;x++){
    if(clockOv[x].ID.value() == id){
      return x;
    }
  }
  for(int x = 0; x < MAX_FLOAT_OV;x++){
    if(floatOv[x].ID.value() == id){
      return x;
    }
  }
  for(int x = 0; x < MAX_BOOL_OV;x++){
    if(boolOv[x].ID.value() == id){
      return x;
    }
  }
  return OFF_VAL;
}

void OverrideManager::clearOverride(byte id){
  for(int x = 0; x < MAX_CLOCK_OV;x++){
    if(clockOv[x].ID.value() == id){
      clockOv[x].ID.setValue(OFF_VAL);
      clockOv[x].enabled.setValue(false);
      clockOv[x].desactivate();
    }
  }
  for(int x = 0; x < MAX_FLOAT_OV;x++){
    if(floatOv[x].ID.value() == id){
      floatOv[x].ID.setValue(OFF_VAL);
      floatOv[x].enabled.setValue(false);
      floatOv[x].desactivate();
    }
  }
  for(int x = 0; x < MAX_BOOL_OV;x++){
    if(boolOv[x].ID.value() == id){
      boolOv[x].ID.setValue(OFF_VAL);
      boolOv[x].enabled.setValue(false);
      boolOv[x].desactivate();
    }
  }
  Serial.println(F("CLEAR-ID NOT VALID"));
}
void OverrideManager::disableOverrides(){
  for(int x = 0; x < MAX_CLOCK_OV;x++){
      clockOv[x].enabled.setValue(false);
      clockOv[x].desactivate();
  }
  for(int x = 0; x < MAX_FLOAT_OV;x++){
      floatOv[x].enabled.setValue(false);
      floatOv[x].desactivate();
  }
  for(int x = 0; x < MAX_BOOL_OV;x++){
      boolOv[x].enabled.setValue(false);
      boolOv[x].desactivate();
  }
}

void OverrideManager::clearOverridesInEEPROM(){
  for(int x = 0; x < MAX_CLOCK_OV;x++){
    clockOv[x].ID.setValue(OFF_VAL);
    clockOv[x].enabled.setValue(false);
    clockOv[x].desactivate();
  }
  for(int x = 0; x < MAX_FLOAT_OV;x++){
    floatOv[x].ID.setValue(OFF_VAL);
    floatOv[x].enabled.setValue(false);
    floatOv[x].desactivate();
  }
  for(int x = 0; x < MAX_BOOL_OV;x++){
    boolOv[x].ID.setValue(OFF_VAL);
    boolOv[x].enabled.setValue(false);
    boolOv[x].desactivate();
  }
}


void OverrideManager::initOverride(byte id, byte prior, byte hrStar, byte mnStar, byte hrSto, byte mnSto, byte state, byte on, byte off){
  if(idIsUnique(id)){
    if(_clockOvCount < MAX_CLOCK_OV){
      clockOv[_clockOvCount].ID.setValue(id);
      clockOv[_clockOvCount].enabled.setValue(true);
      clockOv[_clockOvCount].priority.setValue(prior);
      clockOv[_clockOvCount].hrStart.setValue(hrStar);
      clockOv[_clockOvCount].mnStart.setValue(mnStar);
      clockOv[_clockOvCount].hrStop.setValue(hrSto);
      clockOv[_clockOvCount].mnStop.setValue(mnSto);
      clockOv[_clockOvCount].condition.setValue(0);
      clockOv[_clockOvCount].ovType.setValue(ON);
      clockOv[_clockOvCount].target.setValue(state);
      clockOv[_clockOvCount].pulseOn.setValue(on);
      clockOv[_clockOvCount].pulseOff.setValue(off);
      _clockOvCount +=1;
    }
    else{
      Serial.println(F("Clock override overflow - make some space!"));
    }
  }
  else{
    clockOv[indexPosition(id)].enabled.setValue(true);
    clockOv[indexPosition(id)].priority.setValue(prior);
    clockOv[indexPosition(id)].hrStart.setValue(hrStar);
    clockOv[indexPosition(id)].mnStart.setValue(mnStar);
    clockOv[indexPosition(id)].hrStop.setValue(hrSto);
    clockOv[indexPosition(id)].mnStop.setValue(mnSto);
    clockOv[indexPosition(id)].condition.setValue(0);
    clockOv[indexPosition(id)].ovType.setValue(ON);
    clockOv[indexPosition(id)].target.setValue(state);
    clockOv[indexPosition(id)].pulseOn.setValue(on);
    clockOv[indexPosition(id)].pulseOff.setValue(off);
  }
}
byte OverrideManager::hourStart(byte id){
  for(int x = 0; x < MAX_CLOCK_OV;x++){
    if(clockOv[x].ID.value() == id){
      return clockOv[x].hrStart.value();
    }
  }
  return OFF_VAL;
}

byte OverrideManager::minStart(byte id){
  for(int x = 0; x < MAX_CLOCK_OV;x++){
    if(clockOv[x].ID.value() == id){
      return clockOv[x].mnStart.value();
    }
  }
  return OFF_VAL;
}

byte OverrideManager::hourStop(byte id){
  for(int x = 0; x < MAX_CLOCK_OV;x++){
    if(clockOv[x].ID.value() == id){
      return clockOv[x].hrStop.value();
    }
  }
  return OFF_VAL;
}
byte OverrideManager::minStop(byte id){
  for(int x = 0; x < MAX_CLOCK_OV;x++){
    if(clockOv[x].ID.value() == id){
      return clockOv[x].mnStop.value();
    }
  }
  return OFF_VAL;
}

byte OverrideManager::condition(byte id){
  for(int x = 0; x < MAX_CLOCK_OV;x++){
    if(clockOv[x].ID.value() == id){
      return clockOv[x].condition.value();
    }
  }
  return OFF_VAL;
}

byte OverrideManager::ovType(byte id){
  for(int x = 0; x < MAX_CLOCK_OV;x++){
    if(clockOv[x].ID.value() == id){
      return clockOv[x].ovType.value();
    }
  }
  return OFF_VAL;
}

unsigned short OverrideManager::remainingTime(byte hr, byte mn, byte id){
  for(int x = 0; x < MAX_CLOCK_OV;x++){
    if(clockOv[x].ID.value() == id){
      return clockOv[x].remainingTime(hr, mn);
    }
  }
  return 0;
}

void OverrideManager::initOverride(byte id, byte prior, byte comp, float var, float hys, byte state, byte on, byte off){
  if(idIsUnique(id)){
    if(_floatOvCount < MAX_FLOAT_OV){
      floatOv[_floatOvCount].ID.setValue(id);
      floatOv[_floatOvCount].enabled.setValue(true);
      floatOv[_floatOvCount].priority.setValue(prior);
      floatOv[_floatOvCount].comparator.setValue(comp);
      floatOv[_floatOvCount].floatVar.setValue(var);
      floatOv[_floatOvCount].hyst.setValue(hys);
      floatOv[_floatOvCount].target.setValue(state);
      floatOv[_floatOvCount].pulseOn.setValue(on);
      floatOv[_floatOvCount].pulseOff.setValue(off);
      _floatOvCount +=1;
    }
    else{
      Serial.println(F("Float override overflow - make some space!"));
    }
  }
  else{
  floatOv[indexPosition(id)].enabled.setValue(true);
  floatOv[indexPosition(id)].priority.setValue(prior);
  floatOv[indexPosition(id)].comparator.setValue(comp);
  floatOv[indexPosition(id)].floatVar.setValue(var);
  floatOv[indexPosition(id)].hyst.setValue(hys);
  floatOv[indexPosition(id)].target.setValue(state);
  floatOv[indexPosition(id)].pulseOn.setValue(on);
  floatOv[indexPosition(id)].pulseOff.setValue(off);
  }
}
float OverrideManager::floatVar(byte id){
  for(int x = 0; x < MAX_FLOAT_OV;x++){
    if(floatOv[x].ID.value() == id){
      return floatOv[x].floatVar.value();
    }
  }
  return OFF_VAL;
}
float OverrideManager::hysteresis(byte id){
  for(int x = 0; x < MAX_FLOAT_OV;x++){
    if(floatOv[x].ID.value() == id){
      return floatOv[x].hyst.value();
    }
  }
  return OFF_VAL;
}
float OverrideManager::comparator(byte id){
  for(int x = 0; x < MAX_FLOAT_OV;x++){
    if(floatOv[x].ID.value() == id){
      return floatOv[x].comparator.value();
    }
  }
  return OFF_VAL;
}
/*
void OverrideManager::initOverride(byte id, byte prior, byte state){
  if(idIsUnique(id)){
    if(_boolOvCount < MAX_BOOL_OV){
      boolOv[_boolOvCount].ID.setValue(id);
      boolOv[_boolOvCount].enabled.setValue(true);
      boolOv[_boolOvCount].priority.setValue(prior);
      boolOv[_boolOvCount].target.setValue(state);
      boolOv[_boolOvCount].pulseOff.setValue(0);
      _boolOvCount +=1;
    }
    else{
      Serial.println(F("Bool override overflow - make some space!"));
    }
  }
  else{
    boolOv[indexPosition(id)].enabled.setValue(true);
    boolOv[indexPosition(id)].priority.setValue(prior);
    boolOv[indexPosition(id)].target.setValue(state);
    boolOv[indexPosition(id)].pulseOff.setValue(0);
  }

}
*/
void OverrideManager::initOverride(byte id, byte prior, byte state, byte on, byte off){
  if(idIsUnique(id)){
    if(_boolOvCount < MAX_BOOL_OV){
      boolOv[_boolOvCount].ID.setValue(id);
      boolOv[_boolOvCount].enabled.setValue(true);
      boolOv[_boolOvCount].priority.setValue(prior);
      boolOv[_boolOvCount].target.setValue(state);
      boolOv[_boolOvCount].pulseOn.setValue(on);
      boolOv[_boolOvCount].pulseOff.setValue(off);
      _boolOvCount +=1;
    }
    else{
      Serial.println(F("Bool override overflow - make some space!"));
    }
  }
  else{
    boolOv[indexPosition(id)].enabled.setValue(true);
    boolOv[indexPosition(id)].priority.setValue(prior);
    boolOv[indexPosition(id)].target.setValue(state);
    boolOv[indexPosition(id)].pulseOn.setValue(on);
    boolOv[indexPosition(id)].pulseOff.setValue(off);
  }

}

byte OverrideManager::overrideTarget(byte id){
  for(int x = 0; x < MAX_CLOCK_OV;x++){
    if(clockOv[x].ID.value() == id){
      return clockOv[x].target.value();
    }
  }
  for(int x = 0; x < MAX_FLOAT_OV;x++){
    if(floatOv[x].ID.value() == id){
      return floatOv[x].target.value();
    }
  }
  for(int x = 0; x < MAX_BOOL_OV;x++){
    if(boolOv[x].ID.value() == id){
      if(id == LOCK && boolOv[x].pulseOff.value() != 0){
        if(boolOv[x].lockOff() == true){
          return 0;
        }
        else{
          return boolOv[x].target.value();
        }
      }
    }
  }
  return OFF_VAL;
}

byte OverrideManager::pulseOnTime(byte id){
  for(int x = 0; x < MAX_CLOCK_OV;x++){
    if(clockOv[x].ID.value() == id){
      return clockOv[x].pulseOn.value();
    }
  }
  for(int x = 0; x < MAX_FLOAT_OV;x++){
    if(floatOv[x].ID.value() == id){
      return floatOv[x].pulseOn.value();
    }
  }
  for(int x = 0; x < MAX_BOOL_OV;x++){
    if(boolOv[x].ID.value() == id){
      return boolOv[x].pulseOn.value();
    }
  }
  return OFF_VAL;
}

byte OverrideManager::pulseOffTime(byte id){
  for(int x = 0; x < MAX_CLOCK_OV;x++){
    if(clockOv[x].ID.value() == id){
      return clockOv[x].pulseOff.value();
    }
  }
  for(int x = 0; x < MAX_FLOAT_OV;x++){
    if(floatOv[x].ID.value() == id){
      return floatOv[x].pulseOff.value();
    }
  }
  for(int x = 0; x < MAX_BOOL_OV;x++){
    if(boolOv[x].ID.value() == id){
      return boolOv[x].pulseOff.value();
    }
  }
  return OFF_VAL;
}
byte OverrideManager::activeOverrideID(){
    for(int x = 0; x < MAX_CLOCK_OV;x++){
      if(clockOv[x].isActive()){
        return clockOv[x].ID.value();
      }
    }
    for(int x = 0; x < MAX_FLOAT_OV;x++){
      if(floatOv[x].isActive()){
        return floatOv[x].ID.value();
      }
    }
    for(int x = 0; x < MAX_BOOL_OV;x++){
      if(boolOv[x].isActive()){
        return boolOv[x].ID.value();
      }
    }
  return OFF_VAL;
}
bool OverrideManager::activeOverride(){
    for(int x = 0; x < MAX_CLOCK_OV;x++){
      if(clockOv[x].isActive()){
        return true;
      }
    }
    for(int x = 0; x < MAX_FLOAT_OV;x++){
      if(floatOv[x].isActive()){
        return true;
      }
    }
    for(int x = 0; x < MAX_BOOL_OV;x++){
      if(boolOv[x].isActive()){
        return true;
      }
    }
  return false;
}
byte OverrideManager::activeOverrides(){
  byte count = 0;
    for(int x = 0; x < MAX_CLOCK_OV;x++){
      if(clockOv[x].isActive()){
        count++;
      }
    }
    for(int x = 0; x < MAX_FLOAT_OV;x++){
      if(floatOv[x].isActive()){
        count++;
      }
    }
    for(int x = 0; x < MAX_BOOL_OV;x++){
      if(boolOv[x].isActive()){
        count++;
      }
    }
  return count;
}

byte OverrideManager::overrideTarget(){

  byte priority = MAX_OVERRIDES;
  byte index = MAX_OVERRIDES;
  byte target;

  for(int x = MAX_CLOCK_OV; x > 0;x--){
    if(clockOv[x-1].isActive() && clockOv[x-1].priority.value() < priority){
      priority = clockOv[x-1].priority.value() ;
      target = clockOv[x-1].target.value();
    }
  }
  for(int x = MAX_FLOAT_OV; x > 0;x--){
    if(floatOv[x-1].isActive() && floatOv[x-1].priority.value() < priority){
      priority = floatOv[x-1].priority.value() ;
      target = floatOv[x-1].target.value();
    }
  }
  for(int x = MAX_BOOL_OV; x > 0;x--){
    if(boolOv[x-1].isActive() && boolOv[x-1].priority.value() < priority){
      priority = boolOv[x-1].priority.value() ;
      if(boolOv[x-1].ID.value() == LOCK && boolOv[x-1].pulseOff.value() != 0){
        if(boolOv[x-1].lockOff() == true){
          target = 0;
        }
        else{
          target = boolOv[x-1].target.value();
        }
      }
      else{
        target = boolOv[x-1].target.value();
      }
    }
  }
  if(priority != MAX_OVERRIDES){
    return target;
  }
  else{
    return OFF_VAL;
  }
}


void OverrideManager::setIndexPosition(byte id, byte pos){
  for(int x = 0; x < MAX_CLOCK_OV;x++){
    if(clockOv[x].ID.value() == id){
      clockOv[x].priority.setValue(pos);
    }
  }
  for(int x = 0; x < MAX_FLOAT_OV;x++){
    if(floatOv[x].ID.value() == id){
      floatOv[x].priority.setValue(pos);
    }
  }
  for(int x = 0; x < MAX_BOOL_OV;x++){
    if(boolOv[x].ID.value() == id){
      boolOv[x].priority.setValue(pos);
    }
  }

}
void OverrideManager::setOverrideTarget(byte id, byte target){
  for(int x = 0; x < MAX_CLOCK_OV;x++){
    if(clockOv[x].ID.value() == id){
      clockOv[x].resetPulseTimer();
      clockOv[x].desactivate();
      clockOv[x].target.setValue(target);
    }
  }
  for(int x = 0; x < MAX_FLOAT_OV;x++){
    if(floatOv[x].ID.value() == id){
      clockOv[x].resetPulseTimer();
      clockOv[x].desactivate();
      floatOv[x].target.setValue(target);
    }
  }
  for(int x = 0; x < MAX_BOOL_OV;x++){
    if(boolOv[x].ID.value() == id){
      clockOv[x].resetPulseTimer();
      clockOv[x].desactivate();
      boolOv[x].target.setValue(target);
    }
  }
}
void OverrideManager::setPulseOnTime(byte id, byte pulse){
  for(int x = 0; x < MAX_CLOCK_OV;x++){
    if(clockOv[x].ID.value() == id){
      clockOv[x].pulseOn.setValue(pulse);
    }
  }
  for(int x = 0; x < MAX_FLOAT_OV;x++){
    if(floatOv[x].ID.value() == id){
      floatOv[x].pulseOn.setValue(pulse);
    }
  }
  for(int x = 0; x < MAX_BOOL_OV;x++){
    if(boolOv[x].ID.value() == id){
      boolOv[x].pulseOn.setValue(pulse);
    }
  }
}
void OverrideManager::setPulseOffTime(byte id, byte pulse){
  for(int x = 0; x < MAX_CLOCK_OV;x++){
    if(clockOv[x].ID.value() == id){
      clockOv[x].pulseOff.setValue(pulse);
    }
  }
  for(int x = 0; x < MAX_FLOAT_OV;x++){
    if(floatOv[x].ID.value() == id){
      floatOv[x].pulseOff.setValue(pulse);
    }
  }
  for(int x = 0; x < MAX_BOOL_OV;x++){
    if(boolOv[x].ID.value() == id){
      boolOv[x].pulseOff.setValue(pulse);
    }
  }
}

void OverrideManager::setHourStart(byte id, byte hr){
  for(int x = 0; x < MAX_CLOCK_OV;x++){
    if(clockOv[x].ID.value() == id){
      clockOv[x].hrStart.setValue(hr);
    }
  }
}
void OverrideManager::setMinStart(byte id, byte mn){
  for(int x = 0; x < MAX_CLOCK_OV;x++){
    if(clockOv[x].ID.value() == id){
      clockOv[x].mnStart.setValue(mn);
    }
  }
}
void OverrideManager::setHourStop(byte id, byte hr){
  for(int x = 0; x < MAX_CLOCK_OV;x++){
    if(clockOv[x].ID.value() == id){
      clockOv[x].hrStop.setValue(hr);
    }
  }
}
void OverrideManager::setMinStop(byte id, byte mn){
  for(int x = 0; x < MAX_CLOCK_OV;x++){
    if(clockOv[x].ID.value() == id){
      clockOv[x].mnStop.setValue(mn);
    }
  }
}

void OverrideManager::setCondition(byte id, byte cond){
  for(int x = 0; x < MAX_CLOCK_OV;x++){
    if(clockOv[x].ID.value() == id){
      clockOv[x].condition.setValue(cond);
    }
  }
}

void OverrideManager::setType(byte id, byte cond){
  for(int x = 0; x < MAX_CLOCK_OV;x++){
    if(clockOv[x].ID.value() == id){
      clockOv[x].ovType.setValue(cond);
    }
  }
}
void OverrideManager::setFloatVar(byte id, float var){
  for(int x = 0; x < MAX_FLOAT_OV;x++){
    if(floatOv[x].ID.value() == id){
      floatOv[x].floatVar.setValue(var);
    }
  }
}
void OverrideManager::setHysteresis(byte id, float hysteresis){
  for(int x = 0; x < MAX_FLOAT_OV;x++){
    if(floatOv[x].ID.value() == id){
      floatOv[x].hyst.setValue(hysteresis);
    }
  }
}
void OverrideManager::setComparator(byte id, byte comp){
  for(int x = 0; x < MAX_FLOAT_OV;x++){
    if(floatOv[x].ID.value() == id){
      floatOv[x].comparator.setValue(comp);
    }
  }
}


  ClockRelativeOverride& OverrideManager::id(byte ID){
    for(int x = 0; x < MAX_CLOCK_OV;x++){
      if(clockOv[x].ID.value() == ID){
        return clockOv[x];
      }
    }
    return clockOv[0];
  }

  void OverrideManager::checkOverride(ClockRelativeOverride& override, byte hr, byte mn){
    if(override.enabled.value()){
      override.checkIfActive(hr, mn);
    }
    else{
      override.desactivate();
    }
  }


  void OverrideManager::checkOverride(ClockRelativeOverride& override, byte hr, byte mn, byte condition){
    if(override.enabled.value()){
      override.checkIfActive(hr, mn, condition);
    }
    else{
      override.desactivate();
    }
  }

  void OverrideManager::checkOverride(byte id, bool variable){
    for(int x = 0; x < MAX_BOOL_OV;x++){
      if(boolOv[x].ID.value() == id){
        if(boolOv[x].enabled.value()){
          boolOv[x].checkIfActive(variable);
        }
        else{
          boolOv[x].desactivate();
        }
      }
    }
  }

  void OverrideManager::checkOverride(byte id,  float variable){
    for(int x = 0; x < MAX_FLOAT_OV;x++){
      if(floatOv[x].ID.value() == id){
        if(floatOv[x].enabled.value()){
          floatOv[x].checkIfActive(variable);
        }
        else{
          floatOv[x].desactivate();
        }
      }
    }
  }

//Clock RELATIVE
      unsigned short ClockRelativeOverride::_EEPROMindex = 0;

      ClockRelativeOverride::ClockRelativeOverride(){

        _localIndex = CLOCKOV_INDEX + _EEPROMindex;
        _EEPROMindex += CLOCKOV_INDEX_SIZE;

        _active = false;
        ID.setAddress(_localIndex);
        _localIndex += sizeof(byte);
        priority.setAddress(_localIndex);
        _localIndex += sizeof(byte);
        enabled.setAddress(_localIndex);
        _localIndex += sizeof(boolean);
        target.setAddress(_localIndex);
        _localIndex += sizeof(byte);
        hrStart.setAddress(_localIndex);
        _localIndex += sizeof(byte);
        mnStart.setAddress(_localIndex);
        _localIndex += sizeof(byte);
        hrStop.setAddress(_localIndex);
        _localIndex += sizeof(byte);
        mnStop.setAddress(_localIndex);
        _localIndex += sizeof(byte);
        pulseOff.setAddress(_localIndex);
        _localIndex += sizeof(byte);
        pulseOn.setAddress(_localIndex);
        _localIndex += sizeof(byte);
        condition.setAddress(_localIndex);
        _localIndex += sizeof(byte);
        ovType.setAddress(_localIndex);
        _localIndex += sizeof(byte);

      }

      unsigned short ClockRelativeOverride::remainingTime(byte hr, byte mn){
        unsigned long startTime = (unsigned long)hrStart.value()*60 +(unsigned long)mnStart.value();
        unsigned long stopTime = (unsigned long)hrStop.value()*60 +(unsigned long)mnStop.value();
        unsigned long nowTime = (unsigned long)hr*60 +(unsigned long)mn;
        boolean isSearching = true;
        unsigned long remainingMinutes;

        if(nowTime < startTime){
          nowTime += 24*60;
        }
        if(stopTime <= startTime){
          stopTime += 24*60;
        }

        while (isSearching){
          startTime += pulseOn.value();
          if(startTime > nowTime){
            if(pulseOff.value() != 0){
              remainingMinutes = startTime - nowTime;
              return remainingMinutes;
            }
            else{
              remainingMinutes = stopTime - nowTime;
              return remainingMinutes;
            }
          }
          startTime += pulseOff.value();
          if(startTime > nowTime){
            return 0;
          }
        }
      }

      void ClockRelativeOverride::updateState(byte hr, byte mn, boolean state){
        unsigned long startTime = (unsigned long)hrStart.value()*60 +(unsigned long)mnStart.value();
        unsigned long nowTime = (unsigned long)hr*60 +(unsigned long)mn;
        boolean isSearching = true;

        if(nowTime < startTime){
          nowTime += 24*60;
        }
        if(startTime == nowTime){
          activate();
          isSearching = false;
          return;
        }
        while (isSearching){
          startTime += pulseOn.value();

          if(startTime == nowTime){
            if(state == true){
              activate();
            }
            else{
              desactivate();
            }
          }
          if(startTime >= nowTime){
            isSearching = false;
          }
        }
      }
      void ClockRelativeOverride::findState(byte hr, byte mn){
        unsigned long startTime = (unsigned long)hrStart.value()*60 +(unsigned long)mnStart.value();
        unsigned long nowTime = (unsigned long)hr*60 +(unsigned long)mn;
        boolean isSearching = true;

        if(nowTime < startTime){
          nowTime += 24*60;
        }
        if(nowTime >= startTime && nowTime < (startTime+pulseOn.value())){
          activate();
          isSearching = false;
          return;
        }
        while (isSearching){
          startTime += pulseOn.value();

          if((nowTime >= startTime)&&(nowTime < (startTime+pulseOff.value()))){
            desactivate();
            isSearching = false;
          }
          startTime += pulseOff.value();
          if((nowTime >= startTime)&&(nowTime < (startTime+pulseOn.value()))&&(isSearching)){
            activate();
            isSearching = false;
          }
          if(startTime > nowTime){
            isSearching = false;
          }
        }
      }
      void ClockRelativeOverride::checkIfActive(byte hr, byte mn){

        if(isBetweenExclusively(hrStart.value(), mnStart.value(), hr, mn, hrStop.value(), mnStop.value())){
          findState(hr,  mn);

        }
        else{
          desactivate();
          _pulseTimer = 0;
        }
      }



void ClockRelativeOverride::followPulseAboveTarget(byte hr, byte mn, float cond){
  if(pulseOff.value() != 0){

    if(_lastMin != mn){
      if(cond >= condition.value()){
        findState(hr, mn);
      }
      else if ((isActive())){
        findState(hr, mn);
      }
      else{
        desactivate();
      }
    }
  }
  else{
    if(_lastMin != mn){
      if(cond >= condition.value()){
        updateState(hr, mn, true);
      }
      else{
        updateState(hr, mn, false);
      }
    }
  }
  _lastMin = mn;

}

void ClockRelativeOverride::followPulseUnderTarget(byte hr, byte mn, float cond){

  if(pulseOff.value() != 0){
    if(_lastMin != mn){
      if(cond <= condition.value()){
        findState(hr, mn);
      }
      else if ((isActive())){
        findState(hr, mn);
      }
      else{
        desactivate();
      }
    }
  }
  else{
    if(_lastMin != mn){
      if(cond <= condition.value()){
        updateState(hr, mn, true);
      }
      else{
        updateState(hr, mn, false);
      }
    }
  }
  _lastMin = mn;


}
void ClockRelativeOverride::followPulseAroundTarget(byte hr, byte mn, byte cond){
  boolean metCondition = false;
  switch (condition.value()){
    case 1 : if(cond <= 30){metCondition = true;}break;
    case 50 : if(cond > 30 && cond <= 70){metCondition = true;}break;
    case 100 : if(cond > 70){metCondition = true;}break;
  }
  if(pulseOff.value() != 0){
    if(_lastMin != mn){
      if(metCondition == true){
        findState(hr, mn);
      }
      else if ((isActive())){
        findState(hr, mn);
      }
      else{
        desactivate();
      }
    }
  }
  else{
    if(_lastMin != mn){
      if(metCondition == true){
        updateState(hr, mn, true);
      }
      else{
        updateState(hr, mn, false);
      }
    }
  }
  _lastMin = mn;
}


  void ClockRelativeOverride::checkIfActive(byte hr, byte mn, float cond){

    if(isBetweenExclusively(hrStart.value(), mnStart.value(), hr, mn, hrStop.value(), mnStop.value()) ){
      if(ovType.value() == WEATHERCONDITIONAL){
        followPulseAroundTarget(hr, mn, cond);
      }
      else if(ovType.value() == UNDERCONDITIONAL || ovType.value() == HDEFCONDITIONAL){
        followPulseUnderTarget(hr, mn, cond);
      }
      else{
        followPulseAboveTarget(hr, mn, cond);
      }
    }
    else{
      desactivate();
    }
  }
//FLOAT RELATIVE
      unsigned short FloatRelativeOverride::_EEPROMindex = 0;

      FloatRelativeOverride::FloatRelativeOverride(){
        _localIndex = FLOATOV_INDEX + _EEPROMindex;
        _EEPROMindex += FLOATOV_INDEX_SIZE;

        _active = false;
        ID.setAddress(_localIndex);
        _localIndex += sizeof(byte);
        priority.setAddress(_localIndex);
        _localIndex += sizeof(byte);
        enabled.setAddress(_localIndex);
        _localIndex += sizeof(boolean);
        target.setAddress(_localIndex);
        _localIndex += sizeof(byte);
        comparator.setAddress(_localIndex);
        _localIndex += sizeof(byte);
        floatVar.setAddress(_localIndex);
        _localIndex += sizeof(float);
        hyst.setAddress(_localIndex);
        _localIndex += sizeof(float);
        pulseOff.setAddress(_localIndex);
        _localIndex += sizeof(byte);
        pulseOn.setAddress(_localIndex);
        _localIndex += sizeof(byte);

      }

      void FloatRelativeOverride::checkIfActive(float variable){
        if (comparator.value() == ABOVE){
          if (variable > floatVar.value()){
            if(pulseOff.value() == 0){
              activate();
              _pulseTimer = 0;
            }
            else{
              followPulseCounter();
            }
          }
          else if(variable <= (floatVar.value() - hyst.value())){
            desactivate();
            _pulseTimer = 0;
          }
        }
        if (comparator.value() == BELOW){
          if (variable < floatVar.value()){
            if(pulseOff.value() == 0){
              activate();
              _pulseTimer = 0;
            }
            else{
              followPulseCounter();
            }
          }
          else if(variable >= (floatVar.value() + hyst.value())){
            desactivate();
            _pulseTimer = 0;
          }
        }
      }

//BOOL RELATIVE
      unsigned short BoolRelativeOverride::_EEPROMindex = 0;

      BoolRelativeOverride::BoolRelativeOverride(){

        _localIndex = BOOLOV_INDEX + _EEPROMindex;
        _EEPROMindex += BOOLOV_INDEX_SIZE;

        _active = false;
        ID.setAddress(_localIndex);
        _localIndex += sizeof(byte);
        priority.setAddress(_localIndex);
        _localIndex += sizeof(byte);
        enabled.setAddress(_localIndex);
        _localIndex += sizeof(boolean);
        target.setAddress(_localIndex);
        _localIndex += sizeof(boolean);
        pulseOff.setAddress(_localIndex);
        _localIndex += sizeof(boolean);
        pulseOn.setAddress(_localIndex);
        _localIndex += sizeof(boolean);

      }

      void BoolRelativeOverride::checkIfActive(bool variable){
        if(variable == true){
          if(pulseOff.value() == 0){
            activate();
            _pulseTimer = 0;
          }
          //special function for onoffdevices
          else if(ID.value() == LOCK && pulseOff.value() != 0){
            followLockPulseCounter();
          }
          else{
            followPulseCounter();
          }
        }
        else{
          desactivate();
          _pulseTimer = 0;
        }
      }
//OVERRIDE
/*

      bool OverrideR::initialized(){
        if(ID.value() != OFF_VAL){
          return true;
        }
        else{
          return false;
        }
      }*/
      void OverrideR::resetPulseTimer(){
        _pulseTimer = 0;
      }
      void OverrideR::followPulseCounter(){/*
        Serial.println(_pulseTimer);
        Serial.print("pulse ON : ");
        Serial.println(target.value()*60000);
        Serial.print("pulse OFF : ");
        Serial.println((unsigned long)target.value()*60000 + (unsigned long)pulseOff.value()*60000);
      */
        if(_pulseTimer <= (unsigned long)pulseOn.value()*60000){
          activate();
        }
        else if(_pulseTimer <= ((unsigned long)pulseOn.value()*60000 + (unsigned long)pulseOff.value()*60000)){
          desactivate();
        }
        else{
          _pulseTimer = 0;
        }
      }
      void OverrideR::followLockPulseCounter(){
        if(_pulseTimer <= (unsigned long)pulseOn.value()*60000){
          activate();
        }
        else if(_pulseTimer <= ((unsigned long)pulseOn.value()*60000 + (unsigned long)pulseOff.value()*60000)){
          _lockOff = true;
        }
        else{
          desactivate();
          _pulseTimer = 0;
          _lockOff = false;
        }
      }

      boolean  OverrideR::lockOff(){
        return _lockOff;
      }
      bool OverrideR::isActive(){
        return _active;
      }

      void OverrideR::activate(){
        _active = true;
        _lockOff = false;
      }


      void OverrideR::desactivate(){
        _active = false;
        _lockOff = false;
      }
