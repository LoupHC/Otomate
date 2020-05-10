
//**************************************SD CARD***************
#include <SPI.h>
#include <SDE.h>
//Otomate V.2
const int chipSelect = 53;

File datalog;
File parameters;
int lastMinutRecord;
int lastHourRecord;
int lastDayRecord;

int minutCount = 0;
const int exportDelayForDailyRecord = 5;//minuts

unsigned short exportCounter = 0;


//***************************************************
//*********************EXPORTS**************************
//***************************************************

void exportData(){
    String counter = String(exportCounter+1);
    Serial.print(F("ROW,SET,"));
    Serial.println(counter);
    Serial.print(F("DATA,DATE,TIME,"));
    Serial.print(greenhouseTemperature.value());
    Serial.print(F("),F("));
    Serial.println(greenhouseHumidity.value());
    Serial.print(F("CELL,SET,G2,"));
    Serial.println(exportCounter);
}

String dailyRecord(){

    t = rtc.getTime();

    String day_s = String(t.date);
    String fullday_s;

    if(t.date<10){
      fullday_s = String('0'+day_s);
    }
    else{
      fullday_s = String(day_s);
    }

    String month_s = String(t.mon);
    String fullmonth_s;

    if(t.mon<10){
      fullmonth_s = String('0'+month_s);
    }
    else{
      fullmonth_s = String(month_s);
    }

    String year_s = String(t.year);
    String fileName =  "DDATALOG/"+ year_s + fullmonth_s + fullday_s+".txt";

    return fileName;
}

String monthlyRecord(){

    t = rtc.getTime();

    String month_s = String(t.mon);
    String fullmonth_s;

    if(t.mon<10){
      fullmonth_s = String('0'+month_s);
    }
    else{
      fullmonth_s = String(month_s);
    }

    String year_s = String(t.year);
    String fileName = "MDATALOG/"+ year_s + fullmonth_s +".txt";

    return fileName;
}

String paramRecord(){

    t = rtc.getTime();

    String day_s = String(t.date);
    String fullday_s;

    if(t.date<10){
      fullday_s = String('0'+day_s);
    }
    else{
      fullday_s = String(day_s);
    }
    String month_s = String(t.mon);
    String fullmonth_s;

    if(t.mon<10){
      fullmonth_s = String('0'+month_s);
    }
    else{
      fullmonth_s = String(month_s);
    }

    String year_s = String(t.year);
    String fileName = "PARAM/"+year_s + fullmonth_s + fullday_s + ".txt";

    return fileName;
}

float lastRainfall= 0;

void exportData(String record){
  float rainSample;
  if(totalRainfall >= lastRainfall){
    rainSample = totalRainfall-lastRainfall;
  }
  else{
    rainSample = totalRainfall;
  }
  lastRainfall = totalRainfall;

  SDEClass SD;
  if (!SD.begin(chipSelect)) {
      Serial.println(F("Card failed, or not present"));
      return;
  }
  datalog = SD.open(record, FILE_WRITE);
  if (SD.exists(record)) {
    Serial.println(record);
  }
  else{
    Serial.println(F("Error writing to file"));
  }
  if (datalog) {
    datalog.print(greenhouse.hr());
    datalog.print(F(":"));
    datalog.print(greenhouse.mn());
    datalog.print(F(":00,"));
    datalog.print(greenhouseTemperature.value());
    datalog.print(F(","));
    if(greenhouse.insideTempSensor.value() == SHT1X_TEMP){
      datalog.print(greenhouseHumidity.value());
    }
    datalog.print(F(","));
    if(greenhouse.insideTempSensor.value() == SHT1X_TEMP){
      datalog.print(greenhouseHumidity.absolute(greenhouseTemperature.value()));
    }
    datalog.print(F(","));
    if(greenhouse.outsideTempSensor.value() != OFF_TEMP){
      datalog.print(outsideTemperature.value());
    }
    datalog.print(F(","));
    if(greenhouse.outsideTempSensor.value() == SHT1X_TEMP){
      datalog.print(outsideHumidity.value());
    }
    datalog.print(F(","));
    if(greenhouse.outsideTempSensor.value() == SHT1X_TEMP){
      datalog.print(outsideHumidity.absolute(outsideTemperature.value()));
    }
    datalog.print(F(","));
    if(greenhouse.rainSensor.value() != OFF_RAIN){
      datalog.print(rain);

    }
    datalog.print(F(","));
    if(greenhouse.rainSensor.value() == RG11_BUCKET){
      datalog.print(rainSample);
    }
    datalog.print(F(","));
    if(greenhouse.anemometer.value() != OFF_WIND){
      datalog.print(windSpeed);
    }
    datalog.print(F(","));
    if(greenhouse.radiationSensor.value()){
      datalog.print(radiation.value());
    }
    datalog.print(F(","));
    if(greenhouse.radiationSensor.value()){
      datalog.print(radiation.dayJoules());
    }
    datalog.print(F(","));
    for (int x = 0; x < MAX_TENSIOMETERS;x++){
      if(greenhouse.tensiometer[x].value()){
        datalog.print(soilMoisture[x].value());
      }
      datalog.print(F(","));
    }
    datalog.print(greenhouse.weather());
    datalog.print(F(","));
    datalog.print(greenhouse.heatingTemp());
    datalog.print(F(","));
    datalog.print(greenhouse.coolingTemp());
    datalog.print(F(","));
    if(R1.isActivated()){
      datalog.print(R1.incrementCounter());
    }
    datalog.print(F(","));
    if(R2.isActivated()){
      datalog.print(R2.incrementCounter());
    }
    datalog.print(F(","));
    if(R3.isActivated()){
      datalog.print(R3.incrementCounter());
    }
    datalog.print(F(","));
    if(D1.isActivated()){
      datalog.print(D1.isOn());
    }
    datalog.print(F(","));
    if(D2.isActivated()){
      datalog.print(D2.isOn());
    }
    datalog.print(F(","));
    if(D3.isActivated()){
      datalog.print(D3.isOn());
    }
    datalog.print(F(","));
    if(D4.isActivated()){
      datalog.print(D4.isOn());
    }
    datalog.print(F(","));
    if(D5.isActivated()){
      datalog.print(D5.isOn());
    }
    datalog.print(F(","));
    if(D6.isActivated()){
      datalog.println(D6.isOn());
    }
    datalog.print(F(","));
    if(greenhouse.alarmEnabled.value()){
      datalog.println(greenhouse.alarm.isActive());
    }
    datalog.close(); // close the file
  }
}

