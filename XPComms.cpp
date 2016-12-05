
#include "Arduino.h"
#include "XPComms.h"

void sendXP_Cmd(String command){
  Serial.print("CMND0"+command+'\13'+'\10');
}

void sendXP_Dref(float value, String dataref){
  union{ byte bv[4]; float fv;} bf;
  bf.fv = value; 

  /*char DataRef[500]; 
  int n = 0;    
  while (dataref[n]) { DataRef[n] = dataref[n]; n++;}   
  for (int i = n; i < 499; i++) DataRef[i] = char(32); DataRef[500] = char(0);
  */
  Serial.print("DREF0");
  for (int i=0; i<4; i++)  Serial.print((char)bf.bv[i]); 
  Serial.print(dataref+'\13'+'\10');
    
}


