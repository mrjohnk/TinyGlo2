//#include "DigiKeyboard.h"
#include <Adafruit_NeoPixel.h>
#define LED 12
#define micPin A8
#define FET 1

Adafruit_NeoPixel strip = Adafruit_NeoPixel(3, LED, NEO_GRB + NEO_KHZ800);
     
    #define samples 40
    #define brightness 255
    float baseline = 65;
     
    void setup() 
    {
      pinMode(FET,OUTPUT);
      digitalWrite(FET,HIGH);
      
      pinMode(micPin,INPUT);
      digitalWrite(micPin, LOW);

      pinMode(LED,OUTPUT);
      
      strip.begin();
    }
     
    void loop() 
    {
       showSound2( sampleRange() );
    }


unsigned int sampleRange(void){
  unsigned int sample;
  unsigned int signalMax = 0;   
  unsigned int signalMin = 1024;   
  byte s;
             
       for (s=0;s<samples;s++){
         sample = analogRead(micPin);
         if (sample < 1024){  // toss out spurious readings

           if (sample > signalMax){
             signalMax = sample;  // save just the max levels
             }
           else if (sample < signalMin){
             signalMin = sample;  // save just the min levels
             }
           }
         }
       unsigned int peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
       return(signalMax);
  }

    void showSound2(unsigned int s){
      float increment = 3;  //three LED@50:   0.04
      float baseUp = 1.9;     //                   4
      float squelch = 2;   //                   1.5
       
      if(s < baseline){
        strip.setPixelColor(0,0,0,0);
        strip.setPixelColor(1,0,0,0);
        strip.setPixelColor(2,0,0,0);
        baseline -= baseUp;
        }
      else if( s >= baseline && s < (baseline + squelch) ){  
        }
      
      else if( s > (baseline + squelch) && s < (baseline+increment) ){
        strip.setPixelColor(0,0,0,brightness);
        strip.setPixelColor(1,0,0,brightness);
        strip.setPixelColor(2,0,0,brightness);
        baseline += baseUp;
        }
      else if( s >= (baseline + increment) && s < (baseline + (increment*1.3)) ){
        strip.setPixelColor(0,0,brightness,0);   //3@50= 1.3 & 1.5
        strip.setPixelColor(1,0,brightness,0);
        strip.setPixelColor(2,0,brightness,0);
        baseline += baseUp;
        }
      else if( s >= (baseline + (increment*1.3)) && s < (baseline + (increment*1.7)) ){
        strip.setPixelColor(0,brightness,0,brightness); //3@50= 1.5 & 2
        strip.setPixelColor(1,brightness,0,brightness);
        strip.setPixelColor(2,brightness,0,brightness);
        baseline += baseUp;
        }        
      else if(s <= (baseline + (increment*1.7)) ){  //3@50= 2
        strip.setPixelColor(0,brightness,0,0); 
        strip.setPixelColor(1,brightness,0,0);
        strip.setPixelColor(2,brightness,0,0);
        baseline += baseUp;
        }
      strip.show();    
    }

    
