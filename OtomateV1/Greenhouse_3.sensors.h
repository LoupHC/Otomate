


#define TEST_CLOCKF         201//RTC
#define TEST_CLOCKFF        202//RTC
#define TEST_TEMP           105
#define TEST_OUTTEMP        106
#define TEST_RADIATION      107
#define TEST_TENSIOM1       108


//#define DEBUG_CLOCK
  #include <OneWire.h>
  #include <DallasTemperature.h>
  #include <DS3231.h>
  #include <SHT1x.h>
  #include <GreenhouseLib_sensors.h>
  //Create DS18B20 objects
  OneWire oneWire1(TEMP1_CLOCK);
  DallasTemperature ds18b20_in(&oneWire1);
  OneWire oneWire2(TEMP2_CLOCK);
  DallasTemperature ds18b20_out(&oneWire2);
  //Create SHT1X objects
  SHT1x sht1x_in(TEMP1_DATA, TEMP1_CLOCK);
  SHT1x sht1x_out(TEMP2_DATA, TEMP2_CLOCK);
  //Create a RTC object
  DS3231  rtc(SDA, SCL);                // Init the DS3231 using the hardware interface
  Time  t;

  byte clockTest = 0;
  byte sensorTest = 0;

  //Time array
  byte rightNow[6];
  //sensors

  Temperature greenhouseTemperature;
  float temperatureBackup;
  Temperature outsideTemperature;
  Humidity greenhouseHumidity;
  Humidity outsideHumidity;
  Sensor soilMoisture[MAX_TENSIOMETERS+1];

  Current motorOne;
  Current motorTwo;

  Radiation radiation;

  boolean rain = false;
  float rainfall24h = 0;
  volatile int interruptWindSpeed = 0;
  unsigned short windSpeed = 0;


//rain bucket
const float Bucket_Size = 0.0254; // bucket size to trigger tip count

volatile unsigned long tipCount = 0; // bucket tip counter used in interrupt routine
volatile unsigned long contactTime; // Timer to manage any contact bounce in interrupt routine
volatile float totalRainfall = 0; // total amount of rainfall detected
float rainSample = 0;
float rainSetpoint = 0.240;
unsigned long rainSampleDelay = 120000;
elapsedMillis rainSampleCounter;
byte resetBucketDay;
byte resetBucketMonth;


boolean sensorFailure = false;
boolean outSensorFailure = false;
boolean tensiomFailure = false;

unsigned long counter = 1;

int startMin = 0;
int startHour = 0;
int startDay = 1;
int startMonth = 1;
int startYear = 0;

void setRollupsInTestMode(){
  if(clockTest != 0){
      greenhouse.testRollups(true);
  }
  else{
    greenhouse.testRollups(false);
  }
}
void sensorBackup(){
  if(EEPROM.read(1) == 111){
    float emergencyTemp = EEPROM.read(2);
    greenhouseTemperature.registerValue(emergencyTemp);
  }
  else{
    greenhouseTemperature.registerValue(20);
  }
  if(EEPROM.read(3) == 111){
    float emergencyHum = EEPROM.read(4);
    greenhouseHumidity.registerValue(emergencyHum);
  }
  else{
    greenhouseHumidity.registerValue(50);
  }
}
// Interrupt handler routine that is triggered when the rg-11 detects rain
void isr_rg() {
  if((millis() - contactTime) > 15 ) { // debounce of sensor signal
  tipCount++;
  totalRainfall = tipCount * Bucket_Size;
  contactTime = millis();
  }
}
boolean samplingRain = false;

