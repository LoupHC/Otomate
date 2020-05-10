//Assign same pin to different outputs
#if ROLLUP1_OPENING_PIN != OFF_VAL && (ROLLUP1_OPENING_PIN == ROLLUP1_CLOSING_PIN ||ROLLUP1_OPENING_PIN == ROLLUP2_CLOSING_PIN || ROLLUP1_OPENING_PIN == ROLLUP2_OPENING_PIN || ROLLUP1_OPENING_PIN == DEVICE1_PIN || ROLLUP1_OPENING_PIN == DEVICE2_PIN || ROLLUP1_OPENING_PIN == ALARM_PIN)
  #error Rollup1 opening pin attributed to other output
#endif

#if ROLLUP1_CLOSING_PIN != OFF_VAL  && (ROLLUP1_CLOSING_PIN == ROLLUP1_OPENING_PIN ||ROLLUP1_CLOSING_PIN == ROLLUP2_CLOSING_PIN || ROLLUP1_CLOSING_PIN == ROLLUP2_OPENING_PIN || ROLLUP1_CLOSING_PIN == DEVICE1_PIN || ROLLUP1_CLOSING_PIN == DEVICE2_PIN || ROLLUP1_CLOSING_PIN == ALARM_PIN)
  #error Rollup1 closing pin attributed to other output
#endif

#if ROLLUP2_OPENING_PIN != OFF_VAL && (ROLLUP2_OPENING_PIN == ROLLUP1_CLOSING_PIN ||ROLLUP2_OPENING_PIN == ROLLUP2_CLOSING_PIN || ROLLUP2_OPENING_PIN == ROLLUP1_OPENING_PIN  || ROLLUP2_OPENING_PIN == DEVICE1_PIN || ROLLUP2_OPENING_PIN == DEVICE2_PIN || ROLLUP2_OPENING_PIN == ALARM_PIN)
  #error Rollup2 opening pin attributed to other output
#endif

#if ROLLUP2_CLOSING_PIN != OFF_VAL  && (ROLLUP2_CLOSING_PIN == ROLLUP1_CLOSING_PIN ||ROLLUP2_CLOSING_PIN == ROLLUP1_OPENING_PIN || ROLLUP2_CLOSING_PIN == ROLLUP2_OPENING_PIN || ROLLUP2_CLOSING_PIN == DEVICE1_PIN || ROLLUP2_CLOSING_PIN == DEVICE2_PIN || ROLLUP2_CLOSING_PIN == ALARM_PIN)
  #error Rollup2 closing pin attributed to other output
#endif


//Undeclared output pins
#if (ROLLUP1_OPENING_PIN == OFF_VAL || ROLLUP1_CLOSING_PIN == OFF_VAL) && ROLLUPS > 0
    #error Rollup1 pin is missing
#endif

#if (ROLLUP2_OPENING_PIN == OFF_VAL || ROLLUP2_CLOSING_PIN == OFF_VAL) && ROLLUPS > 1
      #error Rollup2 pin is missing
#endif

#if DEVICE1_PIN == OFF_VAL && DEVICES > 0
      #error Device1 pin is missing
#endif

#if DEVICE2_PIN == OFF_VAL && DEVICES > 1
      #error Device2 pin is missing
#endif


//Too much class instances for greenhouseLib
#if TIMEPOINTS < 0 ||  TIMEPOINTS > MAX_TIMEPOINTS
  #error So many timepoints... really?
#endif

#if ROLLUPS < 0 || ROLLUPS > MAX_ROLLUPS
  #error So many rollups... really?
#endif

#if STAGES < 0 || STAGES > MAX_STAGES
  #error So many stages... really?
#endif
#if DEVICES < 0 || DEVICES > MAX_DEVICES
  #error So many devices... really?
#endif

//Variables are off limits
#if (ROLLUP1_OPENING_PIN  > 7 || ROLLUP1_OPENING_PIN < 0) && ROLLUP1_OPENING_PIN != OFF_VAL && defined(I2C_OUTPUTS)
  #error Check your pinout...
#endif
#if (ROLLUP1_CLOSING_PIN  > 7 || ROLLUP1_CLOSING_PIN < 0) && ROLLUP1_CLOSING_PIN != OFF_VAL && defined(I2C_OUTPUTS)
  #error Check your pinout...
