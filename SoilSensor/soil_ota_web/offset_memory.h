#include <EEPROM.h>
uint8_t addMOI = 160, addCON = 320, addPH = 0, addNIT = 40, addPHO = 80, addPOT = 120, addNIT2 = 200, addPHO2 = 240, addPOT2 = 280;
float off_valMOI = 0, off_valCON = 0, off_valPH = 0, off_valNIT = 0, off_valPHO = 0, off_valPOT = 0, off_valNIT2 = 0, off_valPHO2 = 0, off_valPOT2 = 0;

void store_offsetMOI(float valMOI){
  EEPROM.put(addMOI,valMOI); // Writes the value to EEPROM
  EEPROM.commit();  
}
void store_offsetCON(float valCON){
  EEPROM.put(addCON,valCON); // Writes the value to EEPROM
  EEPROM.commit();  
}
void store_offsetPH(float valPH){
  EEPROM.put(addPH,valPH); // Writes the value to EEPROM
  EEPROM.commit();  
}

void store_offsetNIT(float off_valNIT){
  EEPROM.put(addNIT,off_valNIT); // Writes the value to EEPROM
  EEPROM.commit();  
}

void store_offsetPHO(float off_valPHO){
  EEPROM.put(addPHO,off_valPHO); // Writes the value to EEPROM
  EEPROM.commit();  
}

void store_offsetPOT(float off_valPOT){
  EEPROM.put(addPOT,off_valPOT); // Writes the value to EEPROM
  EEPROM.commit();  
}


void read_offset(){
  off_valMOI = 0;
  off_valCON = 0;
  off_valPH = 0;
  off_valNIT = 0;
  off_valPHO = 0;
  off_valPOT = 0;
  EEPROM.get(addMOI,off_valMOI);
  EEPROM.get(addPH,off_valCON);
  EEPROM.get(addPH,off_valPH);
  EEPROM.get(addNIT,off_valNIT);
  EEPROM.get(addPHO,off_valPHO);
  EEPROM.get(addPOT,off_valPOT);
}

void store_offsetNIT2(float off_valNIT2){
  EEPROM.put(addNIT2,off_valNIT2); // Writes the value to EEPROM
  EEPROM.commit();  
}

void store_offsetPHO2(float off_valPHO2){
  EEPROM.put(addPHO2,off_valPHO2); // Writes the value to EEPROM
  EEPROM.commit();  
}

void store_offsetPOT2(float off_valPOT2){
  EEPROM.put(addPOT2,off_valPOT2); // Writes the value to EEPROM
  EEPROM.commit();  
}


void read_offset2(){
  off_valNIT2 = 0;
  off_valPHO2 = 0;
  off_valPOT2 = 0;
  EEPROM.get(addNIT2,off_valNIT2);
  EEPROM.get(addPHO2,off_valPHO2);
  EEPROM.get(addPOT2,off_valPOT2);
}

void setupEEPROM(){
  EEPROM.begin(4096);
  for(int i=0; i<100; i++){
  }
}
