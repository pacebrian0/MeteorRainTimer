/*  Copyright (c) 2023 Brian J Pace
    Email: pacebrian0@gmail.com
*/

typedef struct PixelStrip {
  byte pin;
  int timingMS;
} PixelStrip;


/*************************************************************
pin: pin on this controller
pollTiming: time in ms to wait until next trigger
countdown: internal time to wait for next trigger. Leave as-is.
*************************************************************/

PixelStrip strips[] = {
  //  pin   triggerTiming countdown        
    {   6,  1000,         0},
    {   7,  1000,         0},
};

#define NUMSTRIPS (sizeof(strips) / sizeof(strips[0]))

const int wipeTiming = 1000; //time in ms to wait for wipe to finish
unsigned long previousMillisMainLoop = 0;  // to measure loop time per millisecond precisely
unsigned long currentMS = 0;  // to keep track of current ms

void setup() {
  // put your setup code here, to run once:
    for (int i = 0; i < NUMSTRIPS; i++) {
      // set the digital pin as output:
      pinMode(strips[i].pin, OUTPUT);
    }

  delay(wipeTiming);
}


void Timings()
{
  for (int i = 0; i < NUMSTRIPS; i++) {
    PixelStrip s = strips[i];
    if(s.countdown == 0)
    {
      //send pin
      s.countdown = s.triggerTiming
    }
    else s.countdown--;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
    currentMS = millis();
    if(currentMS > previousMillisMainLoop){
      Timings();
      previousMillisMainLoop = currentMS;
    }


}
