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
#include "EEPROM.h"
#include "elapsedMillis.h"
#include "Parameters.h"
#include "Defines.h"
#include "GreenhouseLib_devices.h"
#include "GreenhouseLib_sensors.h"

#ifndef GreenhouseLib_FF_h
#define GreenhouseLib_FF_h


class OnOffDevice  : public OverrideManager
{
  public:
      OnOffDevice();
      //initialization functions
      void initOutput(byte outputType, boolean relayType, byte pin);

      //Action function
      void lockOn();
      void lockOff();
      void lockOnAndWait(unsigned long seconds);
      void lockOffAndWait(unsigned long seconds);
      void forceStart();
      void forceStop();
      void unforce();
      boolean isLock();

      void routine(float target, float temp);
      void valvRoutine();

      //programmation functions
      void setParameters(byte type, float temp, float hyst, bool enabled);

			//return private variables
      floatParameter hyst;
      floatParameter mod;
      byteParameter type;
      byteParameter tensiometer;
      byteParameter analogSwitch;
      boolParameter enabled;
      boolParameter pulse;
      boolParameter lock;
      byteParameter lockTarget;
      uShortParameter maxOnTime;

			boolean isOn();
      boolean override();





      void EEPROMGet();
      unsigned short EEPROMIndexBegin();
      unsigned short EEPROMIndexEnd();
      unsigned short nb();

      void desactivateDevice();
      void activateDevice();
      boolean isActivated();
      void asignTensiometer(byte tensiometer);
      byte tensiometerIndex();


      void checkOverrideTimer();
      void resetLockTimer(unsigned long seconds);
      void keepLockInMemory(byte increment);

      void unlock();
      OnTime onTime;

    protected :


      elapsedMillis overrideTimer;
      bool lockedAndWaiting;
      unsigned long overrideWaitingTime;

      Output output;
      void heaterRoutine(float target, float temp);
      void fanRoutine(float target, float temp);

      byte _activeOverride;

      byte _tensiometerIndex;

      unsigned short _localCounter;
      static unsigned short _counter;
      unsigned short _localIndex;
      static unsigned short _EEPROMindex;

};

#endif
