/*
 * TM1637.cpp
 * A library for the 4 digit display
 *
 * Copyright (c) 2012 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : Frankie.Chu
 * Create Time: 9 April,2012
 * Change Log :
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
//#include "TM1637.h"
#include <DS1307RTC.h>
#include <Time.h>
#include <TM1637Display.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>

//#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Encoder.h>
// Library for using the Rotary Encoder
// Search for "encoder" in the Arduino Library manager and select the one
// by Paul Stoffregen
// https://github.com/PaulStoffregen/Encoder


//TM1637
#define CLK 2//pins definitions for TM1637 and can be changed to other ports
#define DIO 3
//TM1637 tm1637(CLK,DIO);
// Create TM1637 object
TM1637Display display(CLK,DIO);
// --------
// Neopixels
// --------
#define COLOUR_WHITE 0
#define COLOUR_RED 1
#define COLOUR_BLUE 2
#define COLOUR_GREEN 3
#define NUMPIXELS      16
#define DEFAULT_BRIGHTNESS 128
const int ledPin = 8;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, ledPin, NEO_GRB + NEO_KHZ800);

// Segments for showing BEEP
const uint8_t SEG_BEEP[] = {
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G, // B
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,                 // E
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,                 // E
  SEG_A | SEG_B | SEG_E | SEG_F | SEG_G,                 // P
  };
const uint8_t zero[] = {0, 0, 0, 0};
const uint8_t one[] = {0xff, 0, 0, 0};
const uint8_t two[] = {0xff, 0xff, 0, 0};
const uint8_t three[] = {0xff, 0xff, 0xff, 0};
const uint8_t four[] = {0xff, 0xff, 0xff, 0xff};
// --------
// Rotary Encoder
// --------

//const int reButtonPin = 0; //D3
const int reDTPin = 12;
const int reCLKPin = 11;


// Create RTC time object
tmElements_t RTC_time;

// Create RTC timer objects to hold the timer
// prevTimes is used to determine whether the timer has changed
tmElements_t Timer, prevTimer;

Encoder myEnc(reDTPin, reCLKPin);
int rotation;
int RotPosition = 0;
int value;
int previous_value = 0;
int progress = 0;

void setup()
{

  display.setBrightness(0x0a);
  Serial.begin(115200);
  pixels.begin();
  turnOffAllPixels();

    // Sync to DS3231
  setSyncProvider(RTC.get);
  setSyncInterval(10);
  
  // Send time to serial
  RTC.read(RTC_time);
  OutputTimeToSerial();

    // Get current Position
  rotation = myEnc.read();
  
}

void setAllPixels(uint32_t targetColour) {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, targetColour);
  }

  pixels.show();
}

void turnOffAllPixels(){
  setAllPixels(pixels.Color(0, 0, 0));
}

void bar (uint8_t percent)
{

//showNumberDec(14, false, 2, 1); // Expect: _14_
//display.showNumberDec(4, true, 2, 2);  // Expect: __04
//  display.showNumberDec(1, false); // Expect: ___1
  if (percent == 100) {
  
    //display.showNumberDec(8888, true);
    display.setSegments(four);
  }
  else if ((percent >= 75)&&(percent < 100)) {
  
    //display.showNumberDec(888, false, 3, 0);
    display.setSegments(three);
  }
  else if ((percent >= 50)&&(percent < 75)) {
  
    //display.showNumberDec(88, false, 2, 0);
    display.setSegments(two);
  }
  else if ((percent >=25 )&&(percent < 50)) {
  
    //display.showNumberDec(8, false, 1, 0);
    display.setSegments(one);

  }
//  else display.clear();
  
}

void OutputTimeToSerial()
// Send time to Serial-output
{
  Serial.print("Clock: ");
  Serial.print(RTC_time.Hour);
  Serial.print(":");
  Serial.print(RTC_time.Minute);
  Serial.print(":");
  Serial.println(RTC_time.Second);
}


void loop()
{

  unsigned char i = 0;
  unsigned char count = 0;
//  delay(150);
  pixels.clear(); // Set all pixel colors to 'off'

  //light white pixels set
  setAllPixels(pixels.Color(40, 40, 40));
  pixels.show();   // Send the updated pixel colors to the hardware.
  
  while(1)
  {

    bar(progress);
//    Serial.print("Time is ");
 //   Serial.println(i);
//    Serial.print("Position is ");
//    Serial.println(myEnc.read());   
    value = myEnc.read();
     if (value != rotation){ // we use the DT pin to find out which way we turning.
    
    if (value > rotation) progress++;
    else progress--;
    
        if (progress > 100) {
          progress = 100;
        } else if (progress <0) {
          progress = 0;
        };

      Serial.print("Progress is ");
      Serial.println(progress); 
      };

    rotation = value;
      
    i ++;
 //   delay(500);
    
//    RTC.read(RTC_time);
//    OutputTimeToSerial();


  }
}



  
