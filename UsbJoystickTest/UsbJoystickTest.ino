// Used to control an input panel an look like a joystick
// via HID to the operating system
#include "Joystick.h"

//Pin connected to latch pin (ST_CP) of 74HC595
#define latchPin 5
//Pin connected to clock pin (SH_CP) of 74HC595
#define clockPin 6
////Pin connected to Data in (DS) of 74HC595
#define dataPin 4

#define width 4
#define height 2
const int col[2] = {2, 3};

#define RELEASE_DELAY_START 1
#define RELEASE_DELAY 10

bool switchState[ width * height ];
char shouldRelease[ width * height];

char buf[16];

void resetSwitchStates()
{
  memset( switchState, 0, sizeof( switchState ) );
  memset( shouldRelease, 0, sizeof( shouldRelease ) );
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
  int button = (r * width + c) + 1;

  bool state = switchState[button];
  bool changed = input != state;

  if (changed)
  {
    switchState[button] = input;
    shouldRelease[button] = RELEASE_DELAY_START;

    Joystick.pressButton(button);

//    Serial.print('p');
//    Serial.print(state);
//    Serial.print(',');
//    Serial.print( input);
//    Serial.print(",[");
//    Serial.print(c);
//    Serial.print(',');
//    Serial.print(r);
//    Serial.print("],");
//    Serial.println(button);
//
    return true;
  }

  // wait 5 iterations
  if( shouldRelease[button] > 0 )
  {
    if ( ++shouldRelease[button] > RELEASE_DELAY  )
    {
      shouldRelease[button] = 0;
      
      Joystick.releaseButton(button);
      
  //    Serial.print('r');
  //    sprintf( buf, "%X", state );
  //    Serial.print(buf);
  //    Serial.println(button);
  
      return true;
    }
  }

//  delay(100);
  return false;
}

// scan all column/rows and update switch states
//
bool updateInputs()
{
  bool sendState = false;

  // put your main code here, to run repeatedly:
  for ( int w = 0; w < width; ++w )
  {
    //digitalWrite( 7, HIGH );
    registerWrite( w + 1, HIGH );

//    if ( w == 0 )
//      Serial.println("====================");
//    Serial.print(w);
//    Serial.println(" ---");

    delay(2);
    for ( int h = 0; h < height; ++h )
    {
      //digitalWrite(col[h], LOW);
      pinMode( col[h], INPUT );
      //Serial.println( w + h );
      bool set = (bool)digitalRead( col[h] );

      //pinMode( col[h], OUTPUT );
      //digitalWrite(col[h], HIGH);

      sendState |= updateInput( w, h, set );
      //delay(10);
    }
  }

  return sendState;
}

void loop() {

  bool sendUpdate = updateInputs();
  if ( sendUpdate)
  {
    Joystick.sendState();
  }
}