void getRain(){
  //Normally open contact rain sensor
  if(greenhouse.rainSensor.value() == NO_CONTACT){
    if(digitalRead(RAIN_SWITCH) == LOW){
      rain = true;
    }
    else{
      rain = false;
    }
  }
  //Normally close contact rain sensor
  else if(greenhouse.rainSensor.value() == NC_CONTACT){
    if(digitalRead(RAIN_SWITCH) == HIGH){
      rain = true;
    }
    else{
      rain = false;
    }
  }
  //RG11/Sparkfun - 0.01'' bucket rain sensor
  else if(greenhouse.rainSensor.value() == RG11_BUCKET){
    //if precipitations have increased, start sampling data
    if(totalRainfall > rainSample && samplingRain == false){
      rainSampleCounter = 0;
      samplingRain = true;
    }
    //if sampling is going on
    if(samplingRain == true){
      //if counter is below its limit
      if(rainSampleCounter < rainSampleDelay){
        //if rain has cross setpoint, it's raining, reset counter
        if(totalRainfall >= rainSample + (float)greenhouse.rainSetpoint.value()/(float)30){
          rain = true;
          rainSample = totalRainfall;
          rainSampleCounter = 0;
        }
      }
      //if counter has reached its limit
      else{
        //if rain hasnt cross setpoint, it's not raining, stop sampling
        if(totalRainfall < rainSample + (float)greenhouse.rainSetpoint.value()/(float)30){
          rain = false;
          samplingRain = false;
        }
        //reset counter
        rainSample = totalRainfall;
        rainSampleCounter = 0;
      }
    }
  }
  else{
    rain = false;
    totalRainfall = false;
  }
}

#define switch_delay_to_wind_speed 2.4 // 2.4km/h = 1 trigger/second
volatile unsigned long contactTimeAne; // Timer to manage any contact bounce in interrupt routine for anemometer
elapsedMillis AneSampleCounter; //Anemometer switch counter; elapsed time in millisecond since last trigger

// Interrupt handler routine that is triggered when the anemometer make one rotation
void isr_ane() {
  if((millis() - contactTimeAne) > 15 ) { // debounce of sensor signal
    interruptWindSpeed = switch_delay_to_wind_speed*1000/AneSampleCounter;
  if(interruptWindSpeed < 1){
    interruptWindSpeed = 0; //Avoid low speed when no wind, so if windspeed lower than 1km/h than reset to 0km/h
  }
  AneSampleCounter = 0;
  contactTimeAne = millis();
  }
}

void getWind(){
  if(greenhouse.anemometer.value() == ANALOG_WIND){
    //0-5V (0-30 m/s) analog wind sensor
    int aRead = analogRead(WIND_ANALOG_SENSOR);
    float calculation = (float)108/(float)1023*(float)aRead; //108km/h = 30m/s
    windSpeed = calculation;
  }
  else if(greenhouse.anemometer.value() == SPARKFUN_WIND){
    windSpeed = (unsigned short)interruptWindSpeed;
  }
  else{
    windSpeed = 0;
  }

}



void testTime(){

    if(startMin > 59){
      startMin = 0;
      startHour++;
    }
    if(startHour > 23){
      startHour = 0;
    }
    if(startDay > 30){
      startDay = 1;
      startMonth++;
    }
    if(startMonth > 12){
      startMonth = 1;
      clockTest = 0;
    }
}

void getDateAndTime(){
  setRollupsInTestMode();
  if(clockTest == TEST_CLOCKF){
    t = rtc.getTime();

    rightNow[0] = 0;
    rightNow[1] = startMin;
    rightNow[2] = startHour;
    rightNow[3] = t.date;
    rightNow[4] = t.mon;
    rightNow[5] = t.year-2000;

    startMin++;
    testTime();

  }
  else if(clockTest == TEST_CLOCKFF){

    rightNow[0] = 0;
    rightNow[1] = startMin;
    rightNow[2] = startHour;
    rightNow[3] = startDay;
    rightNow[4] = startMonth;
    rightNow[5] = startYear;

    startDay++;
    testTime();

  }
  else{
      startMin = 0;
      startHour = 0;
      t = rtc.getTime();
      rightNow[5] = t.year-2000;
      rightNow[4] = t.mon;
      rightNow[3] = t.date;
      rightNow[2] = t.hour;
      rightNow[1] = t.min;
      rightNow[0] = t.sec;
  }


  #ifdef DEBUG_CLOCK
  for(int x = 0; x < 6; x++){
    Serial.print(rightNow[x]);
    Serial.print(":");
  }
  Serial.println("");
  #endif
}

boolean checkSensorFailure(float parameter,float measuredValue, float minValue, float maxValue, int recoveryAddress){
      if((measuredValue <= minValue)||(measuredValue >= maxValue)){
        byte lastRecordedValue;
        if(parameter < 0){
          lastRecordedValue = 0;
        }
        else{
          lastRecordedValue = parameter;
        }
        //Serial.println(F("failure!"));
        //Serial.println(measuredValue);
        return true;
      }
      else{
        return false;
      }
}


