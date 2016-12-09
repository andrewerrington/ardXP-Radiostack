
#include "Arduino.h"
#include "XPDR.h"
#include "XPComms.h"
#include <ClickEncoder.h>

#include <Key.h>
#include <Keypad.h>


XPDR::XPDR( int XPDR_THSDS_ENC_PINA,  int XPDR_THSDS_ENC_PINB,
            int XPDR_HUNDS_ENC_PINA,  int XPDR_HUNDS_ENC_PINB,
            int XPDR_TENS_ENC_PINA,  int XPDR_TENS_ENC_PINB,
            int XPDR_ONES_ENC_PINA,  int XPDR_ONES_ENC_PINB,
            char keyChar_IDT, char keyChar_VFR, 
            char keyChar_OFF, char keyChar_SBY, char keyChar_TST, char keyChar_GND, char keyChar_ON, char keyChar_ALT
            ) {
   
    XPDR_THSDS_ENC = new ClickEncoder(XPDR_THSDS_ENC_PINA, XPDR_THSDS_ENC_PINB,5, 4);
    XPDR_HUNDS_ENC = new ClickEncoder(XPDR_HUNDS_ENC_PINA, XPDR_HUNDS_ENC_PINB,5, 4);
    XPDR_TENS_ENC = new ClickEncoder(XPDR_TENS_ENC_PINA, XPDR_TENS_ENC_PINB,5, 4);
    XPDR_ONES_ENC = new ClickEncoder(XPDR_ONES_ENC_PINA, XPDR_ONES_ENC_PINB,5, 4);

    _keyChar_IDT  = keyChar_IDT;
    _keyChar_VFR  = keyChar_VFR;
    _keyChar_OFF  = keyChar_OFF;
    _keyChar_SBY  = keyChar_SBY;
    _keyChar_TST  = keyChar_TST;
    _keyChar_GND  = keyChar_GND;
    _keyChar_ON   = keyChar_ON;
    _keyChar_ALT  = keyChar_ALT;
}

void XPDR::service(){
  XPDR_THSDS_ENC->service(); 
  XPDR_HUNDS_ENC->service(); 
  XPDR_TENS_ENC->service(); 
  XPDR_ONES_ENC->service(); 
}

void XPDR::refreshKeys(Keypad keypad){
  if (keypad.isPressed(_keyChar_IDT)){
      _IDT_pressed = true; 
  }
  if (keypad.isPressed(_keyChar_VFR)){
      _VFR_pressed = true; 
  }
  //int _transponder_mode = 0; //Transponder mode (off=0,stdby=1,on=2,test=3) ALT = 4
    
  if (keypad.isPressed(_keyChar_OFF)){
      _OFF_pressed = true;  
  }
  if (keypad.isPressed(_keyChar_SBY)){
      _SBY_pressed = true;  
  }
  if (keypad.isPressed(_keyChar_TST)){
      _TST_pressed = true;  
  }
  if (keypad.isPressed(_keyChar_GND)){
      _GND_pressed = true;  
  }
  if (keypad.isPressed(_keyChar_ON)){
      _ON_pressed = true;  
  }
  if (keypad.isPressed(_keyChar_ALT)){
      _ALT_pressed = true;  
  }
}

void XPDR::refresh(){
  if (_OFF_pressed == true) {
    _OFF_pressed = false;
    //sendXP_Cmd("sim/transponder/transponder_off");
    sendXP_Dref(0.0, "sim/cockpit/radios/transponder_mode[0]");
  } else if(_SBY_pressed == true) {
    _SBY_pressed = false;
    //sendXP_Cmd("sim/transponder/transponder_standby");
    sendXP_Dref(1.0, "sim/cockpit/radios/transponder_mode[0]");
  } else if(_TST_pressed == true) {
    _TST_pressed = false;
    sendXP_Dref(3.0, "sim/cockpit/radios/transponder_mode[0]");
    //sendXP_Cmd("sim/transponder/transponder_test");
  } else if(_GND_pressed == true) {
    _GND_pressed = false;
    sendXP_Dref(4.0, "sim/cockpit/radios/transponder_mode[0]");
    //sendXP_Cmd("sim/transponder/transponder_ground");
  } else if(_ON_pressed == true) {
    _ON_pressed = false;
    sendXP_Dref(2.0, "sim/cockpit/radios/transponder_mode[0]");
    //sendXP_Cmd("sim/transponder/transponder_on");
  } else if(_ALT_pressed == true) {
    _ALT_pressed = false;
    sendXP_Dref(5.0, "sim/cockpit/radios/transponder_mode[0]");
    //sendXP_Cmd("sim/transponder/transponder_alt");
  }

  if (_VFR_pressed == true) {
    _VFR_pressed = false;
    sendXP_Dref(1200.0, "sim/cockpit/radios/transponder_code[0]");
  }
  if (_IDT_pressed == true) {
    _IDT_pressed = false;
    sendXP_Cmd("sim/transponder/transponder_ident");
  }
  
  // Read Encoders
  XPDR_THSDS_ENC_Current += XPDR_THSDS_ENC->getValue();
  if (XPDR_THSDS_ENC_Current > XPDR_THSDS_ENC_Last) {
    sendXP_Cmd("sim/transponder/transponder_thousands_up");
    XPDR_THSDS_ENC_Current = XPDR_THSDS_ENC_Last;
  } 
  if (XPDR_THSDS_ENC_Current < XPDR_THSDS_ENC_Last) {
    sendXP_Cmd("sim/transponder/transponder_thousands_down");
    XPDR_THSDS_ENC_Current = XPDR_THSDS_ENC_Last;
  } 
  
  XPDR_HUNDS_ENC_Current += XPDR_HUNDS_ENC->getValue();
  if (XPDR_HUNDS_ENC_Current > XPDR_HUNDS_ENC_Last) {
    sendXP_Cmd("sim/transponder/transponder_hundreds_up");
    XPDR_HUNDS_ENC_Current = XPDR_HUNDS_ENC_Last;
  } 
  if (XPDR_HUNDS_ENC_Current < XPDR_HUNDS_ENC_Last) {
    sendXP_Cmd("sim/transponder/transponder_hundreds_down");
    XPDR_HUNDS_ENC_Current = XPDR_HUNDS_ENC_Last;
  } 

  XPDR_TENS_ENC_Current += XPDR_TENS_ENC->getValue();
  if (XPDR_TENS_ENC_Current > XPDR_TENS_ENC_Last) {
    sendXP_Cmd("sim/transponder/transponder_tens_up");
    XPDR_TENS_ENC_Current = XPDR_TENS_ENC_Last;
  } 
  if (XPDR_TENS_ENC_Current < XPDR_TENS_ENC_Last) {
    sendXP_Cmd("sim/transponder/transponder_tens_down");
    XPDR_TENS_ENC_Current = XPDR_TENS_ENC_Last;
  } 

  XPDR_ONES_ENC_Current += XPDR_ONES_ENC->getValue();
  if (XPDR_ONES_ENC_Current > XPDR_ONES_ENC_Last) {
    sendXP_Cmd("sim/transponder/transponder_ones_up");
    XPDR_ONES_ENC_Current = XPDR_ONES_ENC_Last;
  } 
  if (XPDR_ONES_ENC_Current < XPDR_ONES_ENC_Last) {
    sendXP_Cmd("sim/transponder/transponder_ones_down");
    XPDR_ONES_ENC_Current = XPDR_ONES_ENC_Last;
  } 
}

