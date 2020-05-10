/*
*******************************************************
*******************CONTROL PARAMETERS******************
*******************************************************

********************Timepoints parameters***************

Timepoints define the greenhouse normal temperature range over time. To set a timepoint, four parameters are asked:
- type (timpoint can be set relatively to sunrise or sunset, or set manually)
- time (if set relatively to sunrise or sunset, the time parameter is a value between -60 minuts and +60 minuts,.
        if set manually, the time parameter is two values : an hour value and a minut value)
- heating temperature (reference temperature for heating devices)
- cooling temperature (reference temperature for cooling devices(rollups included)
- ramping time (in timepoints transitions, how many time (in minuts) before increasing/decreasing cooling/heating temperature of 0.5C)

EXAMPLE 1 (timepoint relative to sunrise):

#define TP1_TYPE        SR
#define TP1_HOUR        0
#define TP1_MN_MOD      -30
#define TP1_HEAT        18
#define TP1_COOL        20
#define TP1_RAMP        15
Timepoint occur 30 minuts before sunrise, heating temperature reference is set to 18C, cooling temperature reference is set to 20C.

EXAMPLE 2 (timepoint relative to sunrise):

#define TP1_TYPE        CLOCK
#define TP1_HOUR        12
#define TP1_MN_MOD      30
#define TP1_HEAT        20
#define TP1_COOL        25
#define TP1_RAMP        15
Timepoint occur at 12h30, heating temperature reference is set to 20C, cooling temperature reference is set to 25C.

********************Rollups parameters***************

Rollup parameters set the general behaviour of the roll-up motors, according to measured temperature and cooling reference temperature
A rollup program splits in two parts : global parameters and stages parameters
- Global parameters are active at all time
- Stages parameters are only active within a short temperature range, defined as "stage" or "cool stage".
  They set the target increment (%) within this temperature range

For global parameters, four parameters are asked:
- hysteresis (tolerated temperature drop before closing)
- rotation up (# of sec before rollup reaches the top)
- rotation down (# of sec before rollup reaches the bottom)
- pause time(pause (in sec) between each motor move)

For each stages parameters (there is usally many stages), two parameters are asked :
- temperature modificator (Adds to cooling reference temperature, defines at which temperature the "cool stage" starts)
- target increments (while in this stage, rollup will move to reach this increment, in % of opening.

EXAMPLE :
#define R1_HYST         1
#define R1_ROTUP        189
#define R1_ROTDOWN      150
#define R1_PAUSE        30

#define R1_S1_MOD       0
#define R1_S1_TARGET    25
#define R1_S2_MOD       1
#define R1_S2_TARGET    50
#define R1_S3_MOD       2
#define R1_S3_TARGET    75
#define R1_S4_MOD       3
#define R1_S4_TARGET    100

Total opening time is 189 seconds, total closing time is 150 seconds. Pause between motor movements is 30 seconds.
Stage 1: At cooling temperature + 0C, rollup will open at 25%. At cooling temperature +0(mod) -1(hysteresis), it will close back to 0%.
Stage 2: At cooling temperature + 1C, rollup will open at 50%. At cooling temperature +1(mod) -1(hysteresis), it will close back to 25%(last stage target target increment).
Stage 3: At cooling temperature + 2C, rollup will open at 75%. At cooling temperature +2(mod) -1(hysteresis), it will close back to 50%(last stage target target increment).
Stage 4: At cooling temperature + 3C, rollup will open at 100%. At cooling temperature +3(mod) -1(hysteresis), it will close back to 75%(last stage target target increment).

If cooling temperature is 24C :
Stage 1: At 24C, rollup will open at 25%. At 23C, it will close back to 0%.
Stage 2: At 25C, rollup will open at 50%. At 24C, it will close back to 25%.
Stage 3: At 26C, rollup will open at 75%. At 25C, it will close back to 50%.
Stage 4: At 27C, rollup will open at 100%. At 25C, it will close back to 75%.


********************Devices/heaters parameters***************

Device parameters set the general behaviour of ON/OFF cooling devices (typically, devices), according to measured temperature and cooling reference temperature
Two parameters are asked :
- hysteresis (tolerated temperature drop before shutting)
- temperature modificator (Adds to cooling reference temperature, defines at which temperature it starts)

Example :
#define F1_MOD          3
#define F1_HYST         1
At cooling reference +3, device will starts.
At cooling reference temperature +3 (mod) -1 (hyst), device will stops.

If cooling reference temperature is 24C :
At 27C, device will starts.
At 26C, device will stops.

Heater parameters set the general behaviour of ON/OFF heating devices (typically, heaters), according to measured temperature and heating reference temperature
- hysteresis (tolerated temperature rise before shutting)
- temperature modificator (Substract to heating reference temperature, defines at which temperature it starts)

EXAMPLE :
Example :
#define H1_MOD          -1
#define H1_HYST         2
At heating reference -1, furnace will start.
At heating reference temperature +-1 (mod) +1 (hyst), furna*************

Create greenhouse object
#define TIMEPOINTS      5          //# of timepoints
#define ROLLUPS         2             //# of rollups
#define STAGES          5             //# of stagesce will stop.

If cooling reference temperature is 18C :
At 17C, furnace will start.
At 19C, furnace  stop.
*/