boolean droppingTest = false;
boolean risingTest = false;
float startTempTest = 10.0;
float startHumTest = 80.0;
float startTensiom = 15.0;
unsigned short startRadiation = 400;
float mockedGreenhouseTemp = 10.0;
float mockedGreenhouseHum = 80.0;
float mockedOutsideHum = 80.0;
float mockedOutsideTemp = 10.0;
float mockedTensiom1 = 15.0;
unsigned short mockedRadiation = 400;
byte sensorVariable = 0;


void testTemp(){
  greenhouseTemperature.setValue(mockedGreenhouseTemp);
}
void testHum(){
  greenhouseHumidity.setValue(mockedGreenhouseHum);
}
void testOutTemp(){
  outsideTemperature.setValue(mockedOutsideTemp);
}

void testOutHum(){
  outsideHumidity.setValue(mockedOutsideHum);
}
void testRadiation(){
  radiation.setValue(mockedRadiation);
}

void testTensiom1(){
  soilMoisture[0].setValue(mockedTensiom1);
}
unsigned long tempFailTest = 0;
byte humFailTest = 0;


void getGreenhouseTemp(){
/*//test T24h
  for (int x = 0; x < 24; x++){
    if(greenhouseTemperature.hourAverage(x) != OFF_VAL){
      Serial.print(F("["));
      Serial.print(greenhouseTemperature.hourAverage(x));
      Serial.print(F("]"));
    }
  }*/
  setRollupsInTestMode();
  if(sensorTest == TEST_TEMP){
    testTemp();
  }
  else{
    float temp;

    if(greenhouse.insideTempSensor.value() == DS18B20_TEMP){
      ds18b20_in.requestTemperatures();
      temp = ds18b20_in.getTempCByIndex(0);

      if(checkSensorFailure(greenhouseTemperature.value(), temp, -127.00,60.00,1)){
        if(tempFailTest < 20){
          tempFailTest++;
        }
      }
      else{
        tempFailTest = 0;
      }

    }
    else if(greenhouse.insideTempSensor.value() == SHT1X_TEMP){
      temp = sht1x_in.readTemperatureC();
      if(checkSensorFailure(greenhouseTemperature.value(), temp, -40.00,60.00,1)){
        if(tempFailTest < 20){
          tempFailTest++;
        }
      }
      else{
        tempFailTest = 0;
      }
    }
    if(tempFailTest == 0){
      greenhouseTemperature.registerValue(temp);
      sensorFailure = false;
    }
    else if (tempFailTest == 20){
      if((temperatureBackup > -40.00)&&(temperatureBackup < 75.00)){
        greenhouseTemperature.registerValue(temperatureBackup);
      }
      else{
        EEPROM.update(1, 111);
        EEPROM.update(2, greenhouseTemperature.value());
        greenhouseTemperature.registerValue(EEPROM.read(2));
      }
      sensorFailure = true;
    }
  }
}



void getGreenhouseHum(){
  setRollupsInTestMode();

  if(sensorTest == TEST_TEMP){
    testHum();
  }
  else{

    float hum;
    if(greenhouse.insideTempSensor.value() == SHT1X_TEMP){
      hum = sht1x_in.readHumidity();

      if(checkSensorFailure(greenhouseHumidity.value(), hum, -1.00,101.00,2)){
        if(humFailTest < 20){
          humFailTest++;
        }
      }
      else{
        humFailTest = 0;
      }
    }

    if(humFailTest == 0){
      greenhouseHumidity.registerValue(hum);
    }
  }
}


unsigned long outTempFailTest = 0;
byte outHumFailTest = 0;


