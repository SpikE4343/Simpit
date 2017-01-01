// Program used to test the USB Joystick object on the 
// Arduino Leonardo or Arduino Micro.
//
// Matthew Heironimus
// 2015-03-28
// Updated on 2015-11-18 to use the new Joystick library written for version 1.6.6.
//------------------------------------------------------------

#include "Joystick.h"

//Pin connected to latch pin (ST_CP) of 74HC595
#define latchPin 5
//Pin connected to clock pin (SH_CP) of 74HC595
#define clockPin 6
////Pin connected to Data in (DS) of 74HC595
#define dataPin 4

#define width 4
#define height 2
const int col[2] = {2,3};  

bool switchState[ width ][ height ];

#define SWITCH_OFF 0
#define SWITCH_ON 1 << 0
#define SWTICH_SHOULD_RELEASE 1 << 1

void resetSwitchStates()
{
  memset( switchState, SWITCH_OFF, sizeof( switchState ) );
}

void setShouldRelease( int c, int r, bool enable=true )
{
  if( enable )
    switchState[c][r] |= SWTICH_SHOULD_RELEASE;
  else
    switchState[c][r] &= ~SWTICH_SHOULD_RELEASE;
}

void setSwitchActive( int c, int r, bool active=true )
{
  if( active )
    switchState[c][r] |= SWITCH_ON;
  else
    switchState[c][r] &= ~SWITCH_ON;
}


void registerWrite(int whichPin, int whichState) 
{
  // the bits you want to send
  byte bitsToSend = 0;

  // turn off the output so the pins don't light up
  // while you're shifting bits:
  digitalWrite(latchPin, LOW);

  // turn on the next highest bit in bitsToSend:
  bitWrite(bitsToSend, whichPin, whichState);

  // shift the bits out:
  shiftOut(dataPin, clockPin, MSBFIRST, bitsToSend);

    // turn on the output so the LEDs can light up:
  digitalWrite(latchPin, HIGH);

}

void setup() 
{ 
  Joystick.begin();
  
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);

  // input columns
  pinMode( col[0], OUTPUT );
  pinMode( col[1], OUTPUT );

  // debug/configuration serial 
  Serial.begin(9600);
  Serial.println("reset");

  resetSwitchStates();
}

// update the status of a single switch
// converts switch throw to momentary button press
//
bool updateInput( int c, int r, bool input )
{
  bool state = switchState[c][r] & SWITCH_ON;
  bool shouldRelease = switchState[c][r] & SWTICH_SHOULD_RELEASE;

  int button = (r*width+c)+1;
  bool changed = input != state;

  if (changed)
  {
    setSwitchActive( c, r, input );
    Joystick.pressButton(button);
    setShouldRelease(c,r);

    Serial.print('p');
    Serial.println(button);
    
    return true;
  }
  
  if( shouldRelease )
  {
    Joystick.releaseButton(button);
    setShouldRelease(c,r, false);
    
    Serial.print('r');
    Serial.println(button);
    
    return true;
  }

  return false;
}

// scan all column/rows and update switch states
//
bool updateInputs()
{
  bool sendState = false;
  
   // put your main code here, to run repeatedly:
  for( int w = 0; w < width; ++w )
  {
    //digitalWrite( 7, HIGH );
    registerWrite( w+1, HIGH );
    for( int h = 0; h < height; ++h )
    {
      digitalWrite(col[h], LOW);
      pinMode( col[h], INPUT_PULLUP );
      //Serial.println( w + h );
      bool set = digitalRead( col[h] );

      pinMode( col[h], OUTPUT );
      digitalWrite(col[h], HIGH);
      
      sendState |= updateInput( w, h, set );
      delay(100);
    }
  }

  return sendState;
}

void loop() {

  bool sendUpdate = updateInputs();
  if( sendUpdate) 
  {
    Joystick.sendState();
  }
}

