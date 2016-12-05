
#include "Arduino.h"
#include "ADF.h"
#include "XPComms.h"
#include <ClickEncoder.h>

#include <Key.h>
#include <Keypad.h>


ADF::ADF( int ADF_ENC_PINA,  int ADF_ENC_PINB,    
            int ADF_VOL_POT_PIN,
            char keyChar_FreqRes, 
            char keyChar_ADF, char keyChar_BFO,
            char keyChar_FRQ, char keyChar_FLT,
            char keyChar_RST 
                 ) {
    _ADF_VOL_POT_PIN = ADF_VOL_POT_PIN;

    ADF_Frequ_Encoder = new ClickEncoder(ADF_ENC_PINA, ADF_ENC_PINB,5, 4);
    
    _keyChar_FreqRes = keyChar_FreqRes ;
    _keyChar_ADF = keyChar_ADF;
    _keyChar_BFO = keyChar_BFO;
    _keyChar_FRQ = keyChar_FRQ;
    _keyChar_FLT = keyChar_FLT;
    _keyChar_RST = keyChar_RST;  
}

void ADF::service(){
  ADF_Frequ_Encoder->service();
}

void ADF::refreshKeys(Keypad keypad){
  //Serial.print("DME refresh keys");
  if (keypad.isPressed(_keyChar_FreqRes)){
      _ADF_FrequRes = !_ADF_FrequRes; 
  }
  if (keypad.isPressed(_keyChar_FRQ)){
      _flip_ADF_SBY = true;
  }
  if (keypad.isPressed(_keyChar_ADF)){
    _flip_ANT_ADF_mode = true;
  }
  if (keypad.isPressed(_keyChar_BFO)){
    _flip_BFO_mode = true;
  }
  if (keypad.isPressed(_keyChar_FLT)){
    _FLT_ET_pressed = true;
  }
}

void ADF::refresh(){
  this->updateADF_Volume();

  if (_flip_ADF_SBY == true) {
    _flip_ADF_SBY = false;
    if (_timer_mode > 0) { // if the timer is displayed then we need to hide it
      _timer_mode = 0;
      sendXP_Cmd("ST/time/timer_mode_0");
    } else { // otherwise we were in FRQ mode and need to swap the standby and active 
      sendXP_Cmd("sim/radios/adf1_standy_flip");
    }
  }
  if (_FLT_ET_pressed == true) {
    _FLT_ET_pressed = false;
    if (_timer_mode == 0){
      _timer_mode = 1;
      sendXP_Cmd("ST/time/timer_mode_1");
    }else if (_timer_mode == 1){
      _timer_mode = 2;
      sendXP_Cmd("ST/time/timer_mode_2");
    } else if (_timer_mode == 2){
      _timer_mode = 1;
      sendXP_Cmd("ST/time/timer_mode_1");  
    }
  }
  
  if (_flip_BFO_mode == true) {
    _flip_BFO_mode = false;
    if (_powerStatus ==2) {
      _powerStatus = 3;
      sendXP_Cmd("sim/radios/adf1_power_mode_3");
    } else if (_powerStatus ==3) {
      _powerStatus = 2;
      sendXP_Cmd("sim/radios/adf1_power_mode_2");
    }
  }
  if (_flip_ANT_ADF_mode == true) {
    _flip_ANT_ADF_mode = false;
    if (_powerStatus == 2 || _powerStatus == 3 ) {
      sendXP_Cmd("sim/radios/adf1_power_mode_1");
      _powerStatus = 1;
    } else if (_powerStatus == 1) {
      sendXP_Cmd("sim/radios/adf1_power_mode_2");
      _powerStatus = 2;
    }
    
  }
  
  // Read ADF Encoder
  ADF_Enc_Current += ADF_Frequ_Encoder->getValue();
  if (ADF_Enc_Current > ADF_Enc_Last) {
    if(_ADF_FrequRes == false){
      sendXP_Cmd("sim/radios/stby_adf1_ones_tens_up");
    } else {
      sendXP_Cmd("sim/radios/stby_adf1_hundreds_thous_up");
    }
    ADF_Enc_Last = ADF_Enc_Current;
  } 
  if (ADF_Enc_Current < ADF_Enc_Last) {
    if(_ADF_FrequRes == false){
      sendXP_Cmd("sim/radios/stby_adf1_ones_tens_down");
    } else {
      sendXP_Cmd("sim/radios/stby_adf1_hundreds_thous_down");
    }
    ADF_Enc_Last = ADF_Enc_Current;
  } 
}

void ADF::updateADF_Volume () {
    
    float ADF_Volume = 1.0 - (float)analogRead(_ADF_VOL_POT_PIN)/1023.0; 
    
    if(abs(ADF_Volume - _currentADFVol)>0.01) {
      if(ADF_Volume < 0.01) {
        sendXP_Cmd("sim/radios/adf1_power_mode_0");
        _powerStatus = 0;
      } 
      if (ADF_Volume >= 0.01 && _powerStatus == 0) {
        sendXP_Cmd("sim/radios/adf1_power_mode_2");
        _powerStatus = 2;
      }
      sendXP_Dref(ADF_Volume, "sim/cockpit2/radios/actuators/audio_volume_adf1[0]");
      _currentADFVol = ADF_Volume;
      //Serial.print("ADF analog read :"); Serial.println(ADF_Volume); 
    
    }
}
