/*
  GreenhouseLib_rollups.cpp
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
#include "GreenhouseLib_rollups.h"

unsigned short Stage::_counter = 0;

Stage::Stage(){
  _localCounter = _counter;
  _counter++;
  if(_counter == MAX_STAGES){
    _counter = 0;
  }
}
Stage::~Stage(){}

unsigned short Stage::nb(){
  return _localCounter;
}
//****************************************************************
//*******************ROLLUP FUNCTIONS*****************************
//****************************************************************



/*
Constructor : Define Opening and Closing pins on the arduino, to connect to the relay box
_routine (normal logic for the rollup applies if true)
_closing (closing pin is active if true)
_opening (same with opening pin)
_closingCycle (a closing cycle is going on, including pause time)
_closingCycle (same with closing)
_incrementCounter (what increment the rollup has reached)
incrementCounter is set to OFF_VAL so the program recognize the first opening/closing run.
_stage (what cooling stage the rollup has reached)
_localIndex (starting point for EEPROM savings)
_index (starting point for next object of the same class)
EEPROMTimer (checks every 10 seconds if EEPROM has to be updated)

*/

unsigned short Rollup::_EEPROMindex = 0;
unsigned short Rollup::_counter = 0;

Rollup::Rollup(){
  _localCounter = _counter;
  _counter++;

  _activeOverride = false;
  _routineCycle = false;

//Addresses assignation

  _localIndex = ROLLUP_INDEX + _EEPROMindex;
  _EEPROMindex += ROLLUP_INDEX_SIZE;

  hyst.setAddress(_localIndex);
  _localIndex += sizeof(float);

  rotationUp.setAddress(_localIndex);
  _localIndex += sizeof(unsigned short);

  rotationDown.setAddress(_localIndex);
  _localIndex += sizeof(unsigned short);

  pause.setAddress(_localIndex);
  _localIndex += sizeof(unsigned short);
  _increments = 100;
  _test = false;

  for(int x = 0; x < MAX_STAGES; x++){
    stage[x].mod.setAddress(_localIndex);
    _localIndex += sizeof(float);
    stage[x].target.setAddress(_localIndex);
    _localIndex += sizeof(unsigned short);
  }

  stages.setAddress(_localIndex);
  _localIndex += sizeof(byte);
  enabled.setAddress(_localIndex);
  _localIndex += sizeof(boolean);

  lock.setAddress(_localIndex);
  _localIndex += sizeof(boolean);
  lockTarget.setAddress(_localIndex);
  _localIndex += sizeof(byte);
  currentLimit.setAddress(_localIndex);
  _localIndex += sizeof(float);

  #ifdef TEST_MODE
  _incrementCounter = 0;
  _stage = 0;
  #endif
  #ifndef TEST_MODE
  _incrementCounter = OFF_VAL;
  _stage = OFF_VAL;
  _reset = true;
  _calibrating = false;
  #endif

  initOverride(LOCK, 0,0,0,0);
  debugTimer = 0;
  rollupTimer = 0;
}

/*
Destructor
*/
Rollup::~Rollup(){}
/*
INIT OUTPUTS
- MAN_TEMP : target temperature is set outisde the class and differ for each cooling stage
relayType:
- ACT_HIGH : relay is active when pin is high
- ACT_LOW : relay is active when pin is low
rOpen (pin connected to opening relay)
rClose (pin connected to closing relay)
*/
void Rollup::initOutputs(byte outputType, boolean relayType, byte rOpen, byte rClose){
  //define opening/closing pins
  _openingPin.init(outputType, relayType, rOpen);
  _closingPin.init(outputType, relayType, rClose);
}

void Rollup::initCurrentPin(uint8_t currentPin){
  _currentPin = currentPin;
}

boolean Rollup::isLock(){
  if(isActive(LOCK)||isActive(EXT_LOCK)||_standby == true||_calibrating == true||_overcurrent == true){
    return true;
  }
  else{
    return false;
  }
}
void Rollup::unlock(){
/*
  if(_calibrating == true){
    forceStop();
    _calibrating = false;
    reset();
  }
  else{
    */
  if(_overcurrent == true){
    _overcurrent = false;
  }
  else if(_calibrating == false){
    checkOverride(LOCK, false);
    lockedAndWaiting = false;
    _activeOverride = false;
    lock.setValue(false);
    disable(LOCK);
  }
}

