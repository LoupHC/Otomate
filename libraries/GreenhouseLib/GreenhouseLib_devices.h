
#include "Arduino.h"
#include "EEPROM.h"
#include "elapsedMillis.h"
#include "Parameters.h"
#include "Defines.h"
#include "GreenhouseLib_overrides.h"

#include <Wire.h>
#include "Adafruit_MCP23008.h"

extern  Adafruit_MCP23008 mcp;

#ifndef GreenhouseLib_devices_h
#define GreenhouseLib_devices_h

class Output{
  public:
    Output();
    ~Output();
    void init(byte relayType, byte pin);
    void init(byte outputType, byte relayType, byte pin);
    void stop();
    void start();
    byte pin();
    boolean isActive();
  private:
//Parameters
    void setRelayType(byte relayType);
    void setOutput(byte outputType);
    boolean _outputType;
    boolean _relayType;
    boolean _activate;
    boolean _desactivate;
    byte _pin;

    static boolean _MCP23008IsInitialized;
};

class Device : public OverrideManager{
  public:
    void desactivateDevice();
    void activateDevice();
    boolean isActivated();

  protected :
    boolParameter enabled;

};

struct Sequence{
  unsigned short onTime;
  unsigned short offTime;
};

class Alarm
{
  public :
    Alarm();
    ~Alarm();
    void init(byte outputType, boolean relayType, byte alarmPin);
    //Single alarm setup
    void above(float parameter, float value);
    void below(float parameter, float value);
    void offRange(float parameter, float minimum, float maximum);
    void conditionalTo(boolean value);
    //Multiple alarm setup
    void addSequence(byte number, unsigned short on, unsigned short off);
    void above(float parameter, float value, byte sequence);
    void below(float parameter, float value, byte sequence);
    void offRange(float parameter, float minimum, float maximum, byte sequence);
    void conditionalTo(boolean value, byte sequence);
    //Execute the alarm
    void start();
    void stop();
    void checkAlarm();
    boolean isActive();

  private :
    boolean _isActive;
    Output output;
    elapsedMillis counter;
    Sequence sequence[MAX_ALARM_SEQUENCES];
    byte _sequencePlaying;
};

#endif
