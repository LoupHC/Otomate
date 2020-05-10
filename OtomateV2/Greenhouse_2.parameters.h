/*
  Greenhouse_prototype_CAPE_v.1.1.ino
  Copyright (C)2017 Loup Hébert-Chartrand. All right reserved

  This code has been made to interface with Arduino-like microcontrollers,
  for inclusion in greenhouse automation devices.


  You can find the latest version of this code at :
  https://github.com/LoupHC/controleur-CAPE


  This code is free software; you can redistribute it and/or
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

//*******************************************************************
/*
(SEE Greenhouse_userManual.h FOR MORE DETAILS)

TIMEPOINTS PARAMETERS - SYNTAX RULES:
- type variable(TYPE) :   SR, CLOCK or SS (sunrise, manual, sunset)
- hour variable(HOUR) :   SR/SS types => 0 -24 to 24
                          CLOCK type => 0 to 24
- min variable(MN_MOD) :  SR/SS types => -60 to 60
                          CLOCK type => 0 to 60
- heating temperature variable(HEAT) : 0 to 50
- cooling temperature variable(COOL) : 0 to 50
- ramping (RAMP) : 0 to 60
*/
//*******************************************************Timepoint 1
#define TP1_TYPE            CLOCK
#define TP1_HOUR            7
#define TP1_MN_MOD          0
#define TP1_HEAT_SUN        18
#define TP1_COOL_SUN        20
#define TP1_HEAT_CLOUD      18
#define TP1_COOL_CLOUD      20
#define TP1_RAMP            0
//*******************************************************Timepoint 2
#define TP2_TYPE            CLOCK
#define TP2_HOUR            7
#define TP2_MN_MOD          0
#define TP2_HEAT_SUN        18
#define TP2_COOL_SUN        20
#define TP2_HEAT_CLOUD      18
#define TP2_COOL_CLOUD      20
#define TP2_RAMP            0
//*******************************************************Timepoint 3
#define TP3_TYPE            CLOCK
#define TP3_HOUR            19
#define TP3_MN_MOD          0
#define TP3_HEAT_SUN        18
#define TP3_COOL_SUN        20
#define TP3_HEAT_CLOUD      18
#define TP3_COOL_CLOUD      20
#define TP3_RAMP            0
//*******************************************************Timepoint 4
#define TP4_TYPE            CLOCK
#define TP4_HOUR            19
#define TP4_MN_MOD          0
#define TP4_HEAT_SUN        18
#define TP4_COOL_SUN        20
#define TP4_HEAT_CLOUD      16
#define TP4_COOL_CLOUD      18
#define TP4_RAMP            0
//*******************************************************Timepoint 5
/*
(SEE Greenhouse_userManual.h FOR MORE DETAILS)

ROLLUP PARAMETERS - SYNTAX RULES :
- hysteresis (HYST): 0 to 5
- rotation up(ROTUP): 0 to 300
- rotation down (ROTDOWN): 0 to 300
- pause time(PAUSE): 0 to 240
*/
//*******************************************************Rollup 1 (overral parameters)
#define R1_HYST             1
#define R1_ROTUP            0
#define R1_ROTDOWN          0
#define R1_PAUSE            20
//*******************************************************Rollup 2 (overral parameters)

#define R2_HYST             1
#define R2_ROTUP            0
#define R2_ROTDOWN          0
#define R2_PAUSE            20

//*******************************************************************
/*
(SEE Greenhouse_userManual.h FOR MORE DETAILS)

ROLLUP STAGES - SYNTAX RULES :
- temperature mod(MOD) : -5 to 10
- target increment(TARGET): 0 to 100
*/
//*******************************************************Rollup 1 (stages)
#define R1_S0_MOD           0
#define R1_S0_TARGET        0
#define R1_S1_MOD           0
#define R1_S1_TARGET        25
#define R1_S2_MOD           1
#define R1_S2_TARGET        50
#define R1_S3_MOD           2
#define R1_S3_TARGET        75
#define R1_S4_MOD           3
#define R1_S4_TARGET        100
//*******************************************************Rollup 2 (stages)

#define R2_S0_MOD           0
#define R2_S0_TARGET        0
#define R2_S1_MOD           0
#define R2_S1_TARGET        25
#define R2_S2_MOD           1
#define R2_S2_TARGET        50
#define R2_S3_MOD           2
#define R2_S3_TARGET        75
#define R2_S4_MOD           3
#define R2_S4_TARGET        100

//*************************************************************************
/*
(SEE Greenhouse_userManual.h FOR MORE DETAILS)

DEVICE PARAMETERS - SYNTAX RULES:
- hysteresis (HYST): 0 to 5
- temperature modificator (MOD): -5 to 10
*/
//*******************************************************Device parameters

#define D1_TYPE             FANTYPE
#define D1_HYST             1
#define D1_MOD              0
#define D2_TYPE             HEATERTYPE
#define D2_HYST             1
#define D2_MOD              0


//************************************************************************

/*ALARM
  Fire alarm (ALARM_PIN) when temperature get over a maximum or under a minimum
*/
#define ALARM_MIN_TEMP    5.00
#define ALARM_MAX_TEMP    35.00



#define R1_HRSTART  0
#define R1_MNSTART  0
#define R1_HRSTOP 0
#define R1_MNSTOP 0
#define R1_TARGET 50

#define D1_HRSTART  0
#define D1_MNSTART  0
#define D1_HRSTOP 0
#define D1_MNSTOP 0
#define D1_TARGET  true

#define DESHUM_HOT         80

#define DESHUM_COLD        80

#define RAIN_TARGET        20

#define MAX_WIND_SPEED     80


#define EXT_LOCK_PRIORITY   0
#define LOCK_PRIORITY       1
#define WINDOV_PRIORITY     2
#define RAINOV_PRIORITY     3
#define ANALOG_PRIORITY     3
#define OUTT_PRIORITY       4
#define CLOCKOV1_PRIORITY   5
#define CLOCKOV2_PRIORITY   6
#define CLOCKOV3_PRIORITY   7
