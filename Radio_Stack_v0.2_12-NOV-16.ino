/*

 */
#define WITHOUT_BUTTON 1

#include <Key.h>
#include <Keypad.h>
#include <SPI.h>         
#include <Wire.h>
#include <ClickEncoder.h>
#include <TimerOne.h>
#include "XPComms.h"

#include "NAVCOMM.h"
#include "DME.h"
#include "ADF.h"
#include "XPDR.h"

//int COMM1_VOL_POT_PIN = 0;
//int NAV1_VOL_POT_PIN = 1;

NAVCOMM *NAVCOMM1;
NAVCOMM *NAVCOMM2;
DME *KN6X_DME;
ADF *KR87_ADF;
XPDR *KT70_XPDR;

const byte ROWS = 8; //four rows
const byte COLS = 8; //three columns
char keys[ROWS][COLS] = {
  {'0','1','2','3','4','5','6','7'},
  {'8','9','q','w','e','r','t','y'},
  {'u','i','o','p','a','s','d','f'},
  {'g','h','<','j','k','l','z','x'},
  {'c','v','b','n','m','A','B','C'},
  {'D','E','F','G','H','I','J','K'},
  {'L','M','N','O','P','Q','R','S'},
  {'T','U','V','W','X','Y','Z','*'}
};

byte rowPins[ROWS] = {49, 48, 47, 46, 45, 44, 43}; //connect to the row pinouts of the keypad
byte colPins[COLS] = { 2, 3, 4, 5, 6, 7, 8, 9}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void timerIsr() {
  NAVCOMM1->service();
  NAVCOMM2->service();
  KR87_ADF->service();
  KT70_XPDR->service();
}


//****************************************************************
//
//              SETUP
//
//****************************************************************

void setup()  { 
    Serial.begin(115200); 
    Serial.println("Starting... ");

    
    NAVCOMM1 = new NAVCOMM("1",
                          22, 23, 
                          25, 24,
                          0,1,
                          '2', '5',
                          '3', '6',
                          '1','4');
                          
    NAVCOMM2 = new NAVCOMM("2",
                          29, 28, 
                          27, 26,
                          4,5,
                          'w', 'q',
                          'r', '9',
                          '8','e');
                          
    KN6X_DME = new DME(3, 
                       'x', 'z' 
                      );
                      
    KR87_ADF = new ADF(39,  38,    
            2,
            'p', 
            's', 'u',
            'o', 'd',
            'a');
            
    KT70_XPDR = new XPDR( 30,31,
                          32,33,
                          34,35,
                          36,37,
                          'i','0',
                          'g', '<', 'k', 'j', 'l', 'h');

    /*  int XPDR_THSDS_ENC_PINA,  int XPDR_THSDS_ENC_PINB,
            int XPDR_HUNDS_ENC_PINA,  int XPDR_HUNDS_ENC_PINB,
            int XPDR_TENS_ENC_PINA,  int XPDR_TENS_ENC_PINB,
            int XPDR_ONES_ENC_PINA,  int XPDR_ONES_ENC_PINB,
            char keyChar_IDT, char keyChar_VFR, 
            char keyChar_OFF, char keyChar_SBY, char keyChar_TST, char keyChar_GND, char keyChar_ON, char keyChar_ALT
    */
   /* String NAVCOMM_nr,
                 int COMM_ENC_PINA,     int COMM_ENC_PINB,
                 int NAV_ENC_PINA,      int NAV_ENC_PINB,
                 int COMM_VOL_POT_PIN,  int NAV_VOL_POT_PIN, 
                 char keyChar_COMM_stdbyFlip,   char keyChar_NAV_stdbyFlip,
                 char keyChar_COMM_tuningRes,   char keyChar_NAV_tuningRes,
                 char keyChar_CHAN,             char keyChar_MODE
                 ) {
    */
    Timer1.initialize(1000);
    Timer1.attachInterrupt(timerIsr); 
    
    Serial.println("Set up complete... ");
    Serial.println("");
    delay(2000);
  
} 

//****************************************************************
//
//              LOOP
//
//****************************************************************
void loop() 
{   
  //Serial.println("looping....");
 
  NAVCOMM1->refresh();
  NAVCOMM2->refresh();
  KN6X_DME->refresh();
  KR87_ADF->refresh();
  KT70_XPDR->refresh();
  /*
  char key = keypad.getKey();
  if (key != NO_KEY){
    Serial.println(key);
  }
  */
  bool keyspressed = keypad.getKeys();
  if (keyspressed == true) {
    NAVCOMM1->refreshKeys(keypad);
    NAVCOMM2->refreshKeys(keypad);
    KN6X_DME->refreshKeys(keypad);
    KR87_ADF->refreshKeys(keypad);
    KT70_XPDR->refreshKeys(keypad);
    
    Serial.println("Keys pressed:");
    Serial.print(keypad.key[0].kchar); Serial.print(" ; ");
    Serial.print(keypad.key[1].kchar);Serial.print(" ; ");
    Serial.print(keypad.key[2].kchar);Serial.print(" ; ");
    Serial.print(keypad.key[3].kchar);Serial.print(" ; ");
    Serial.print(keypad.key[4].kchar);Serial.print(" ; ");
    Serial.print(keypad.key[5].kchar);Serial.print(" ; ");
    Serial.print(keypad.key[6].kchar);Serial.print(" ; ");
    Serial.println(keypad.key[7].kchar);
    
  }
   
} 