#endif
#if (ROLLUP2_OPENING_PIN  > 7 || ROLLUP2_OPENING_PIN < 0) && ROLLUP2_OPENING_PIN != OFF_VAL && defined(I2C_OUTPUTS)
  #error Check your pinout...
#endif
#if (ROLLUP2_CLOSING_PIN  > 7 || ROLLUP2_CLOSING_PIN < 0) && ROLLUP2_CLOSING_PIN != OFF_VAL && defined(I2C_OUTPUTS)
  #error Check your pinout...
#endif
#if (DEVICE1_PIN  > 7 || DEVICE1_PIN < 0) && DEVICE1_PIN != OFF_VAL && defined(I2C_OUTPUTS)
  #error Check your pinout...
#endif
#if (DEVICE2_PIN  > 7 || DEVICE2_PIN < 0) && DEVICE2_PIN != OFF_VAL && defined(I2C_OUTPUTS)
  #error Check your pinout...
#endif
#if (ALARM_PIN  > 7 || ALARM_PIN) < 0 && ALARM_PIN != OFF_VAL && defined(I2C_OUTPUTS)
  #error Check your pinout...
#endif

//Timepoints
#if (TP1_TYPE != SR && TP1_TYPE != CLOCK && TP1_TYPE != SS)
  #error Wrong timepoint type
#endif

#if (TP1_HOUR > 23 || TP1_HOUR < 0 && TP1_TYPE == CLOCK)
  #error Wrong timepoints hour value
#elif(TP1_HOUR > 23 || TP1_HOUR < -23)
  #error Wrong timepoints hour value
#endif

#if (TP1_MN_MOD > 59 || TP1_MN_MOD < 0 && TP1_TYPE == CLOCK)
  #error Wrong timepoints minut value
#elif(TP1_MN_MOD > 59 || TP1_MN_MOD < -59)
  #error Wrong timepoints minut value
#endif

#if (TP1_HEAT > 50 || TP1_HEAT < 0)
  #error Check your timepoints parameter : thats damn hot/cold
#endif
#if (TP1_COOL > 50 || TP1_HEAT < COOL)
  #error Check your timepoints parameter : thats damn hot/cold
#endif


#if (TP2_TYPE != SR && TP2_TYPE != CLOCK && TP2_TYPE != SS)
  #error Wrong timepoint type
#endif

#if (TP2_HOUR > 23 || TP2_HOUR < 0 && TP2_TYPE == CLOCK)
  #error Wrong timepoints hour value
#elif(TP2_HOUR > 23 || TP2_HOUR < -23)
  #error Wrong timepoints hour value
#endif

#if (TP2_MN_MOD > 59 || TP2_MN_MOD < 0 && TP2_TYPE == CLOCK)
  #error Wrong timepoints minut value
#elif(TP2_MN_MOD > 59 || TP2_MN_MOD < -59)
  #error Wrong timepoints minut value
#endif

#if (TP2_HEAT > 50 || TP2_HEAT < 0)
  #error Check your timepoints parameter : thats damn hot/cold
#endif
#if (TP2_COOL > 50 || TP2_HEAT < COOL)
  #error Check your timepoints parameter : thats damn hot/cold
#endif


#if (TP3_TYPE != SR && TP3_TYPE != CLOCK && TP3_TYPE != SS)
  #error Wrong timepoint type
#endif

#if (TP3_HOUR > 23 || TP3_HOUR < 0 && TP3_TYPE == CLOCK)
  #error Wrong timepoints hour value
#elif(TP3_HOUR > 23 || TP3_HOUR < -23)
  #error Wrong timepoints hour value
#endif

#if (TP3_MN_MOD > 59 || TP3_MN_MOD < 0 && TP3_TYPE == CLOCK)
  #error Wrong timepoints minut value
#elif(TP3_MN_MOD > 59 || TP3_MN_MOD < -59)
  #error Wrong timepoints minut value
#endif

#if (TP3_HEAT > 50 || TP3_HEAT < 0)
  #error Check your timepoints parameter : thats damn hot/cold
