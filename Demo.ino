#include <avr/sleep.h>
#include <avr/wdt.h>
#include <Adafruit_NeoPixel.h>
//#include "DigiKeyboard.h"

#define PIN 12
#define button A5
#define FET 1
#define brightness 255
unsigned long shutdownTimer;
byte shutdownFlag=0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(3, PIN, NEO_GRB + NEO_KHZ800);

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

byte state[3][3] = {
    {0,0,0},
    {0,0,0},
    {0,0,0}
    };
byte ctr=0;
byte buttonTrack=4;

// Candle: color variables: mix RGB (0-255) for desired yellow
int redPx = 255;
int grnHigh = 135; //110-120 for 5v, 135 for 3.3v
int bluePx = 15; //10 for 5v, 15 for 3.3v

// Candle: animation time variables, with recommendations
int burnDepth = 14; //10 for 5v, 14 for 3.3v -- how much green dips below grnHigh for normal burn - 
int flutterDepth = 45; //25 for 5v, 30 for 3.3v -- maximum dip for flutter
int cycleTime = 120; //120 -- duration of one dip in milliseconds

// Candle variables:
int fDelay;
int fRep;
int flickerDepth;
int burnDelay;
int burnLow;
int flickDelay;
int flickLow;
int flutDelay;
int flutLow;


void setup() {
  setup_watchdog(9);
  
  pinMode(FET,OUTPUT);  
  digitalWrite(FET,HIGH); //setup FET

  pinMode(PIN,OUTPUT);
  digitalWrite(PIN,LOW); //setup LED signal bus

  shutdownTimer = millis() + (60000*90);  // shutdown timer;

  randomSeed(analogRead(A8)+analogRead(A7));

  PORTA = (1<<PA7); // turn on pull-up on button

  // Candle Setup:
  flickerDepth = (burnDepth + flutterDepth) / 2.4;
  burnLow = grnHigh - burnDepth;
  burnDelay = (cycleTime / 2) / burnDepth;
  flickLow = grnHigh - flickerDepth;
  flickDelay = (cycleTime / 2) / flickerDepth;
  flutLow = grnHigh - flutterDepth;
  flutDelay = ((cycleTime / 2) / flutterDepth);  
  
  strip.begin();
  strip.show();

  
  //DigiKeyboard.sendKeyStroke(0);
}

// set system into the sleep state 
// system wakes up when wtchdog is timed out
void system_sleep() {
  //f_wdt=0;                             // reset flag
  cbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter OFF

  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
  sleep_enable();

  sleep_mode();                        // System sleeps here

  sleep_disable();                     // System continues execution here when watchdog timed out 
  sbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter ON
}


void goToSleep(void)
{
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    MCUCR &= ~(_BV(ISC01) | _BV(ISC00));      //INT0 on low level
//    GIMSK |= _BV(INT0);                       //enable INT0
    byte adcsra = ADCSRA;                     //save ADCSRA
    ADCSRA &= ~_BV(ADEN);                     //disable ADC
    cli();                                    //stop interrupts to ensure the BOD timed sequence executes as required
    byte mcucr1 = MCUCR | _BV(BODS) | _BV(BODSE);  //turn off the brown-out detector
    byte mcucr2 = mcucr1 & ~_BV(BODSE);
    MCUCR = mcucr1;
    MCUCR = mcucr2;
    sei();                                    //ensure interrupts enabled so we can wake up again
    sleep_cpu();                              //go to sleep
    sleep_disable();                          //wake up here
    ADCSRA = adcsra;                          //restore ADCSRA
}


// 0=16ms, 1=32ms,2=64ms,3=128ms,4=250ms,5=500ms
// 6=1 sec,7=2 sec, 8=4 sec, 9=8sec
void setup_watchdog(int ii) {

  byte bb;
  int ww;
  if (ii > 9 ) ii=9;
  bb=ii & 7;
  if (ii > 7) bb|= (1<<5);
  bb|= (1<<WDCE);
  ww=bb;

  MCUSR &= ~(1<<WDRF);
  // start timed sequence
  WDTCR |= (1<<WDCE) | (1<<WDE);
  // set new watchdog timeout value
  WDTCR = bb;
  WDTCR |= _BV(WDIE);
}
  