void exportParametersToSD(){

  SDEClass SD;
  if (!SD.begin(chipSelect)) {
    Serial.println(F("Card failed, or not present"));
    return;
  }
  if (SD.exists(paramRecord())) {
    Serial.println("PARAMETERS rewrite");
    SD.remove(paramRecord());
  }
  datalog = SD.open(paramRecord(), FILE_WRITE);
  if (datalog) {
    for(int x = 0; x < MAX_ROLLUPS;x++){

      datalog.print(F(";"));
      datalog.print(greenhouse.rollup[x].hyst.value());
      datalog.print(F(";"));
      datalog.print(greenhouse.rollup[x].rotationUp.value());
      datalog.print(F(";"));
      datalog.print(greenhouse.rollup[x].rotationDown.value());
      datalog.print(F(";"));
      datalog.print(greenhouse.rollup[x].pause.value());

      for(int y = 0; y < MAX_STAGES;y++){
        datalog.print(F(";"));
        datalog.print(greenhouse.rollup[x].stage[y].mod.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.rollup[x].stage[y].target.value());
      }
      datalog.print(F(";"));
      datalog.print(greenhouse.rollup[x].stages.value());
      datalog.print(F(";"));
      datalog.print(greenhouse.rollup[x].enabled.value());
      datalog.print(F(";"));
      datalog.print(greenhouse.rollup[x].lock.value());
      datalog.print(F(";"));
      datalog.print(greenhouse.rollup[x].lockTarget.value());
      datalog.print(F(";"));
      datalog.print(greenhouse.rollup[x].currentLimit.value());

      for(int y = 0; y < MAX_CLOCK_OV;y++){
      datalog.print(F(";"));
        datalog.print(greenhouse.rollup[x].clockOv[y].ID.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.rollup[x].clockOv[y].priority.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.rollup[x].clockOv[y].enabled.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.rollup[x].clockOv[y].target.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.rollup[x].clockOv[y].hrStart.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.rollup[x].clockOv[y].mnStart.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.rollup[x].clockOv[y].hrStop.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.rollup[x].clockOv[y].mnStop.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.rollup[x].clockOv[y].pulseOff.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.rollup[x].clockOv[y].pulseOn.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.rollup[x].clockOv[y].condition.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.rollup[x].clockOv[y].ovType.value());
      }
      for(int y = 0; y < MAX_BOOL_OV;y++){
      datalog.print(F(";"));
        datalog.print(greenhouse.rollup[x].boolOv[y].ID.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.rollup[x].boolOv [y].priority.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.rollup[x].boolOv [y].enabled.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.rollup[x].boolOv [y].target.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.rollup[x].boolOv [y].pulseOff.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.rollup[x].boolOv [y].pulseOn.value());
      }
      for(int y = 0; y < MAX_FLOAT_OV;y++){
      datalog.print(F(";"));
        datalog.print(greenhouse.rollup[x].floatOv[y].ID.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.rollup[x].floatOv [y].priority.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.rollup[x].floatOv [y].enabled.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.rollup[x].floatOv [y].target.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.rollup[x].floatOv [y].comparator.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.rollup[x].floatOv [y].floatVar.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.rollup[x].floatOv [y].hyst.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.rollup[x].floatOv [y].pulseOff.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.rollup[x].floatOv [y].pulseOn.value());
      }
    }

    for(int x = 0; x < MAX_DEVICES;x++){
    datalog.print(F(";"));
      datalog.print(greenhouse.device[x].mod.value());
      datalog.print(F(";"));
      datalog.print(greenhouse.device[x].hyst.value());
      datalog.print(F(";"));
      datalog.print(greenhouse.device[x].type.value());
      datalog.print(F(";"));
      datalog.print(greenhouse.device[x].enabled.value());
      datalog.print(F(";"));
      datalog.print(greenhouse.device[x].lock.value());
      datalog.print(F(";"));
      datalog.print(greenhouse.device[x].lockTarget.value());
      datalog.print(F(";"));
      datalog.print(greenhouse.device[x].pulse.value());
      datalog.print(F(";"));
      datalog.print(greenhouse.device[x].tensiometer.value());


      for(int y = 0; y < MAX_CLOCK_OV;y++){
      datalog.print(F(";"));
        datalog.print(greenhouse.device[x].clockOv[y].ID.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.device[x].clockOv[y].priority.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.device[x].clockOv[y].enabled.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.device[x].clockOv[y].target.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.device[x].clockOv[y].hrStart.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.device[x].clockOv[y].mnStart.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.device[x].clockOv[y].hrStop.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.device[x].clockOv[y].mnStop.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.device[x].clockOv[y].pulseOff.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.device[x].clockOv[y].pulseOn.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.device[x].clockOv[y].condition.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.device[x].clockOv[y].ovType.value());
      }

      for(int y = 0; y < MAX_BOOL_OV;y++){
      datalog.print(F(";"));
        datalog.print(greenhouse.device[x].boolOv[y].ID.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.device[x].boolOv [y].priority.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.device[x].boolOv [y].enabled.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.device[x].boolOv [y].target.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.device[x].boolOv [y].pulseOff.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.device[x].boolOv [y].pulseOn.value());
      }

      for(int y = 0; y < MAX_FLOAT_OV;y++){
      datalog.print(F(";"));
        datalog.print(greenhouse.device[x].floatOv[y].ID.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.device[x].floatOv [y].priority.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.device[x].floatOv [y].enabled.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.device[x].floatOv [y].target.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.device[x].floatOv [y].comparator.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.device[x].floatOv [y].floatVar.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.device[x].floatOv [y].hyst.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.device[x].floatOv [y].pulseOff.value());
        datalog.print(F(";"));
        datalog.print(greenhouse.device[x].floatOv [y].pulseOn.value());
      }
    }
    for(int x = 0; x < MAX_TIMEPOINTS;x++){
    datalog.print(F(";"));
      datalog.print(greenhouse.timepoint[x].type.value());
      datalog.print(F(";"));
      datalog.print(greenhouse.timepoint[x].mnMod.value());
      datalog.print(F(";"));
      datalog.print(greenhouse.timepoint[x].hrMod.value());
      datalog.print(F(";"));
      datalog.print(greenhouse.timepoint[x].coolingTemp.value());
      datalog.print(F(";"));
      datalog.print(greenhouse.timepoint[x].heatingTemp.value());
      datalog.print(F(";"));
      datalog.print(greenhouse.timepoint[x].coolingTempCloud.value());
      datalog.print(F(";"));
      datalog.print(greenhouse.timepoint[x].heatingTempCloud.value());
      datalog.print(F(";"));
      datalog.print(greenhouse.timepoint[x].ramping.value());
    }
    datalog.print(F(";"));
    datalog.print(greenhouse.timezone.value());
    datalog.print(F(";"));
    datalog.print(greenhouse.latitude.value());
    datalog.print(F(";"));
    datalog.print(greenhouse.longitude.value());
    datalog.print(F(";"));
    datalog.print(greenhouse.timepoints.value());
    datalog.print(F(";"));
    datalog.print(greenhouse.rollups.value());
    datalog.print(F(";"));
    datalog.print(greenhouse.devices.value());
    datalog.print(F(";"));
    datalog.print(greenhouse.daynight.value());
    datalog.print(F(";"));
    datalog.print(greenhouse.weatheradjust.value());
    datalog.print(F(";"));
    datalog.print(greenhouse.dif.value());
    datalog.print(F(";"));
    datalog.print(greenhouse.prenight.value());
    datalog.print(F(";"));
    datalog.print(greenhouse.weatherP.value());
    datalog.print(F(";"));
    datalog.print(greenhouse.insideTempSensor.value());
    datalog.print(F(";"));
    datalog.print(greenhouse.outsideTempSensor.value());
    datalog.print(F(";"));
    datalog.print(greenhouse.radiationSensor.value());
    datalog.print(F(";"));
    datalog.print(greenhouse.rainSensor.value());
    datalog.print(F(";"));
    datalog.print(greenhouse.anemometer.value());
    for(int y = 0; y < MAX_TENSIOMETERS;y++){
      datalog.print(F(";"));
      datalog.print(greenhouse.tensiometer[y].value());
    }
    datalog.print(F(";"));
    datalog.print(greenhouse.autoWeather.value());
    datalog.print(F(";"));
    datalog.print(greenhouse.lowTempAlarm.value());
    datalog.print(F(";"));
    datalog.print(greenhouse.highTempAlarm.value());
    datalog.print(F(";"));
    datalog.print(greenhouse.minTemp.value());
    datalog.print(F(";"));
    datalog.print(greenhouse.maxTemp.value());
    datalog.print(F(";"));
    datalog.print(greenhouse.autoWeather.value());
    datalog.print(F(";"));
    datalog.print(greenhouse.alarmEnabled.value());
    datalog.print(F(";"));
    datalog.print(greenhouse.energySavingMode.value());
  }
  datalog.close();
}

