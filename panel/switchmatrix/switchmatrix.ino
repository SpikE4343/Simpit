
//Pin connected to latch pin (ST_CP) of 74HC595
const int latchPin = 5;
//Pin connected to clock pin (SH_CP) of 74HC595
const int clockPin = 6;
////Pin connected to Data in (DS) of 74HC595
const int dataPin = 4;

const int col[2] = {2,3};

void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  
  pinMode( col[0], INPUT );
  pinMode( col[1], INPUT );
  
  Serial.begin(9600);
  Serial.println("reset");
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

int width = 4;
int height = 2;

char buf[16];

void loop() {
  // put your main code here, to run repeatedly:
  for( int w = 0; w < width; ++w )
  {
    //digitalWrite( 7, HIGH );
    registerWrite( w+1, HIGH );
    for( int h = 0; h < height; ++h )
    {
      //Serial.println( w + h );
      if( digitalRead( col[h] ) )
      {
        delay( 1000 / 60 / ( width * height ) );
        sprintf( buf, "+[%d, %d]", w, h );
        Serial.println( buf );   
      }
    }
  }
}
