// Program used to test the USB Joystick object on the 
// Arduino Leonardo or Arduino Micro.
//
// Matthew Heironimus
// 2015-03-28
// Updated on 2015-11-18 to use the new Joystick library written for version 1.6.6.
//------------------------------------------------------------

#include "Joystick.h"

bool changed = false;
bool button0 = false;

//Pin connected to latch pin (ST_CP) of 74HC595
const int latchPin = 5;
//Pin connected to clock pin (SH_CP) of 74HC595
const int clockPin = 6;
////Pin connected to Data in (DS) of 74HC595
const int dataPin = 4;

const int col[2] = {2,3};  

bool switchState[ 4 ][ 2 ];

#define SWITCH_OFF 0
#define SWITCH_ON 1 << 0
#define SWTICH_SHOULD_RELEASE 1 << 1
 
void resetSwitchStates()
{
  memset( switchState, 0, sizeof( switchState ) );
}

void setup() {
 
  Joystick.begin();
  
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);

  // input columns
  pinMode( col[0], INPUT );
  pinMode( col[1], INPUT );

  // debug/configuration serial 
  Serial.begin(9600);
  Serial.println("reset");

  resetSwitchStates();
}

bool updateInput( int c, int r )
{
  bool state = digitalRead( 3 ) > 0;

  changed = state != button0;
  button0 = state;

  if (changed)
  {
    Joystick.pressButton(1);
    //Joystick.sendState();
    shouldRelease = true; 
    delay(100);
  }
  else if( shouldRelease )
  {
    Joystick.releaseButton(1);
    Joystick.sendState();
  }
}

bool shouldRelease = false;
void loop() {

  bool state = digitalRead( 3 ) > 0;

  changed = state != button0;
  button0 = state;

  
  
  if (changed)
  {
    Joystick.pressButton(1);
    Joystick.sendState();
    shouldRelease = true;
    delay(100);
  }
  else if( shouldRelease )
  {
    Joystick.releaseButton(1);
    Joystick.sendState();
  }
    
    
 
}