// Watchdog Interrupt Service / is executed when watchdog timed out
ISR(WDT_vect) {
  //f_wdt=1;  // set global flag
}

/////////////////////////////////////////////////////////////

void loop() {

  checkButton();
  
  switch (buttonTrack){
    case 0:
      colorrotate();
      break;
    case 1:
      multiblink();
      break;
    case 2:
      colorfade();
      break;
    case 3:
      randomblink();
      break;
    case 4:
      circle();
      break;
    case 5:
      gloWorm();
      break;
    case 6:
      burn(10);       checkButton(); if(buttonTrack!=6) break;
      flicker(5);     checkButton(); if(buttonTrack!=6) break;
      checkButton();  checkButton(); if(buttonTrack!=6) break;
      burn(8);        checkButton(); if(buttonTrack!=6) break;
      flutter(6);     checkButton(); if(buttonTrack!=6) break;
      burn(3);        checkButton(); if(buttonTrack!=6) break;
      on(10);         checkButton(); if(buttonTrack!=6) break;
      burn(10);       checkButton(); if(buttonTrack!=6) break;
      flicker(10);    checkButton(); if(buttonTrack!=6) break;
      break;
    }

  while(millis() > shutdownTimer){
    if(shutdownFlag==0){
      shutdownFlag=1;
      strip.setPixelColor(0, 0, 0, 0);
      strip.setPixelColor(1, 0, 0, 0);
      strip.setPixelColor(2, 0, 0, 0);
      strip.show();
      }
    system_sleep();  
    }

}

// basic fire funciton - not called in main loop
void fire(int grnLow) {
  for (int grnPx = grnHigh; grnPx > grnLow; grnPx--) {
    strip.setPixelColor(0, redPx, grnPx, bluePx);
    strip.setPixelColor(1, redPx, grnPx, bluePx); 
    strip.setPixelColor(2, redPx, grnPx, bluePx);
    strip.show();
    delay(fDelay);
  }  
  for (int grnPx = grnLow; grnPx < grnHigh; grnPx++) {
    strip.setPixelColor(0, redPx, grnPx, bluePx);
    strip.setPixelColor(1, redPx, grnPx, bluePx);
    strip.setPixelColor(2, redPx, grnPx, bluePx);
    strip.show();
    delay(fDelay);
  }
}

// fire animation
void on(int f) {
  fRep = f * 1000;
  int grnPx = grnHigh - 5;
  strip.setPixelColor(0, redPx, grnPx, bluePx);
  strip.setPixelColor(1, redPx, grnPx, bluePx);
  strip.setPixelColor(2, redPx, grnPx, bluePx);
  strip.show();
  delay(fRep);
}

void burn(int f) {
  fRep = f * 8;
  fDelay = burnDelay;
  for (int var = 0; var < fRep; var++) {
    fire(burnLow);
  }  
}

void flicker(int f) {
  fRep = f * 8;
  fDelay = burnDelay;
  fire(burnLow);
  fDelay = flickDelay;
  for (int var = 0; var < fRep; var++) {
    fire(flickLow);
  }
  fDelay = burnDelay;
  fire(burnLow);
  fire(burnLow);
  fire(burnLow);
}

void flutter(int f) {
  fRep = f * 8;  
  fDelay = burnDelay;
  fire(burnLow);
  fDelay = flickDelay;
  fire(flickLow);
  fDelay = flutDelay;
  for (int var = 0; var < fRep; var++) {
    fire(flutLow);
  }
  fDelay = flickDelay;
  fire(flickLow);
  fire(flickLow);
  fDelay = burnDelay;
  fire(burnLow);
  fire(burnLow);
}

void gloWorm(void){
  pinMode(button, HIGH);
  delay(2);
  int r=analogRead(A9);
  pinMode(button, LOW);
  
  if(r < 20){
    byte c=0;
    for(c=0;c<brightness;c++){
      strip.setPixelColor(0,0,c,0);
      strip.setPixelColor(1,0,c,0);
      strip.setPixelColor(2,0,c,0);
      strip.show();
      delay( (255/brightness) * 15);
      }
    
    checkButton(); 
    delay(100);
    checkButton();    
    
    for(c=brightness;c>0;c--){
      strip.setPixelColor(0,0,c,0);
      strip.setPixelColor(1,0,c,0);
      strip.setPixelColor(2,0,c,0);
      strip.show();
      delay( (255/brightness) * 15);
      }
    strip.show();
    checkButton();
    delay(500);
    checkButton();
    }
  else{
    strip.setPixelColor(0,0,0,0);
    strip.setPixelColor(1,0,0,0);
    strip.setPixelColor(2,0,0,0);
    strip.show();
    }
  }


