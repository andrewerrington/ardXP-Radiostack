#ifndef NAVCOMM_h
#define NAVCOMM_h

#include "Arduino.h"
#include <ClickEncoder.h>
#include "XPComms.h"
#include <Key.h>
#include <Keypad.h>

class NAVCOMM
{
  public:
    NAVCOMM(String NAVCOMM_nr,
            int COMM_ENC_PINA,  int COMM_ENC_PINB,
            int NAV_ENC_PINA,   int NAV_ENC_PINB,
            int COMM_VOL_POT_PIN, int NAV_VOL_POT_PIN,
            char keyChar_COMM_stdbyFlip,   char keyChar_NAV_stdbyFlip,
            char keyChar_COMM_tuningRes,   char keyChar_NAV_tuningRes,
            char keyChar_CHAN,             char keyChar_MODE
            );
    void service();
    void updateNAVCOMM_Volumes();
    void refreshKeys(Keypad keypad);
    void refresh();

  private:
    String _NAVCOMM_nr;
    bool _NAVCOMM_power = false;
    
    int _COMM_VOL_POT_PIN;
    int _NAV_VOL_POT_PIN;
    float _currentCOMVol;
    float _currentNAVVol;
    
    ClickEncoder *COMM_Frequ_Encoder;
    int16_t COMM_Enc_Current = -1;
    int16_t COMM_Enc_Last = -1;

    ClickEncoder *NAV_Frequ_Encoder;
    int16_t NAV_Enc_Current = -1;
    int16_t NAV_Enc_Last = -1;

    char _keyChar_COMM_stdbyFlip;
    char _keyChar_NAV_stdbyFlip;
    char _keyChar_COMM_tuningRes;
    char _keyChar_NAV_tuningRes;
    char _keyChar_CHAN;
    char _keyChar_MODE;

    bool _COMM_FrequRes = false;
    bool _NAV_FrequRes = false;

    bool _flip_COMM_SBY = false;
    bool _flip_NAV_SBY = false;
    
    
};

#endif
