#ifndef XPDR_h
#define XPDR_h

#include "Arduino.h"
#include "XPComms.h"
#include <ClickEncoder.h>
#include <Key.h>
#include <Keypad.h>

class XPDR
{
  public:
    XPDR(   int XPDR_THSDS_ENC_PINA,  int XPDR_THSDS_ENC_PINB,
            int XPDR_HUNDS_ENC_PINA,  int XPDR_HUNDS_ENC_PINB,
            int XPDR_TENS_ENC_PINA,  int XPDR_TENS_ENC_PINB,
            int XPDR_ONES_ENC_PINA,  int XPDR_ONES_ENC_PINB,
            char keyChar_IDT, char keyChar_VFR, 
            char keyChar_OFF, char keyChar_SBY, char keyChar_TST, char keyChar_GND, char keyChar_ON, char keyChar_ALT
            );
    void service();
    void refreshKeys(Keypad keypad);
    void refresh();

  private:
    int _transponder_mode = 0; //Transponder mode (off=0,stdby=1,on=2,test=3) ALT = 4
    bool _IDT_pressed = false;
    bool _VFR_pressed = false;
    bool _OFF_pressed = false;
    bool _SBY_pressed = false;
    bool _TST_pressed = false;
    bool _GND_pressed = false;
    bool _ON_pressed = false;
    bool _ALT_pressed = false;
        
    ClickEncoder *XPDR_THSDS_ENC;
    int16_t XPDR_THSDS_ENC_Current = -1;
    int16_t XPDR_THSDS_ENC_Last = -1;
    
    ClickEncoder *XPDR_HUNDS_ENC;
    int16_t XPDR_HUNDS_ENC_Current = -1;
    int16_t XPDR_HUNDS_ENC_Last = -1;

    ClickEncoder *XPDR_TENS_ENC;
    int16_t XPDR_TENS_ENC_Current = -1;
    int16_t XPDR_TENS_ENC_Last = -1;

    ClickEncoder *XPDR_ONES_ENC;
    int16_t XPDR_ONES_ENC_Current = -1;
    int16_t XPDR_ONES_ENC_Last = -1;
    
    char _keyChar_IDT;
    char _keyChar_VFR;
    char _keyChar_OFF;
    char _keyChar_SBY;
    char _keyChar_TST;
    char _keyChar_GND;
    char _keyChar_ON;
    char _keyChar_ALT;
    
};

#endif