byte checkButton(void){
  pinMode(button, HIGH);
  delay(5);
  if(analogRead(button)<50){
    if(buttonTrack<6){ buttonTrack++; }
    else             { buttonTrack=0; }
    while(analogRead(button)<50){}
    }
  pinMode(button, LOW);  
  }

void circle(void){
  byte pix=0;

  for(pix=0;pix<3;pix++){
    abscolor( pix,random(1,4) );
    strip.show();
    checkButton();
    delay(200);
    strip.setPixelColor(pix,0,0,0);
    strip.show();
    }

}


void randomblink(void){
  byte pix=0;

  pix=random(0,3);
  abscolor( pix,random(1,4) );
  
  strip.show();
  checkButton();
  delay(random(30,300));
  strip.setPixelColor(pix,0,0,0);
  strip.show();

}

void abscolor(byte p, byte c){
  switch(c){
    case 0:
      strip.setPixelColor( p, 0, 0, 0 );
      break;
    case 1:
      strip.setPixelColor( p, brightness, 0, 0 );
      break;
    case 2:
      strip.setPixelColor( p, 0, brightness, 0 );
      break;
    case 3:
      strip.setPixelColor( p, 0, 0, brightness );
      break;    
  }

}

void colorrotate(){
    byte p = 0;
    byte e = 0;
    byte x=0;

   p=random(0,3);
   e=random(0,3);
   
   if(state[p][e] < 2){
     for(x=0;x<brightness;x++){
       state[p][e]=x;
       strip.setPixelColor(p, state[p][0], state[p][1], state[p][2]);
       strip.show();
       checkButton();       
       delay((255/brightness)*15);
       }    
     }
   else if(state[p][e] > brightness - 3){
     for(x=brightness;x>0;x--){
       state[p][e]=x;
       strip.setPixelColor(p, state[p][0], state[p][1], state[p][2]);
       strip.show();
       checkButton();
       delay((255/brightness)*15);
       }
     state[p][e]=0;
     strip.setPixelColor(p, state[p][0], state[p][1], state[p][2]);
     strip.show();
     }  
  }

void colorfade(void){
    byte x=0;
    byte c[3] = {0,0,0};

    strip.setPixelColor(0, 0, 0, 0);
    strip.setPixelColor(1, 0, 0, 0);
    strip.setPixelColor(2, 0, 0, 0);
    strip.show();

   while(c[0]+c[1]+c[2] == 0){
     c[0] = random(0,2);
     c[1] = random(0,2);
     c[2] = random(0,2);
     }

   for(x=0;x<brightness;x++){
     strip.setPixelColor(0, x*c[0], x*c[1], x*c[2]);
     strip.setPixelColor(1, x*c[0], x*c[1], x*c[2]);
     strip.setPixelColor(2, x*c[0], x*c[1], x*c[2]);
     strip.show();
     checkButton();
     delay(5);
     }    

   for(x=brightness;x>0;x--){
     strip.setPixelColor(0, x*c[0], x*c[1], x*c[2]);
     strip.setPixelColor(1, x*c[0], x*c[1], x*c[2]);
     strip.setPixelColor(2, x*c[0], x*c[1], x*c[2]);
     strip.show();
     checkButton();
     delay(5);
     }    
  }


void multiblink(void){
  byte x=0;

  for(x=0;x<10;x++){
    strip.setPixelColor(0, 255, 255, 255);
    strip.setPixelColor(1, 255, 255, 255);
    strip.setPixelColor(2, 255, 255, 255);
    strip.show();
    checkButton();
    delay(random(20,120));

    strip.setPixelColor(0, 0, 0, 0);
    strip.setPixelColor(1, 0, 0, 0);
    strip.setPixelColor(2, 0, 0, 0);
    strip.show();
    checkButton();
    delay(random(20,120));
    }
  }


