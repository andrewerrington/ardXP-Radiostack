
#include "Arduino.h"
#include "NAVCOMM.h"
#include <ClickEncoder.h>
#include "XPComms.h"

#include <Key.h>
#include <Keypad.h>

NAVCOMM::NAVCOMM(String NAVCOMM_nr,
                 int COMM_ENC_PINA,     int COMM_ENC_PINB,
                 int NAV_ENC_PINA,      int NAV_ENC_PINB,
                 int COMM_VOL_POT_PIN,  int NAV_VOL_POT_PIN, 
                 char keyChar_COMM_stdbyFlip,   char keyChar_NAV_stdbyFlip,
                 char keyChar_COMM_tuningRes,   char keyChar_NAV_tuningRes,
                 char keyChar_CHAN,             char keyChar_MODE
                 ) {
  _NAVCOMM_nr = NAVCOMM_nr;
  
  _COMM_VOL_POT_PIN = COMM_VOL_POT_PIN;
  _NAV_VOL_POT_PIN = NAV_VOL_POT_PIN;
  _currentCOMVol = 0.0;
  _currentNAVVol = 0.0;
  
  COMM_Frequ_Encoder = new ClickEncoder(COMM_ENC_PINA, COMM_ENC_PINB,5, 4);
  NAV_Frequ_Encoder  = new ClickEncoder(NAV_ENC_PINA, NAV_ENC_PINB,5, 4);
  
  _keyChar_COMM_stdbyFlip   = keyChar_COMM_stdbyFlip;
  _keyChar_NAV_stdbyFlip    = keyChar_NAV_stdbyFlip;
  _keyChar_COMM_tuningRes   = keyChar_COMM_tuningRes;
  _keyChar_NAV_tuningRes    = keyChar_NAV_tuningRes;
  _keyChar_CHAN             = keyChar_CHAN;
  _keyChar_MODE             = keyChar_MODE;

  // initialise volume
  this->updateNAVCOMM_Volumes();
}

void NAVCOMM::service(){
  COMM_Frequ_Encoder->service();
  NAV_Frequ_Encoder->service();
}

void NAVCOMM::refreshKeys(Keypad keypad){
  if (keypad.isPressed(_keyChar_COMM_stdbyFlip)){
    _flip_COMM_SBY = true;
  }
  if (keypad.isPressed(_keyChar_NAV_stdbyFlip)){
    _flip_NAV_SBY = true; 
  }
  if (keypad.isPressed(_keyChar_COMM_tuningRes)){
    _COMM_FrequRes = !_COMM_FrequRes; 
  }
  if (keypad.isPressed(_keyChar_NAV_tuningRes)){
    _NAV_FrequRes = !_NAV_FrequRes ;
  }
}

void NAVCOMM::refresh(){
  this->updateNAVCOMM_Volumes();
  //
  if (_flip_COMM_SBY == true) {
    _flip_COMM_SBY = false;
    sendXP_Cmd("sim/radios/com"+_NAVCOMM_nr+"_standy_flip");
  }
  if (_flip_NAV_SBY == true) {
    _flip_NAV_SBY = false;
    sendXP_Cmd("sim/radios/nav"+_NAVCOMM_nr+"_standy_flip");
  }
  
  // Read COMM Encoder
  COMM_Enc_Current += COMM_Frequ_Encoder->getValue();
  //Serial.print("COMM1 encoder value: "); Serial.println(last_COMM1_value); 
  if (COMM_Enc_Current > COMM_Enc_Last) {
    if(_COMM_FrequRes == false){
      sendXP_Cmd("sim/radios/stby_com"+_NAVCOMM_nr+"_coarse_up");
    } else {
      sendXP_Cmd("sim/radios/stby_com"+_NAVCOMM_nr+"_fine_up");
    }
    COMM_Enc_Last = COMM_Enc_Current;
  } 
  if (COMM_Enc_Current < COMM_Enc_Last) {
    if(_COMM_FrequRes == false){
      sendXP_Cmd("sim/radios/stby_com"+_NAVCOMM_nr+"_coarse_down");
    } else {
      sendXP_Cmd("sim/radios/stby_com"+_NAVCOMM_nr+"_fine_down");
    }
    COMM_Enc_Last = COMM_Enc_Current;
  } 

  // Read NAV Encoder
  NAV_Enc_Current += NAV_Frequ_Encoder->getValue();
  //Serial.print("COMM1 encoder value: "); Serial.println(last_COMM1_value); 
  if (NAV_Enc_Current > NAV_Enc_Last) {
    if(_NAV_FrequRes == false){
      sendXP_Cmd("sim/radios/stby_nav"+_NAVCOMM_nr+"_coarse_up");
    } else {
      sendXP_Cmd("sim/radios/stby_nav"+_NAVCOMM_nr+"_fine_up");
    }
    NAV_Enc_Last = NAV_Enc_Current;
  } 
  if (NAV_Enc_Current < NAV_Enc_Last) {
    if(_NAV_FrequRes == false){
      sendXP_Cmd("sim/radios/stby_nav"+_NAVCOMM_nr+"_coarse_down");
    } else {
      sendXP_Cmd("sim/radios/stby_nav"+_NAVCOMM_nr+"_fine_down");
    }
    NAV_Enc_Last = NAV_Enc_Current;
  } 
}

void NAVCOMM::updateNAVCOMM_Volumes () {
    
    float COMM_Volume = (float)analogRead(_COMM_VOL_POT_PIN)/1023.0; 
    float NAV_Volume = (float)analogRead(_NAV_VOL_POT_PIN)/1023.0;

    if(abs(COMM_Volume - _currentCOMVol)>0.02) {
      if(COMM_Volume < 0.02) {
        sendXP_Cmd("sim/radios/power_com"+_NAVCOMM_nr+"_off");
        sendXP_Cmd("sim/radios/power_nav"+_NAVCOMM_nr+"_off");
        _NAVCOMM_power = false;
      } 
      if (COMM_Volume >= 0.02 && _NAVCOMM_power == false) {
        sendXP_Cmd("sim/radios/power_com"+_NAVCOMM_nr+"_on");  
        sendXP_Cmd("sim/radios/power_nav"+_NAVCOMM_nr+"_on");
        _NAVCOMM_power = true;
      }
      sendXP_Dref(COMM_Volume, "sim/cockpit2/radios/actuators/audio_volume_com"+_NAVCOMM_nr+"[0]");
      _currentCOMVol = COMM_Volume;
    }
    if(abs(NAV_Volume - _currentNAVVol)>0.02) {
      sendXP_Dref(NAV_Volume, "sim/cockpit2/radios/actuators/audio_volume_nav"+_NAVCOMM_nr+"[0]");
      _currentNAVVol = NAV_Volume;
    }
    //Serial.print("COMM1 analog read :"); Serial.println(COMM1_Volume); 
    //Serial.print("NAV1 analog read :"); Serial.println(NAV1_Volume); 
}

