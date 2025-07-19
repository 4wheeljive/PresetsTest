#include <Arduino.h>
#include <stdio.h>
#include <string>
//#include <FastLED.h>

//*********************************************

    #define DATA_PIN_1 2
    #define HEIGHT 16 
    #define WIDTH 16
    #define NUM_SEGMENTS 1
    #define NUM_LEDS_PER_SEGMENT 256

//*********************************************

#include "bleControl.h"


//**********************************************************************************************

void setup() {

    Serial.begin(115200);

  //  FastLED.addLeds<WS2812B, DATA_PIN_1, GRB>(leds, 0, NUM_LEDS_PER_SEGMENT)
    //    .setCorrection(TypicalLEDStrip);

    bleSetup();

 }

//************************************************************************************************************

void loop() {

    if (!deviceConnected && wasConnected) {
        if (debug) {Serial.println("Device disconnected.");}
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising();
        if (debug) {Serial.println("Start advertising");}
        wasConnected = false;
    }
    
}