#endif
#if (TP3_COOL > 50 || TP3_HEAT < COOL)
  #error Check your timepoints parameter : thats damn hot/cold
#endif


#if (TP4_TYPE != SR && TP4_TYPE != CLOCK && TP4_TYPE != SS)
  #error Wrong timepoint type
#endif

#if (TP4_HOUR > 23 || TP4_HOUR < 0 && TP4_TYPE == CLOCK)
  #error Wrong timepoints hour value
#elif(TP4_HOUR > 23 || TP4_HOUR < -23)
  #error Wrong timepoints hour value
#endif

#if (TP4_MN_MOD > 59 || TP4_MN_MOD < 0 && TP4_TYPE == CLOCK)
  #error Wrong timepoints minut value
#elif(TP4_MN_MOD > 59 || TP4_MN_MOD < -59)
  #error Wrong timepoints minut value
#endif

#if (TP4_HEAT > 50 || TP4_HEAT < 0)
  #error Check your timepoints parameter : thats damn hot/cold
#endif
#if (TP4_COOL > 50 || TP4_HEAT < COOL)
  #error Check your timepoints parameter : thats damn hot/cold
#endif

#if (TP5_TYPE != SR && TP5_TYPE != CLOCK && TP5_TYPE != SS)
  #error Wrong timepoint type
#endif

#if (TP5_HOUR > 23 || TP5_HOUR < 0 && TP5_TYPE == CLOCK)
  #error Wrong timepoints hour value
#elif(TP5_HOUR > 23 || TP5_HOUR < -23)
  #error Wrong timepoints hour value
#endif

#if (TP5_MN_MOD > 59 || TP5_MN_MOD < 0 && TP5_TYPE == CLOCK)
  #error TP5_MN_MOD OFF_VAL LIMIT
#elif(TP5_MN_MOD > 59 || TP5_MN_MOD < -59)
  #error   TP5_MN_MOD OFF_VAL LIMIT

#endif

#if (TP5_HEAT > 50 || TP5_HEAT < 0)
  #error TP5_HEAT OFF_VAL LIMIT : thats damn hot/cold
#endif
#if (TP5_COOL > 50 || TP5_HEAT < COOL)
  #error TP5_COOL OFF_VAL LIMIT : thats damn hot/cold
#endif

//Rollups
#if R1_HYST > 5 || R1_HYST < 0
  #error R1_HYST OFF_VAL LIMITS
#endif
#if R1_ROTUP > 500 || R1_ROTUP < 0
  #error R1_ROTUP OFF_VAL LIMITS
#endif
#if R1_ROTDOWN > 500 || R1_ROTDOWN < 0
  #error R1_ROTDOWN OFF_VAL LIMITS
#endif
#if R1_PAUSE > 240 || R1_PAUSE < 0
  #error R1_PAUSE OFF_VAL LIMITS
#endif
#if R2_HYST > 5 || R2_HYST < 0
  #error R2_HYST OFF_VAL LIMITS
#endif
#if R2_ROTUP > 500 || R2_ROTUP < 0
  #error R2_ROTUP OFF_VAL LIMITS
#endif
#if R2_ROTDOWN > 500 || R2_ROTDOWN < 0
  #error R2_ROTDOWN OFF_VAL LIMITS
#endif
#if R2_PAUSE > 240 || R2_PAUSE < 0
  #error R2_PAUSE OFF_VAL LIMITS
#endif

//Stages
#if R1_S0_MOD >  5 ||  R1_S0_MOD < 0
  #error R1_S0_MOD OFF_VAL LIMITS
#endif
#if R1_S0_TARGET >  100 ||  R1_S0_TARGET < 0
  #error R1_S0_TARGET OFF_VAL LIMITS
#endif
#if R1_S1_MOD >  5 ||  R1_S1_MOD < 0 || R1_S1_MOD < R1_S0_MOD
  #error R1_S1_MOD OFF_VAL LIMITS
#endif
#if R1_S1_TARGET >  100 ||  R1_S1_TARGET < 0 || R1_S1_TARGET < R1_S0_TARGET
  #error R1_S1_TARGET OFF_VAL LIMITS