String recordHeader(){
  return "Time,InTemp,InHum,InAbs,OutTemp,OutHum,OutAbs,isRaining,Rainfall,Windspeed,W/m2,Joules,Weather ratio,CoolingT,HeatingT,R1,R2,R3,D1,D2,D3";
}

void startRecordingToSD(){

  SDEClass SD;
  Serial.println(F("Initializing SD card..."));

  if (!SD.begin(chipSelect)) {
    Serial.println(F("Card failed, or not present"));
    return;
  }

  if (!SD.exists("PARAM")) SD.mkdir("PARAM");  // Directory does not exist, create it
  if (!SD.exists("PARAM"))
  {
    Serial.println(F("Could not create parameters folder "));
  }
  if (!SD.exists("MDATALOG")) SD.mkdir("MDATALOG");  // Directory does not exist, create it
  if (!SD.exists("MDATALOG"))
  {
    Serial.println(F("Could not create monthdatalogs folder "));
  }

  if (!SD.exists("DDATALOG")) SD.mkdir("DDATALOG");  // Directory does not exist, create it
  if (!SD.exists("DDATALOG"))
  {
    Serial.println(F("Could not create daydatalogs folder "));
  }

  //daily record file
  if (!SD.exists(dailyRecord())) {
    Serial.println(F("Creating daily record file"));
    datalog = SD.open(dailyRecord(), FILE_WRITE);
    datalog.println(recordHeader());
    datalog.close();
  }
  if (SD.exists(dailyRecord())) {
    Serial.println(F("Daily record file exists"));
  }
  else{
    Serial.println(F("error creating Daily record file"));
  }

  //monthly record file

  if (!SD.exists(monthlyRecord())) {
    Serial.println(F("Creating monthly record file"));
    datalog = SD.open(monthlyRecord(), FILE_WRITE);
    datalog.println(recordHeader());
    datalog.close();
  }
  if (SD.exists(monthlyRecord())) {
    Serial.println(F("Monthly record file exists"));
  }
  else{
    Serial.println(F("error creating Monthly record file"));
  }
}

void dataloggingToSD(){

  byte checkMin;
  //new record every X minuts
  if(greenhouse.mn() != lastMinutRecord){
    lastMinutRecord = greenhouse.mn();
    minutCount++;
  }
  if (minutCount >= exportDelayForDailyRecord){
    exportData(dailyRecord());
    minutCount = 0;
  }
  //new record every hour

  if (greenhouse.hr() != lastHourRecord){
      exportData(monthlyRecord());
      lastHourRecord = greenhouse.hr();
    }
  if (rightNow[3] != lastDayRecord){
    if(Parameter::updated() == true){
      exportParametersToSD();
      Parameter::acknowledgeUpdate();
    }
    lastDayRecord = rightNow[3];
  }
}
