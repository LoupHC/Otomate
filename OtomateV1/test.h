

const char* const	programsList123[] PROGMEM= {timer3_s, timer2_s,timer1_s};
const char* const	programsList12[] PROGMEM= {timer3_s, timer2_s,btimer1_s};
const char* const	programsList13[] PROGMEM= {timer3_s, btimer2_s,timer1_s};
const char* const	programsList23[] PROGMEM= {btimer3_s, timer2_s,timer1_s};
const char* const	programsList1[] PROGMEM= {timer3_s, btimer2_s,btimer1_s};
const char* const	programsList2[] PROGMEM= {btimer3_s, timer2_s,btimer1_s};
const char* const	programsList3[] PROGMEM= {btimer3_s, btimer2_s,timer1_s};
const char* const	programsList[] PROGMEM= {btimer3_s, btimer2_s,btimer1_s};

const int sizeOfProgramsList = sizeof(programsList)/sizeof(programsList[0]);
byte lastSelectedElementInProgramsList = 3;


boolean clockOvSequence(Rollup rollup, boolean index1, boolean index2,boolean index3){
  if((rollup.isEnabled(CLOCKOV1) == index1)&&(rollup.isEnabled(CLOCKOV2) == index2)&&(rollup.isEnabled(CLOCKOV3) == index3)){
    return true;
  }
  else{
    return false;
  }
}
boolean clockOvSequence(Device device, boolean index1, boolean index2,boolean index3){
  if((device.isEnabled(CLOCKOV1) == index1)&&(device.isEnabled(CLOCKOV2) == index2)&&(device.isEnabled(CLOCKOV3) == index3)){
    return true;
  }
  else{
    return false;
  }
}

if(clockOvSequence(greenhouse.rollup[nb], 1,1,1)){
  confirmVariable(lastSelectedElementInProgramsList, programsList123, sizeOfProgramsList);
}
else if(clockOvSequence(greenhouse.rollup[nb], 1,1,0)){
  confirmVariable(lastSelectedElementInProgramsList, programsList12, sizeOfProgramsList);
}
else if(clockOvSequence(greenhouse.rollup[nb], 1,0,1)){
  confirmVariable(lastSelectedElementInProgramsList, programsList13, sizeOfProgramsList);
}
else if(clockOvSequence(greenhouse.rollup[nb], 0,1,1)){
  confirmVariable(lastSelectedElementInProgramsList, programsList23, sizeOfProgramsList);
}
else if(clockOvSequence(greenhouse.rollup[nb], 1,0,0)){
  confirmVariable(lastSelectedElementInProgramsList, programsList1, sizeOfProgramsList);
}
else if(clockOvSequence(greenhouse.rollup[nb], 0,1,0)){
  confirmVariable(lastSelectedElementInProgramsList, programsList2, sizeOfProgramsList);
}
else if(clockOvSequence(greenhouse.rollup[nb], 0,0,1)){
  confirmVariable(lastSelectedElementInProgramsList, programsList3, sizeOfProgramsList);
}
else{
  confirmVariable(lastSelectedElementInProgramsList, programsList, sizeOfProgramsList);
}
