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

//// Set to true to test "Auto Send" mode or false to test "Manual Send" mode.
////const bool testAutoSendMode = true;
//const bool testAutoSendMode = false;
//
//const unsigned long gcCycleDelta = 1000;
//const unsigned long gcAnalogDelta = 25;
//const unsigned long gcButtonDelta = 500;
//unsigned long gNextTime = 0;
//unsigned int gCurrentStep = 0;
//
//void testSingleButtonPush(unsigned int button)
//{
//  if (button > 0)
//  {
//    Joystick.releaseButton(button - 1);
//  }
//  if (button < 32)
//  {
//    Joystick.pressButton(button);
//  }
//}
//
//void testMultiButtonPush(unsigned int currentStep) 
//{
//  for (int button = 0; button < 32; button++)
//  {
//    if ((currentStep == 0) || (currentStep == 2))
//    {
//      if ((button % 2) == 0)
//      {
//        Joystick.pressButton(button);
//      } else if (currentStep != 2)
//      {
//        Joystick.releaseButton(button);
//      }
//    } // if ((currentStep == 0) || (currentStep == 2))
//    if ((currentStep == 1) || (currentStep == 2))
//    {
//      if ((button % 2) != 0)
//      {
//        Joystick.pressButton(button);
//      } else if (currentStep != 2)
//      {
//        Joystick.releaseButton(button);
//      }
//    } // if ((currentStep == 1) || (currentStep == 2))
//    if (currentStep == 3)
//    {
//      Joystick.releaseButton(button);
//    } // if (currentStep == 3)
//  } // for (int button = 0; button < 32; button++)
//}
//
//void testXYAxis(unsigned int currentStep)
//{
//  if (currentStep < 256)
//  {
//    Joystick.setXAxis(currentStep - 127);
//    Joystick.setYAxis(-127);
//  } 
//  else if (currentStep < 512)
//  {
//    Joystick.setYAxis(currentStep - 256 - 127);
//  }
//  else if (currentStep < 768)
//  {
//    Joystick.setXAxis(128 - (currentStep - 512));
//  }
//  else if (currentStep < 1024)
//  {
//    Joystick.setYAxis(128 - (currentStep - 768));
//  }
//  else if (currentStep < 1024 + 128)
//  {
//    Joystick.setXAxis(currentStep - 1024 - 127);
//    Joystick.setYAxis(currentStep - 1024 - 127);
//  }
//}
//
//void testZAxis(unsigned int currentStep)
//{
//  if (currentStep < 128)
//  {
//    Joystick.setZAxis(-currentStep);
//  } 
//  else if (currentStep < 256 + 128)
//  {
//    Joystick.setZAxis(currentStep - 128 - 127);
//  } 
//  else if (currentStep < 256 + 128 + 127)
//  {
//    Joystick.setZAxis(127 - (currentStep - 383));
//  } 
//}
//
//void testHatSwitch(unsigned int currentStep)
//{
//  if (currentStep < 8)
//  {
//    Joystick.setHatSwitch(0, currentStep * 45);
//  }
//  else if (currentStep == 8)
//  {
//    Joystick.setHatSwitch(0, -1);
//  }
//  else if (currentStep < 17)
//  {
//    Joystick.setHatSwitch(1, (currentStep - 9) * 45);
//  }
//  else if (currentStep == 17)
//  {
//    Joystick.setHatSwitch(1, -1);
//  }
//  else if (currentStep == 18)
//  {
//    Joystick.setHatSwitch(0, 0);
//    Joystick.setHatSwitch(1, 0);
//  }
//  else if (currentStep < 27)
//  {
//    Joystick.setHatSwitch(0, (currentStep - 18) * 45);
//    Joystick.setHatSwitch(1, (8 - (currentStep - 18)) * 45);
//  }
//  else if (currentStep == 27)
//  {
//    Joystick.setHatSwitch(0, -1);
//    Joystick.setHatSwitch(1, -1);
//  }
//}
//
//void testThrottleRudder(unsigned int value)
//{
//  Joystick.setThrottle(value);
//  Joystick.setRudder(255 - value);
//}
//
//void testXYAxisRotation(unsigned int degree)
//{
//  Joystick.setXAxisRotation(degree);
//  Joystick.setYAxisRotation(360 - degree);
//}
  

void setup() {
 
  Joystick.begin();
  

  pinMode(2, INPUT);
  pinMode(3, INPUT);
  //pinMode(A0, INPUT_PULLUP);
  //pinMode(13, OUTPUT);
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

