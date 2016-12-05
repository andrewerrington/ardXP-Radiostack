#ifndef DME_h
#define DME_h

#include "Arduino.h"
#include "XPComms.h"
#include <Key.h>
#include <Keypad.h>

class DME
{
  public:
    DME(    int DME_VOL_POT_PIN, 
            char keyChar_DME1, char keyChar_DME2 
            );
    void service();
    void updateDME_Volume();
    void refreshKeys(Keypad keypad);
    void refresh();

  private:
    bool _DME_power = false;
    
    int _DME_VOL_POT_PIN;
    float _currentDMEVol;
    
    char _keyChar_DME1;
    char _keyChar_DME2;
    bool _currentDMESource = false; // false for DME1, true for DME2
    bool _DMESourceChanged = false;
    
};

#endif
