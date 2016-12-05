#ifndef ADF_h
#define ADF_h

#include "Arduino.h"
#include "XPComms.h"
#include <ClickEncoder.h>
#include <Key.h>
#include <Keypad.h>

class ADF
{
  public:
    ADF(    int ADF_ENC_PINA,  int ADF_ENC_PINB,    
            int ADF_VOL_POT_PIN,
            char keyChar_FreqRes, 
            char keyChar_ADF, char keyChar_BFO,
            char keyChar_FRQ, char keyChar_FLT,
            char keyChar_RST
            );
    void service();
    void updateADF_Volume();
    void refreshKeys(Keypad keypad);
    void refresh();

  private:
    bool _flip_ANT_ADF_mode = false;
    int _powerStatus = 0; //0 = off, 1 = antenna, 2 = on, 3 = tone, 4 = test
    bool _flip_ADF_SBY = false;
    bool _flip_BFO_mode = false;
    
    ClickEncoder *ADF_Frequ_Encoder;
    int16_t ADF_Enc_Current = -1;
    int16_t ADF_Enc_Last = -1;
    bool _ADF_FrequRes = false;
    
    int _ADF_VOL_POT_PIN;
    float _currentADFVol = 0.0;
    
    char _keyChar_FreqRes;
    char _keyChar_ADF;
    char _keyChar_BFO;
    char _keyChar_FRQ;
    char _keyChar_FLT;
    char _keyChar_RST;
    
};

#endif