void Rollup::resetOverride(){

  if(_overcurrent == true){
    _overcurrent = false;
  }
  else if(_calibrating == false){
    checkOverride(LOCK, false);
    lockedAndWaiting = false;
    _activeOverride = false;
    disable(LOCK);
  }
}


void Rollup::resetLockTimer(unsigned long seconds){
  lockedAndWaiting = true;
  overrideWaitingTime = seconds;
  overrideTimer = 0;
}

void Rollup::checkOverrideTimer(){
  if(lockedAndWaiting == true){
    if(overrideTimer >= (unsigned long)overrideWaitingTime*1000){
      checkOverride(LOCK, false);
      lockedAndWaiting = false;
    }
  }
}
void Rollup::keepLockInMemory(byte increment){
  lock.setValue(true);
  lockTarget.setValue(increment);
}

void Rollup::lockOpenAndWait(unsigned long seconds){
  if(_calibrating == false){
    lockOpen();
    if(seconds != 0){
      resetLockTimer(seconds);
    }
    else{
      keepLockInMemory(_increments);
    }
  }
}

void Rollup::lockCloseAndWait(unsigned long seconds){
  if(_calibrating == false){
    lockClose();
    if(seconds != 0){
      resetLockTimer(seconds);
    }
    else{
      keepLockInMemory(0);
    }
  }
}

void Rollup::lockOpen(){
  if(_calibrating == false){
    forceStop();
    resetOverride();
    enable(LOCK);
    setIncrementCounter(0);
    _standby = true;
    setOverrideTarget(LOCK, 100);
    setPulseOnTime(LOCK, 1);
    setPulseOffTime(LOCK, 0);
    checkOverride(LOCK, true);
  }
}

void Rollup::lockClose(){
  if(_calibrating == false){
    forceStop();
    resetOverride();
    enable(LOCK);
    setIncrementCounter(100);
    _standby = true;
    setOverrideTarget(LOCK, 0);
    setPulseOnTime(LOCK, 1);
    setPulseOffTime(LOCK, 0);
    checkOverride(LOCK, true);
  }
}


void Rollup::lockAtIncrement(byte increment){
  if(_calibrating == false){
    if(increment == 0){
      lockClose();
    }
    if(increment == 100){
      lockOpen();
    }
    if(increment <= _increments){
      enable(LOCK);
      setOverrideTarget(LOCK, increment);
      setPulseOnTime(LOCK, 0);
      setPulseOffTime(LOCK, 0);
      checkOverride(LOCK, true);
    }
  }
}

void Rollup::unforce(){
  checkOverride(EXT_LOCK, false);
}

void Rollup::forceAt(byte increment){
  if(_calibrating == false){
    if(increment == 0){
      forceStop();
      unlock();
      _standby = true;
      setIncrementCounter(100);
    }
    if(increment == 100){
      forceStop();
      unlock();
      _standby = true;
      setIncrementCounter(0);
    }
    initOverride(EXT_LOCK, 0, increment, 0,0);
    checkOverride(EXT_LOCK, true);
  }
}

void Rollup::lockAtIncrement(byte increment, unsigned long seconds){
  if(_calibrating == false){
    lockAtIncrement(increment);

    if(seconds != 0){
      resetLockTimer(seconds);
    }
    else{
      keepLockInMemory(increment);
    }
  }
}

/*
Open or close the rollups to specific increment, using a multiple cooling stages logic
*/
void Rollup::forceStop(){
  stopMove();
  _incrementCounter = OFF_VAL;
  _stage = OFF_VAL;
  _incrementMove = 0;
}

void Rollup::reset(){
  _activeOverride = false;
  _calibrating = false;
  _reset = true;
}


float Rollup::current(){
  return _current;
}

void Rollup::getCurrent(){
  if(_currentPin != OFF_VAL){
    float currentRead = (float)10/(float)1024*analogRead(_currentPin)-5;
    if(currentRead < 0.10 && currentRead > -0.10){currentRead = 0;}
    motor.registerValue(currentRead);
    _current = abs(motor.average());
  }
}

