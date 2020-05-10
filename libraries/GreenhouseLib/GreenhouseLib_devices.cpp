#include "GreenhouseLib_devices.h"

    boolean Output::_MCP23008IsInitialized = false;

    Output::Output(){}
    Output::~Output(){}

    void Output::init(byte relayType, byte pin){
      _pin = pin;
      //set action linked to digitalWrite state, according to relay type
      setRelayType(relayType);
     //initial state
      setOutput(IO);
     }

     void Output::init(byte outputType, byte relayType, byte pin){
      _pin = pin;
      //set action linked to digitalWrite state, according to relay type
      setRelayType(relayType);
     //initial state
      setOutput(outputType);
     }

    void Output::setRelayType(byte relayType){
      _relayType = relayType;
      if (_relayType == ACT_HIGH){
        _activate = HIGH;
        _desactivate = LOW;
      }
      else if (_relayType == ACT_LOW){
        _activate = LOW;
        _desactivate = HIGH;
      }
    }

    void Output::setOutput(byte outputType){
      _outputType = outputType;
      if(_outputType == IO){
         pinMode(_pin, OUTPUT);
         digitalWrite(_pin, _desactivate);
       }
       else if(_outputType == MCP23008){
         if(!_MCP23008IsInitialized){
           mcp.begin();
           _MCP23008IsInitialized = true;
         }
         mcp.pinMode(_pin, OUTPUT);
         mcp.digitalWrite(_pin, _desactivate);
       }
    }
    void Output::stop(){
    if(_outputType == IO){

      if(digitalRead(_pin) == _activate){
        digitalWrite(_pin, _desactivate);
      }
    }

    if(_outputType == MCP23008){

      if(mcp.digitalRead(_pin) == _activate){
        mcp.digitalWrite(_pin, _desactivate);
      }
    }
      #ifdef DEBUG_DEVICE
        Serial.println(F("-------------"));
        Serial.println(F("Stop device"));
        Serial.println(F("-------------"));
      #endif
    }

    void Output::start(){
      if(_outputType == IO){
        if(digitalRead(_pin) == _desactivate){
          digitalWrite(_pin, _activate);
        }
      }

      if(_outputType == MCP23008){

        if(mcp.digitalRead(_pin) == _desactivate){
          mcp.digitalWrite(_pin, _activate);
        }
      }
      #ifdef DEBUG_DEVICE
        Serial.println(F("-------------"));
        Serial.println(F("Start device"));
        Serial.println(F("-------------"));
      #endif
    }

    byte Output::pin(){
      return _pin;
    }

    boolean Output::isActive(){
      if(_outputType == IO){
        if(digitalRead(_pin) == _activate){
          return true;
        }
        else{
          return false;
        }
      }
      else if(_outputType == MCP23008){
        if(mcp.digitalRead(_pin) == _activate){
          return true;
        }
        else{
          return false;
        }
      }
      else{
        return false;
      }
    }

    //Create an alarm instance
    Alarm::Alarm(){

      for(int x = 0; x < MAX_ALARM_SEQUENCES;x++){
        sequence[x].onTime = 1000;
        sequence[x].offTime = 0;
      }
      _sequencePlaying = OFF_VAL;
      counter = 0;
    }
    Alarm::~Alarm(){}
    void Alarm::init(byte outputType, boolean relayType, byte alarmPin){
      output.init(outputType, relayType, alarmPin);
    }
    //create alarm sequence (# of seconds the alarm is on + # of secondes the alarm is off)
    //For constant alarm, define "offTime" as 0
    void Alarm::addSequence(byte number, unsigned short onTime, unsigned short offTime){
      if((number >= MAX_ALARM_SEQUENCES)||(number == 0)){
        return;
      }
      sequence[number].onTime = onTime;
      sequence[number].offTime = offTime;
    }
    //above a certain value, alarm will execute default sequence (constant alarm)
    void Alarm::above(float parameter, float value){
        if(parameter > value){
          //Serial.println("above");
          _sequencePlaying = 0;
        }
        else{
          if(_sequencePlaying == 0){
            _sequencePlaying = OFF_VAL;
          }
        }
    }
    //above a certain value, alarm will execute a specified sequence  (constant alarm)
    void Alarm::above(float parameter, float value, byte sequence){
        if(parameter > value){
          _sequencePlaying = sequence;
        }
        else{
          if(_sequencePlaying == sequence){
            _sequencePlaying = OFF_VAL;
          }
        }
      }
    //under a certain value, alarm will execute default sequence (constant alarm)
    void Alarm::below(float parameter, float value){
        if(parameter < value){
          //Serial.println("below");
          _sequencePlaying = 0;
        }
        else{
          if(_sequencePlaying == 0){
            _sequencePlaying = OFF_VAL;
          }
        }
    }
    //under a certain value, alarm will execute a specified sequence (constant alarm)
    void Alarm::below(float parameter, float value, byte sequence){
        if(parameter < value){
          _sequencePlaying = sequence;
        }
        else{
          if(_sequencePlaying == sequence){
            _sequencePlaying = OFF_VAL;
          }
        }
    }
    //under or above a range of value, alarm will execute default sequence (constant alarm)

    void Alarm::offRange(float parameter, float minimum, float maximum){
        if((parameter < minimum)||(parameter > maximum)){
          _sequencePlaying = 0;
        }
        else{
          if(_sequencePlaying == 0){
            _sequencePlaying = OFF_VAL;
          }
        }
    }
    //under or above a range of value, alarm will execute a specified sequence (constant alarm)
    void Alarm::offRange(float parameter, float minimum, float maximum, byte sequence){
        if((parameter < minimum)||(parameter > maximum)){
          _sequencePlaying = sequence;
        }
        else{
          if(_sequencePlaying == sequence){
            _sequencePlaying = OFF_VAL;
          }
        }
    }
    //if argument "value" is true, alarm will execute default sequence  (constant alarm)
    void Alarm::conditionalTo(boolean value){
        if(value == true){
          _sequencePlaying = 0;
        }
        else{
          if(_sequencePlaying == 0){
            _sequencePlaying = OFF_VAL;
          }
        }
    }
    //if argument "value" is true, alarm will execute a specified sequence
    void Alarm::conditionalTo(boolean value, byte sequence){
      if(sequence >= MAX_ALARM_SEQUENCES){
        return;
      }
      if(value == true){
          _sequencePlaying = sequence;
      }
      else{
        if(_sequencePlaying == sequence){
          _sequencePlaying = OFF_VAL;
        }
      }
    }

    //execute the selected sequence
    void Alarm::checkAlarm(){
      #ifdef DEBUG_ALARM
        Serial.print(F("counter : "));
        Serial.println(counter);
        if(_sequencePlaying != OFF_VAL){
          Serial.print(F("offtime : "));
          Serial.println(sequence[_sequencePlaying].offTime);
          Serial.print(F("ontime : "));
          Serial.println(sequence[_sequencePlaying].onTime);
        }
      #endif
      if(_sequencePlaying == OFF_VAL){
        output.stop();
        counter = 0;
        _isActive = false;
      }
      else if(sequence[_sequencePlaying].offTime == 0){
        output.start();
        counter = 0;
        _isActive = true;
      }
      else{
        if(counter < sequence[_sequencePlaying].onTime){
          output.start();
        }
        else if(counter < (sequence[_sequencePlaying].onTime+sequence[_sequencePlaying].offTime)){
          output.stop();
        }
        else{
          counter = 0;
        }
        _isActive = true;
      }
    }

    boolean Alarm::isActive(){
      return _isActive;
    }

    void Alarm::start(){
      output.start();
    }

    void Alarm::stop(){
      output.stop();
    }
