/**
 * SHT1x Library
 *
 * Copyright 2009 Jonathan Oxer <jon@oxer.com.au> / <www.practicalarduino.com>
 * Based on previous work by:
 *    Maurice Ribble: <www.glacialwanderer.com/hobbyrobotics/?p=5>
 *    Wayne ?: <ragingreality.blogspot.com/2008/01/ardunio-and-sht15.html>
 *
 * Manages communication with SHT1x series (SHT10, SHT11, SHT15)
 * temperature / humidity sensors from Sensirion (www.sensirion.com).
 */
#ifndef SHT1x_h
#define SHT1x_h

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include <elapsedMillis.h>

class SHT1x
{
  public:
    SHT1x(int dataPin, int clockPin);
    void initSensor();
    float readHumidity();
    float readTemperatureC();
    float readTemperatureF();
  private:
    elapsedMillis requestDelay;
    boolean waitingForTemp;
    boolean waitingForHumidity;
    int _rawTemp;
    int _rawHumidity;
    int _dataPin;
    int _clockPin;
    int _numBits;
    void readSensor();
    void readHumidityRaw();

    int shiftIn(int _dataPin, int _clockPin, int _numBits);
    void sendCommandSHT(int _command, int _dataPin, int _clockPin);
    void waitForResultSHT(int _dataPin);
    int getData16SHT(int _dataPin, int _clockPin);
    void skipCrcSHT(int _dataPin, int _clockPin);
};

#endif