void Rollup::rotationTimeCalibration(){
    //force close
    if(_calibratingStep == 1){
      forceStop();
      _closingPin.start();
      rollupTimer = 0;
      _calibratingStep = 2;
      Serial.println(F("first closing"));
    }
    //start opening from the bottom limit switch
    if((_calibratingStep == 2)&&(rollupTimer > 5000)&&(_current < 0.1)){
      _closingPin.stop();
        _openingPin.start();
        rollupTimer = 0;
        _calibratingStep = 3;
        Serial.println(F("start opening"));
    }
    //start closing from the top limit switch
    if((_calibratingStep == 3)&&(rollupTimer > 5000)&&(_current < 0.1)){
      _openingPin.stop();
      rotationUp.setValue(rollupTimer/1000);
        _closingPin.start();
        rollupTimer = 0;
        _calibratingStep = 4;

        Serial.println(F("stop opening"));
        Serial.print(F("rotation up : "));
        Serial.println(rotationUp.value());
        Serial.println(F("start closing"));
    }
    //resume when reach the bottom limit switch again
    if((_calibratingStep == 4)&&(rollupTimer > 5000)&&(_current < 0.1)){
      _closingPin.stop();
      rotationDown.setValue(rollupTimer/1000);
      rollupTimer = 0;
      _calibratingStep = 5;

      Serial.println(F("stop closing"));
      Serial.print(F("rotation down : "));
      Serial.println(rotationDown.value());
    }
    if(_calibratingStep == 5){
      setIncrementCounter(0);
      _calibrating = false;
      _reset = false;
    }
  }

void Rollup::autoCalibration(){
  if(_currentPin != OFF_VAL){
    _calibrating = true;
    _calibratingStep = 1;
  }
}

void Rollup::routine(float targetTemp, float temp){
  if (isActivated() && _overcurrent == false){
    _standby = false;
    if(_calibrating == true){
      _reset = false;
      rotationTimeCalibration();
    }
    else{
        //full rotation down cycle at reset
        if(_reset == true){

          if(lock.value() == true && lockTarget.value() == 0){
            lockCloseAndWait(0);
          }
          else{
            lockCloseAndWait(rotationDown.value());
          }
          _reset = false;
        }
        else if(lock.value() == true && !isLock()){
          lockAtIncrement(lockTarget.value());
        }
        autoAdjustStages();
        checkOverrideTimer();

        if((_activeOverride == false)&&(activeOverride() == true)&&(!isMoving())){

            updatePosition(overrideTarget());
            startMove();
        }
        else if(_activeOverride == true){
          watchOverride();
        }
        else{
          if(_routineCycle == false && !isMoving()){
              updatePosition(temp, targetTemp);
              startMove();
          }
          else{
            watchRoutine();
          }

        }
      }
      if(_currentPin != OFF_VAL){
        getCurrent();
        checkOvercurrent();
      }
    }
  debugPrints();
}

void Rollup::checkOvercurrent(){
  if(_current >= currentLimit.value() && currentLimit.value() != 0){
    forceStop();
    reset();
    _overcurrent = true;
  }
}



/*ROUTINE MOVING LOGIC
    INITIAL CONDITIONS:
    -increment counter is calibrated
    - theres a difference between actual increment and target increment
    ACTION:
    - If temp is higher than treshold : go to next stage up
    - If temp is lower than treshold : go to next stage down
    - calculate time of the move
    - start moving
    - set moving timer to 0
*/
void Rollup::updatePosition(float temp, float targetTemp){
  short targetIncrement;

  if (temp >= (targetTemp + stage[_upperStage].mod.value())){
      targetIncrement = stage[_upperStage].target.value();
      _incrementMove = targetIncrement - (short)_incrementCounter;
  }
  else if(temp < (targetTemp + stage[_stage].mod.value() - hyst.value())){
      targetIncrement = stage[_lowerStage].target.value();
      _incrementMove = targetIncrement - (short)_incrementCounter;
  }
  else{
    _incrementMove = 0;
  }

  if(_incrementMove != 0){
    _routineCycle = true;
  }
}
unsigned short Rollup::routinePosition(float temp, float targetTemp){
  short targetIncrement;

  if (temp >= (targetTemp + stage[_upperStage].mod.value())){
      targetIncrement = stage[_upperStage].target.value();
  }
  else if(temp < (targetTemp + stage[_stage].mod.value() - hyst.value())){
      targetIncrement = stage[_lowerStage].target.value();
  }
  else{
    targetIncrement = stage[_stage].target.value();
  }
  return targetIncrement;
}