void getOutsideTemp(){
  float temp;

  if(sensorTest == TEST_OUTTEMP){
    testOutTemp();
  }
  else{
    if(greenhouse.outsideTempSensor.value() == DS18B20_TEMP){
      ds18b20_out.requestTemperatures();
      temp = ds18b20_out.getTempCByIndex(0);

      if(checkSensorFailure(outsideTemperature.value(), temp, -127.00,60.00,1)){
        if(outTempFailTest < 20){
          outTempFailTest++;
        }
      }
      else{
        outTempFailTest = 0;
      }

    }
    else if(greenhouse.outsideTempSensor.value() == SHT1X_TEMP){
      temp = sht1x_out.readTemperatureC();
      if(checkSensorFailure(outsideTemperature.value(), temp, -40.00,60.00,1)){
        if(outTempFailTest < 20){
          outTempFailTest++;
        }
      }
      else{
        outTempFailTest = 0;
      }
    }
    if(outTempFailTest == 0){
      outsideTemperature.registerValue(temp);
      outSensorFailure = false;
    }
    else if (outTempFailTest == 20){
      outSensorFailure = true;
    }
  }
}



void getOutsideHum(){
  float hum;
  if(sensorTest == TEST_OUTTEMP){
    testOutHum();
  }
  else{
    if(greenhouse.outsideTempSensor.value() == SHT1X_TEMP){
      hum = sht1x_out.readHumidity();

      if(checkSensorFailure(greenhouseHumidity.value(), hum, -1.00,101.00,2)){
        if(outHumFailTest < 20){
          outHumFailTest++;
        }
      }
      else{
        outHumFailTest = 0;
      }
    }

    if(outTempFailTest == 0){
      outsideHumidity.registerValue(hum);
      outSensorFailure = false;
    }
    else if (outTempFailTest == 20){
      outSensorFailure = true;
    }
  }
}

float mVPyranometer;
float mVTensiometer[3];


void getRadiation(){
  if(sensorTest == TEST_RADIATION){
    testRadiation();
  }
  else{
    int aRead = analogRead(LIGHT_SENSOR);
    mVPyranometer = aRead * (5.0 / 1023.0);
    float nowWattms = (float)1800/1024*(float)aRead;
    radiation.registerValue((unsigned long)nowWattms);
    //Serial.println(F("New record!"));
    //Serial.print(F("Value;"));
    //Serial.print(radiation.value());
    //Serial.print(F("Average;"));
    //Serial.print(radiation.hourAverage(greenhouse.hr()));
    //Serial.print(F(";Average24h;"));
    //Serial.print(radiation.averageDailyLux());
    //Serial.print(F(";Average until now;"));
    //Serial.print(radiation.averageLuxUntilNow());
    //Serial.print(F(";Weather ratio;"));
    //Serial.println(radiation.autoWeatherRatio());
  }
}

void computeSoilMoistureAverage(){
  float average = 0;
  byte count = 0;
  for(int x = 0; x < MAX_TENSIOMETERS; x++){
    if(greenhouse.tensiometer[x].value() == true){
      average += soilMoisture[x].value();
      count++;
    }
  }
  if(count > 0){
    average/= count;
    soilMoisture[MAX_TENSIOMETERS].registerValue(average);
  }
}

void getSoilMoisture(){

  if(greenhouse.tensiometer[0].value()){
    if(sensorTest == TEST_TENSIOM1){
      testTensiom1();
    }
    else{
      int aRead = analogRead(SOIL_MOISTURE1);
      mVTensiometer[0] = aRead * (5.0 / 1023.0);
      float kPa = (float)95/(float)818*((float)aRead-102);
      soilMoisture[0].registerValue(kPa);
    }
  }
  if(greenhouse.tensiometer[1].value()){
    int aRead = analogRead(SOIL_MOISTURE2);
    mVTensiometer[1] = aRead * (5.0 / 1023.0);
    float kPa = (float)95/(float)818*((float)aRead-102);
    soilMoisture[1].registerValue(kPa);
  }
  if(greenhouse.tensiometer[2].value()){
    int aRead = analogRead(SOIL_MOISTURE3);
    mVTensiometer[2] = aRead * (5.0 / 1023.0);
    float kPa = (float)95/(float)818*((float)aRead-102);
    soilMoisture[2].registerValue(kPa);
  }

  tensiomFailure = false;
  for(int x = 0; x < MAX_TENSIOMETERS; x++){
    if(greenhouse.tensiometer[x].value() == true && (soilMoisture[x].value() <= greenhouse.minTensiometerValue.value()||soilMoisture[x].value() >= greenhouse.maxTensiometerValue.value())){
      tensiomFailure = true;
    }
  }

  computeSoilMoistureAverage();


}

