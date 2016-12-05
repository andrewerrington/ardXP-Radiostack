
#include "Arduino.h"
#include "DME.h"
#include "XPComms.h"

#include <Key.h>
#include <Keypad.h>


DME::DME( int DME_VOL_POT_PIN, 
                  char keyChar_DME1, char keyChar_DME2 
                 ) {
  _DME_VOL_POT_PIN = DME_VOL_POT_PIN;
  _keyChar_DME1 = keyChar_DME1;
  _keyChar_DME2 = keyChar_DME2;        

}

void DME::refreshKeys(Keypad keypad){
  //Serial.print("DME refresh keys");
  if (keypad.isPressed(_keyChar_DME1)){
    
   if (_currentDMESource == true){
      _currentDMESource = false;
      _DMESourceChanged = true;
   }
    
  }
  if (keypad.isPressed(_keyChar_DME2)){
    if (_currentDMESource == false){
      _currentDMESource = true;
      _DMESourceChanged = true;
   }
  }
}

void DME::refresh(){
  this->updateDME_Volume();
  //
  if (_DMESourceChanged == true) {
    _DMESourceChanged = false;
    
   if (_currentDMESource == false){ // DME source NAV1
      sendXP_Dref(0.0, "sim/cockpit2/radios/actuators/DME_slave_source[0]");
   } else {
      sendXP_Dref(1.0, "sim/cockpit2/radios/actuators/DME_slave_source[0]");
   }
    //sim/cockpit/switches/DME_radio_selector  int y ??? Which nav radio is the slaved DME connected to
    //sim/cockpit2/radios/actuators/DME_slave_source  int y enum  DME display selection of what NAV radio to display. 0 for Nav1, 1for Nav2.
  }
}

void DME::updateDME_Volume () {
    
    float DME_Volume = (float)analogRead(_DME_VOL_POT_PIN)/1023.0; 
    
    if(abs(DME_Volume - _currentDMEVol)>0.01) {
      if(DME_Volume < 0.01) {
        sendXP_Dref(0, "sim/cockpit2/radios/actuators/dme_power[0]");
        _DME_power = false;
      } 
      if (DME_Volume >= 0.01 && _DME_power == false) {
        sendXP_Dref(1, "sim/cockpit2/radios/actuators/dme_power[0]");
        _DME_power = true;
      }
      sendXP_Dref(DME_Volume, "sim/cockpit2/radios/actuators/audio_volume_dme[0]");
      //sim/cockpit2/radios/actuators/audio_volume_dme  float y [0..1]  Audio level (0 is off, 1 is max) for dme audio
      _currentDMEVol = DME_Volume;
      //Serial.print("DME analog read :"); Serial.println(DME_Volume); 
    
    }
}