void Rollup::updatePosition(byte target){
  if(_routineCycle == true){
    _routineCycle = false;
  }
  _activeOverride = activeOverride();
  _incrementMove = target - (short)_incrementCounter;
}

void Rollup::startMove(){
  calibrateStages();
  updateTimings();
    //If motor goes up, calculate for how much time
  if (_incrementMove > 0){
    _moveTime = _upStep * (unsigned long)(abs(_incrementMove));
    _openingPin.start();
  }
    //If motor goes down, calculate for how much time
  else if (_incrementMove < 0){
    _moveTime = _downStep * (unsigned long)(abs(_incrementMove));
    _closingPin.start();
  }
  rollupTimer = 0;
}


  /*ROUTINE WATCH
  MOVE : until motor reaches target increment
  PAUSE : until end of pause time
  */
  void Rollup::watchRoutine(){
    if((rollupTimer > _moveTime)){
      stopMove();
      updatePosition();
    }
    if(rollupTimer > (_moveTime + _pauseTime)){
      resumeCycle("ROUTINE");
    }
  }


/*OVERRIDE WATCH
MOVE : until motor reaches target increment
PAUSE :for all override's duration
*/
void Rollup::watchOverride(){
  if(rollupTimer > _moveTime){
      stopMove();
      updatePosition();
  }
  if((rollupTimer > _moveTime)&&(activeOverride() == true)&&(overrideTarget() != incrementCounter())){
      updatePosition(overrideTarget());
      startMove();
  }
  if((rollupTimer > _moveTime)&&(activeOverride() == false)){
      resumeCycle("OVERRIDE");
  }
}
/*STOP MOVING
    INITIAL CONDITION:
    -increment counter is calibrated
    ACTION:
    - stop moving
    - set new value for incrementCounter
    - reset _incrementMove
*/

void Rollup::updatePosition(){
  _incrementCounter = _incrementCounter + _incrementMove;
  _incrementMove = 0;
  calibrateStages();
}
void Rollup::stopMove(){
  if(_openingPin.isActive()){
    _openingPin.stop();
  }
  if(_closingPin.isActive()){
    _closingPin.stop();
  }
}
/*RESUME CYCLE
    INITIAL CONDITION:
    -override is active
    OR
    - routine cycle is active
    ACTION:

    ACTION:
    - desactivate override and activate routine
    OR
    - desactivate routine cycle
    - reset times, moving time and stage move
*/
void Rollup::resumeCycle(String type){
  if(type == "OVERRIDE"){
      _activeOverride = false;
  }
  else if(type == "ROUTINE"){
      _routineCycle = false;
      printEndPause();
  }
  _moveTime = 0;
  rollupTimer = 0;
}

/*CALIBRATE STAGE
    INITIAL CONDITION:
    - stage is uncalibrated
    ACTION:
    - if incrementCounter is uncalibrateddesactivate routine cycle
    - reset times, moving time and stage move
*/
void Rollup::calibrateStages(){
        //If youre at top increment, consider youre at top stage
        if(_incrementCounter >= stage[stages.value()].target.value()){
          _stage = stages.value();
        }
        //If youre at bottom increment, consider youre at lowest stage
        else if(_incrementCounter == stage[0].target.value()){
          _stage = 0;
        }
        //Anywhere between first and second stage increment, consider youre in the second stage
        else if((_incrementCounter > stage[0].target.value())&&(_incrementCounter <= stage[1].target.value())){
          _stage = 1;
        }
        //Between second stage and third, consider youre at second stage;
        //Between third stage and fourth, considier youre at third stage;
        //etc.
        else{
          for(byte x = 1; x < stages.value();x++){
            if((_incrementCounter >= stage[x].target.value())&& (_incrementCounter < stage[x+1].target.value())){
              _stage = x;
            }
          }
        }
        checkStageSuccession();
  }

  void Rollup::setTest(boolean state){
    _test = state;
  }

  void Rollup::updateTimings(){
    float upStep = (float)rotationUp.value()*1000/(float)_increments;
    float downStep = (float)rotationDown.value()*1000/(float)_increments;
    _upStep = upStep;
    _downStep = downStep;
    _pauseTime = pause.value()*1000;

      if(_test == true){
        _upStep = 0;
        _downStep = 0;
        _pauseTime = 0;
      }
  }

