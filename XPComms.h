#ifndef XPComms_h
#define XPComms_h

#include "Arduino.h"

void sendXP_Cmd(String command);
void sendXP_Dref(float value, String dataref);

#endif