#endif
#if R1_S2_MOD >  5 ||  R1_S2_MOD < 0 || R1_S2_MOD < R1_S1_MOD
  #error R1_S2_MOD OFF_VAL LIMITS
#endif
#if R1_S2_TARGET >  100 ||  R1_S2_TARGET < 0 || R1_S2_TARGET < R1_S1_TARGET
  #error R1_S2_TARGET OFF_VAL LIMITS
#endif
#if R1_S3_MOD >  5 ||  R1_S3_MOD < 0 || R1_S3_MOD < R1_S2_MOD
  #error R1_S3_MOD OFF_VAL LIMITS
#endif
#if R1_S3_TARGET >  100 ||  R1_S3_TARGET < 0 || R1_S3_TARGET < R1_S2_TARGET
  #error R1_S3_TARGET OFF_VAL LIMITS
#endif
#if R1_S4_MOD >  5 ||  R1_S4_MOD < 0 || R1_S4_MOD < R1_S3_MOD
  #error R1_S4_MOD OFF_VAL LIMITS
#endif
#if R1_S4_TARGET >  100 ||  R1_S4_TARGET < 0 || R1_S4_TARGET < R1_S3_TARGET
  #error R1_S4_TARGET OFF_VAL LIMITS
#endif
#if R2_S0_MOD >  5 ||  R2_S0_MOD < 0
  #error R2_S0_MOD OFF_VAL LIMITS
#endif
#if R2_S0_TARGET >  100 ||  R2_S0_TARGET < 0
  #error R2_S0_TARGET OFF_VAL LIMITS
#endif
#if R2_S1_MOD >  5 ||  R2_S1_MOD < 0 || R2_S1_MOD < R2_S0_MOD
  #error R2_S1_MOD OFF_VAL LIMITS
#endif
#if R2_S1_TARGET >  100 ||  R2_S1_TARGET < 0 || R2_S1_TARGET < R2_S0_TARGET
  #error R2_S1_TARGET OFF_VAL LIMITS
#endif
#if R2_S2_MOD >  5 ||  R2_S2_MOD < 0 || R2_S2_MOD < R2_S1_MOD
  #error R2_S2_MOD OFF_VAL LIMITS
#endif
#if R2_S2_TARGET >  100 ||  R2_S2_TARGET < 0 || R2_S2_TARGET < R2_S1_TARGET
  #error R2_S2_TARGET OFF_VAL LIMITS
#endif
#if R2_S3_MOD >  5 ||  R2_S3_MOD < 0 || R2_S3_MOD < R2_S2_MOD
  #error R2_S3_MOD OFF_VAL LIMITS
#endif
#if R2_S3_TARGET >  100 ||  R2_S3_TARGET < 0 || R2_S3_TARGET < R2_S2_TARGET
  #error R2_S3_TARGET OFF_VAL LIMITS
#endif
#if R2_S4_MOD >  5 ||  R2_S4_MOD < 0 || R2_S4_MOD < R2_S3_MOD
  #error R2_S4_MOD OFF_VAL LIMITS
#endif
#if R2_S4_TARGET >  100 ||  R2_S4_TARGET < 0 || R2_S4_TARGET < R2_S3_TARGET
  #error R2_S4_TARGET OFF_VAL LIMITS
#endif

//Devices
#if F1_HYST > 5 || F1_HYST < 0
  #error F1_HYST OFF_VAL LIMITS
#endif
#if F1_MOD > 10 || F1_MOD < -5
  #error F1_MOD OFF_VAL LIMITS
#endif
#if F2_HYST > 5 || F2_HYST < 0
  #error F2_HYST OFF_VAL LIMITS
#endif
#if F2_MOD > 10 || F2_MOD < -5
  #error F2_MOD OFF_VAL LIMITS
#endif

//Heaters
#if H1_HYST > 5 || H1_HYST < 0
  #error H1_HYST OFF_VAL LIMITS
#endif
#if H1_MOD > 5 || H1_MOD < -10
  #error H1_MOD OFF_VAL LIMITS
#endif
#if H2_HYST > 5 || H2_HYST < 0
  #error H2_HYST OFF_VAL LIMITS
#endif
#if H2_MOD > 5 || H2_MOD < -10
  #error H2_MOD OFF_VAL LIMITS
#endif