void Rollup::autoAdjustStages(){
  for(byte x = 1; x <= stages.value();x++){
    if(stage[x].mod.value() < stage[x-1].mod.value()){
      stage[x].mod.setValue(stage[x-1].mod.value());
    }
    if(stage[x].target.value() < stage[x-1].target.value()){
      stage[x].target.setValue(stage[x-1].target.value());
    }

    if(stage[x].mod.value() > MOD_MAXIMUM){
      stage[x].mod.setValue(MOD_MAXIMUM);
    }
    if(stage[x].target.value() > STAGE_TARGET_MAXIMUM){
      stage[x].target.setValue(STAGE_TARGET_MAXIMUM);
    }
  }
}

  void Rollup::checkStageSuccession(){
    if(_stage != stages.value()){
      if(stage[_stage].target.value() != stage[_stage+1].target.value()){
        _upperStage = _stage+1;
      }
      else if(stage[_stage].target.value() != stage[_stage+2].target.value()){
        _upperStage = _stage+2;
      }
      else if(stage[_stage].target.value() != stage[_stage+3].target.value()){
        _upperStage = _stage+3;
      }
      else if(stage[_stage].target.value() != stage[_stage+4].target.value()){
        _upperStage = _stage+4;
      }
      else{
        _upperStage = stages.value();
      }
    }
    else{
      _upperStage = stages.value();
    }
    if(_stage != 0){
      if(stage[_stage].target.value() != stage[_stage-1].target.value()){
        _lowerStage = _stage-1;
      }
      else if(stage[_stage].target.value() != stage[_stage-2].target.value()){
        _lowerStage = _stage-2;
      }
      else if(stage[_stage].target.value() != stage[_stage-3].target.value()){
        _lowerStage = _stage-3;
      }
      else if(stage[_stage].target.value() != stage[_stage-4].target.value()){
        _lowerStage = _stage-4;
      }
      else{
        _lowerStage = 0;
      }
    }
    else{
      _lowerStage = 0;
    }
  }
  void Rollup::desactivateDevice(){
    forceStop();
    reset();
    enabled.setValue(false);
  }
  void Rollup::activateDevice(){
    enabled.setValue(true);
    _overcurrent = false;

  }
  boolean Rollup::overCurrentWarning(){
    if(_overcurrent == true){
      return true;
    }
    else{
      return false;
    }
  }

  bool Rollup::isActivated(){
    return enabled.value();
  }


  boolean Rollup::isStandby(){
    if(incrementCounter() == OFF_VAL ||_calibrating == true || _standby == true){
      return true;
    }
    else{
      return false;
    }
  }

void Rollup::printPause(){
  #ifdef DEBUG_ROLLUP1_STATE
  if(_localCounter == 0){
    Serial.println(F("-------------"));
    Serial.println(F("Rollup1 : start pause"));
  }
  #endif
  #ifdef DEBUG_ROLLUP2_STATE
  if(_localCounter == 1){
    Serial.println(F("-------------"));
    Serial.println(F("Rollup2 : start pause"));
  }
  #endif
}

void Rollup::printEndPause(){

  #ifdef DEBUG_ROLLUP1_STATE
  if(_localCounter == 0){
    Serial.println(F("-------------"));
    Serial.println(F("Rollup1 : stop pause"));
  }
  #endif
  #ifdef DEBUG_ROLLUP2_STATE
  if(_localCounter == 1){
    Serial.println(F("-------------"));
    Serial.println(F("Rollup2 : stop pause"));
  }
  #endif
}
/*
Program all parameters all at once...
*/
void Rollup::setParameters(byte st, float rHyst, unsigned short rotUp, unsigned short rotDown, unsigned short paus, bool enab){
  stages.setValue(st);
  hyst.setValue(rHyst);
  rotationUp.setValue(rotUp);
  rotationDown.setValue(rotDown);
  pause.setValue(paus);
  enabled.setValue(enab);
}