//These values are in the datasheet
#define RT0 10000   // Ω
#define B 3380      // K
//--------------------------------------
#define VCC 5    //Supply voltage
#define R 10000  //R=10KΩ

void getTemperatureBackup(){

  float RT, VR, ln, TX, T0, VRT;

  T0 = 25 + 273.15;                 //Temperature T0 from datasheet, conversion from Celsius to kelvin
  VRT = analogRead(TEMP_BACKUP);              //Acquisition analog value of VRT
  VRT = (5.00 / 1023.00) * VRT;      //Conversion to voltage
  VR = VCC - VRT;
  RT = VRT / (VR / R);               //Resistance of RT
  ln = log(RT / RT0);
  TX = (1 / ((ln / B) + (1 / T0))); //Temperature from thermistor

  TX = TX - 273.15;
  temperatureBackup = TX;
    /*
  float Vin=5.0;     // [V]
  float Rt=10000;    // Resistor t [ohm]
  float Re0=10000;    // value of rct in T0 [ohm]
  float Te0=298.15;   // use T0 in Kelvin [K]
  float Vout=0.0;    // Vout in A0
  float Rout=0.0;    // Rout in A0
  // use the datasheet to get this data.
  float Te1=273.15;      // [K] in datasheet 0º C
  float Te2=373.15;      // [K] in datasheet 100° C
  float RT1=35563;   // [ohms]  resistence in T1
  float RT2=549;    // [ohms]   resistence in T2
  float beta=0.0;    // initial parameters [K]
  float Rinf=0.0;    // initial parameters [ohm]
  float TempK=0.0;   // variable output
  float TempC=0.0;   // variable output

  beta=(log(RT1/RT2))/((1/Te1)-(1/Te2));
  Rinf=Re0*exp(-beta/Te0);
  Vout=Vin*((float)(analogRead(TEMP_BACKUP))/1024.0); // calc for ntc
  Rout=(Rt*Vout/(Vin-Vout));

  TempK=(beta/log(Rout/Rinf)); // calc for temperature
  temperatureBackup=TempK-273.15;

  //Serial.println(temperatureBackup);*/


}

void autoWeather(){
  if ((greenhouse.autoWeather.value() == true)&&(greenhouse.weatheradjust.value() == true)){
    greenhouse.weatherP.setValue(radiation.autoWeatherRatio(greenhouse.hourOffset.value(), greenhouse.sunSetpoint.value()));
  }
}

void getOnTime(){
  D1.onTime.record(greenhouse.hr(), greenhouse.mn(), D1.isOn());
  D2.onTime.record(greenhouse.hr(), greenhouse.mn(), D2.isOn());
  D3.onTime.record(greenhouse.hr(), greenhouse.mn(), D3.isOn());
  D4.onTime.record(greenhouse.hr(), greenhouse.mn(), D4.isOn());
  D5.onTime.record(greenhouse.hr(), greenhouse.mn(), D5.isOn());
  D6.onTime.record(greenhouse.hr(), greenhouse.mn(), D5.isOn());
}

void initSensors(){
  //last recorded value if probe doesnt reply back at first cycle
    //sensorBackup();
  //start communication with temp probe
  if(greenhouse.insideTempSensor.value() == DS18B20_TEMP){
    ds18b20_in.begin();
    ds18b20_in.setResolution(12);
  }
  else if(greenhouse.insideTempSensor.value() == SHT1X_TEMP){
    sht1x_in.initSensor();
  }

  if(greenhouse.outsideTempSensor.value() == DS18B20_TEMP){
    ds18b20_out.begin();
    ds18b20_out.setResolution(12);
  }
  else if(greenhouse.outsideTempSensor.value() == SHT1X_TEMP){
    sht1x_out.initSensor();
  }
  //start communication with clock
    rtc.begin();
  //rain sensor
    pinMode(RAIN_SWITCH, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(RAIN_SWITCH), isr_rg, FALLING);
    pinMode(WIND_DIGITAL_SENSOR, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(WIND_DIGITAL_SENSOR), isr_ane, FALLING);
    sei();// Enable Interrupts
    greenhouseTemperature.clearRecords();
    for(int x = 0; x< MAX_TENSIOMETERS;x++){
      soilMoisture[x].setSmoothing(0.01);
    }
    radiation.setCalibrationType(SLOPE);
}
