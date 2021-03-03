// ****************************************************************************
// Arduino Throttle Quadrant for DCS
// ****************************************************************************
//
// MIT License
//
// Copyright (c) 2021 Toni Novak. Email: toninovak@gmail.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// *************************************
// JOYSTICK Declarations
// *************************************

#include "Joystick.h"

// Create Joystick
Joystick_ Joystick;

// Set to true to test "Auto Send" mode or false to test "Manual Send" mode.
//const bool testAutoSendMode = true;
const bool testAutoSendMode = false;

const unsigned long gcCycleDelta = 1000;
const unsigned long gcAnalogDelta = 25;
const unsigned long gcButtonDelta = 500;
unsigned long gNextTime = 0;
unsigned int gCurrentStep = 0;

// *************************************
// Switches Declarations
// *************************************

const int switch0 = 0;
const int switch1 = 1;
const int switch2 = 2;
const int switch3 = 3;
const int switch4 = 4;
const int switch5 = 5;
const int switch6 = 6;
const int switch7 = 7;
const int switch8 = 8;
const int switch9 = 9;
const int switch10 = 10;
const int switch11 = 11;
const int switch12 = 12;
const int switch13 = 13;

const int ledPin =  13;      // the number of the LED pin        

int switchState[14];

// *************************************
// Throttle ENCODE Declarations 
// *************************************

// Rotary Encoder Inputs
#define CLK A0
#define DT A1
// #define SW 4

int counter = 0;
int throttleValue = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
unsigned long lastButtonPress = 0;

void setup() {

  Serial.begin(9600);  
  pinMode(ledPin, OUTPUT);

  // *************************************
  // Pedals ENCODER Setup 
  // *************************************  

  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);    
  pinMode(A5, INPUT_PULLUP);    

  // *************************************
  // Throttle ENCODER Setup 
  // *************************************  

  // Throttle limit sensor
  pinMode(A2, INPUT_PULLUP);

  // Set encoder pins as inputs
  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);

  // Read the initial state of CLK
  lastStateCLK = digitalRead(CLK);  

  // *************************************
  // Switches Setup 
  // *************************************    

  pinMode(switch0, INPUT);
  pinMode(switch1, INPUT);
  pinMode(switch2, INPUT);
  pinMode(switch3, INPUT);
  pinMode(switch4, INPUT);
  pinMode(switch5, INPUT);
  pinMode(switch6, INPUT);
  pinMode(switch7, INPUT);
  pinMode(switch8, INPUT);
  pinMode(switch9, INPUT);
  pinMode(switch10, INPUT);
  pinMode(switch11, INPUT);
  pinMode(switch12, INPUT);
  pinMode(switch13, INPUT);

  // *************************************
  // Joystick Setup 
  // ************************************* 

  // Set Range Values
  Joystick.setXAxisRange(-127, 127);
  Joystick.setYAxisRange(-127, 127);
  Joystick.setZAxisRange(0, 255);
  Joystick.setRxAxisRange(0, 360);
  Joystick.setRyAxisRange(360, 0);
  Joystick.setRzAxisRange(0, 720);
  Joystick.setThrottleRange(0, 255);
  Joystick.setRudderRange(255, 0);   

  Joystick.begin();   

}

void loop() { 

  // *************************************
  // Switches INPUT processing 
  // *************************************   
  
  checkSwitch(switch0);
  checkSwitch(switch1);
  checkSwitch(switch2);
  checkSwitch(switch3);
  checkSwitch(switch4);
  checkSwitch(switch5);
  checkSwitch(switch6);
  checkSwitch(switch7);
  checkSwitch(switch8);
  checkSwitch(switch9);
  checkSwitch(switch10);
  checkSwitch(switch11);
  checkSwitch(switch12);
  checkSwitch(switch13);

  // *************************************
  // Throttle INPUT processing 
  // ************************************* 
  
  // Read the current state of CLK
  currentStateCLK = digitalRead(CLK);

  // If last and current state of CLK are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

    // If the DT state is different than the CLK state then
    // the encoder is rotating CCW so decrement
    if (digitalRead(DT) != currentStateCLK) {
      counter --;
      currentDir ="CCW";
    } else {
      // Encoder is rotating CW so increment
      counter ++;
      currentDir ="CW";
    }

    //Serial.print("Direction: ");
    //Serial.print(currentDir);
    //Serial.print(" | Counter: ");

    // Serial.println("Counter raw: " + (String)counter);    

    //counter = counter + 1;

    //if (counter>=6) { counter=6; }
    //if (counter<=0) { counter=0; }
        
    //Serial.println("Counter: " + (String)counter);  

    int ir = counter;

    if (ir>=6) { ir=6; }
    if (ir<=1) { ir=1; }

    Serial.println("Counter raw: " + (String)ir);    

    throttleValue = (((ir-1) * 255)/5);
    
    Serial.println("Throttle: " + (String)throttleValue);

    Joystick.setThrottle(throttleValue);
    Joystick.setZAxis(throttleValue);
    
  }

    if (digitalRead(A2)==LOW) {
      counter=0;
      // Serial.println("T. limit sensor activated");  
    }    

  // Remember last CLK state
  lastStateCLK = currentStateCLK;

  // Put in a slight delay to help debounce the reading
  // delay(1);  


  // *************************************
  // Pedals check 
  // *************************************  

    if (digitalRead(A3)==LOW) {
      Joystick.pressButton(20);  
      Serial.println("Pedal 0 pressed");
    }
    else {
      Joystick.releaseButton(20);  
    }

    if (digitalRead(A4)==LOW) {
      Joystick.pressButton(21);  
      Serial.println("Pedal 1 pressed");
    } else {
      Joystick.releaseButton(21);  
    }

    if (digitalRead(A5)==LOW) {
      Joystick.pressButton(22);  
      Serial.println("Pedal 2 pressed");
    }else { 
      Joystick.releaseButton(22);  
    }    
        
  // *************************************
  // JOYSTICK processing 
  // *************************************    

    if (testAutoSendMode == false)
    {
      Joystick.sendState();
    }  
  
}

void checkSwitch(int pin){

    int buttonState = digitalRead(pin);

    if (buttonState!=switchState[pin]) {

        switchState[pin] = buttonState;
  
        if (buttonState == HIGH) {
            Joystick.pressButton(pin);            
            Serial.print("ON switch " + (String)pin); 
            Serial.print("\n"); 
        } else {       
            Joystick.releaseButton(pin);   
            Serial.print("OFF switch " + (String)pin); 
            Serial.print("\n");         
        }  

    }
 
}