void Rollup::setIncrementCounter(unsigned short increment){
  _incrementCounter = increment;
}

void Rollup::EEPROMGet(){
/*
  hyst.getInEEPROM();
  rotationUp.getInEEPROM();
  rotationDown.getInEEPROM();
  pause.getInEEPROM();
*/
  #ifdef DEBUG_EEPROM
    Serial.println(F("-------------------"));
    Serial.print(F("-------ROLLUP "));
    Serial.print(_localCounter);
    Serial.println(F("------"));
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
    Serial.print(rotationUp.address());
    Serial.print(F(" - Value :"));
    Serial.print(rotationUp.value());
    Serial.println(F("   - (Rotation up)"));
    Serial.print(F("Address: "));
    Serial.print(rotationDown.address());
    Serial.print(F(" - Value :"));
    Serial.print(rotationDown.value());
    Serial.println(F("   - (Rotation down)"));
    Serial.print(F("Address: "));
    Serial.print(pause.address());
    Serial.print(F(" - Value :"));
    Serial.print(pause.value());
    Serial.println(F("   - (Pause)"));
  #endif

  for(unsigned short x = 0; x < stages.value()+1; x++){/*
    stage[x].mod.getInEEPROM();
    stage[x].target.getInEEPROM();*/

    #ifdef DEBUG_EEPROM
      Serial.print(F("-------STAGE  "));
      Serial.print(stage[x].nb());
      Serial.println(F("------"));
      Serial.print(F("Address: "));
      Serial.print(stage[x].mod.address());
      Serial.print(F(" - Value :"));
      Serial.print(stage[x].mod.value());
      Serial.print(F(" - (Stage mod "));
      Serial.print(x);
      Serial.println(F(")"));
      Serial.print(F("Address: "));
      Serial.print(stage[x].target.address());
      Serial.print(F(" - Value :"));
      Serial.print(stage[x].target.value());
      Serial.print(F("   - (Stage target "));
      Serial.print(x);
      Serial.println(F(")"));
    #endif
  }
}
//Return private variables

unsigned short Rollup::increments(){
  return _increments;
}

unsigned short Rollup::incrementCounter(){
  return _incrementCounter;
}
boolean Rollup::opening(){
  return _openingPin.isActive();
}
boolean Rollup::closing(){
  return _closingPin.isActive();
}
boolean Rollup::override(){
  if(_activeOverride == true){
    return true;
  }
  else{
    return false;
  }
}
boolean Rollup::isMoving(){
  if ((_openingPin.isActive())||(_closingPin.isActive())){
    return true;
  }
  else{
    return false;
  }
}
boolean Rollup::isWaiting(){
    if (((!_openingPin.isActive())||(!_closingPin.isActive()))&&(_routineCycle == true)){
      return true;
    }
    else{
      return false;
    }
}

unsigned short Rollup::nb(){
  return _localCounter;
}

bool Rollup::inrushPhase(){
  if((rollupTimer > 0 && rollupTimer < 1500)&&(isMoving())){
    return true;
  }
  else{
    return false;
  }
}
unsigned short Rollup::EEPROMIndexBegin(){
  return ROLLUP_INDEX + (ROLLUP_INDEX_SIZE*_localCounter);
}

unsigned short Rollup::EEPROMIndexEnd(){
  return _localIndex;
}


void Rollup::debugPrints(){
  if(debugTimer > 1000){
    if(_localCounter == 0 ){
      #ifdef DEBUG_ROLLUP1_CYCLE
        Serial.println(F("---------"));
        if(_activeOverride == true){
          Serial.println(F("OVERRIDE : ON"));
        }
        if(_routineCycle == true){
          Serial.println(F("ROUTINE CYCLE : ON"));
        }
      #endif
      #ifdef DEBUG_ROLLUP1_POSITION
          Serial.println(F("---------"));
          Serial.print(F("up stage : "));
          Serial.println(_upperStage);
          Serial.print(F("up target : "));
          Serial.println(stage[_upperStage].target.value());
          Serial.print(F("Stage : "));
          Serial.println(_stage);
          Serial.print(F("low stage : "));
          Serial.println(_lowerStage);
          Serial.print(F("low target : "));
          Serial.println(stage[_lowerStage].target.value());
      #endif
      #ifdef DEBUG_ROLLUP1_TIMING
        Serial.println(F("---------"));
        if(_routineCycle == true){
          Serial.println(F("ROUTINE1"));
          Serial.print(F("rollupTimer : "));
          Serial.println(rollupTimer);
          Serial.print(F("moveTime : "));
          Serial.println(_moveTime);
          Serial.print(F("pauseTime : "));
          Serial.println(_moveTime + _pauseTime);
        }
        else if(_activeOverride == true){
          Serial.print(F("OVERRIDE1"));
          Serial.println(_activeOverride);
          Serial.print(F("rollupTimer : "));
          Serial.println(rollupTimer);
          Serial.print(F("moveTime : "));
          Serial.println(_moveTime);
        }
      #endif
    }
  if(_localCounter == 1 ){
    #ifdef DEBUG_ROLLUP2_CYCLE
      Serial.println(F("---------"));
      if(_activeOverride == true){
        Serial.println(F("OVERRIDE : ON"));
      }
      if(_routineCycle == true){
        Serial.println(F("ROUTINE CYCLE : ON"));
      }
    #endif

    #ifdef DEBUG_ROLLUP2_POSITION
      Serial.println(F("---------"));
      Serial.print(F("up stage : "));
      Serial.println(_upperStage);
      Serial.print(F("up target : "));
      Serial.println(stage[_upperStage].target.value());
      Serial.print(F("Stage : "));
      Serial.println(_stage);
      Serial.print(F("low stage : "));
      Serial.println(_lowerStage);
      Serial.print(F("low target : "));
      Serial.println(stage[_lowerStage].target.value());
    #endif
    #ifdef DEBUG_ROLLUP2_TIMING
      Serial.println(F("---------"));
      if(_routineCycle == true){
        Serial.println(F("ROUTINE2"));
        Serial.print(F("rollupTimer : "));
        Serial.println(rollupTimer);
        Serial.print(F("moveTime : "));
        Serial.println(_moveTime);
        Serial.print(F("pauseTime : "));
        Serial.println(_moveTime + _pauseTime);
      }
      else if(_activeOverride == true){
        Serial.println(F("OVERRIDE2"));
        Serial.print(F("rollupTimer : "));
        Serial.println(rollupTimer);
        Serial.print(F("moveTime : "));
        Serial.println(_moveTime);
      }
    #endif
    }
    if(_localCounter == 2 ){
      #ifdef DEBUG_ROLLUP3_CYCLE
        Serial.println(F("---------"));
        if(_activeOverride == true){
          Serial.println(F("OVERRIDE : ON"));
        }
        if(_routineCycle == true){
          Serial.println(F("ROUTINE CYCLE : ON"));
        }
      #endif

      #ifdef DEBUG_ROLLUP3_POSITION
        Serial.println(F("---------"));
        Serial.print(F("up stage : "));
        Serial.println(_upperStage);
        Serial.print(F("up target : "));
        Serial.println(stage[_upperStage].target.value());
        Serial.print(F("Stage : "));
        Serial.println(_stage);
        Serial.print(F("low stage : "));
        Serial.println(_lowerStage);
        Serial.print(F("low target : "));
        Serial.println(stage[_lowerStage].target.value());
      #endif
      #ifdef DEBUG_ROLLUP3_TIMING
        Serial.println(F("---------"));
        if(_routineCycle == true){
          Serial.println(F("ROUTINE3"));
          Serial.print(F("rollupTimer : "));
          Serial.println(rollupTimer);
          Serial.print(F("moveTime : "));
          Serial.println(_moveTime);
          Serial.print(F("pauseTime : "));
          Serial.println(_moveTime + _pauseTime);
        }
        else if(_activeOverride == true){
          Serial.println(F("OVERRIDE3"));
          Serial.print(F("rollupTimer : "));
          Serial.println(rollupTimer);
          Serial.print(F("moveTime : "));
          Serial.println(_moveTime);
        }
      #endif
      }
    debugTimer = 0;
  }
}
