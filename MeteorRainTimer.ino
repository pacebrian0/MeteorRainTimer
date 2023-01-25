/*  Copyright (c) 2023 Brian J Pace
    Email: pacebrian0@gmail.com
*/

typedef struct PixelStrip {
  byte pin;
  unsigned long timingMS;
  unsigned long lastRun;
} PixelStrip;


/*************************************************************
pin: pin on this controller
timingMS: time in ms to wait until next trigger
lastRun: internal time to wait for next trigger. Leave as-is.
*************************************************************/

PixelStrip strips[] = {
  //  pin   timingMS  lastRun        
    {   6,  1000,     0},
    //{   7,  1000,     0},
};

#define NUMSTRIPS (sizeof(strips) / sizeof(strips[0]))

const int wipeTiming = 1000; //time in ms to wait for wipe to finish
unsigned long previousTime = 0;  // to measure loop time per millisecond precisely
unsigned long currentTime = 0;  // to keep track of current ms

void setup() {
  Serial.begin(115200);  
  // put your setup code here, to run once:
    for (int i = 0; i < NUMSTRIPS; i++) {
      PixelStrip* s = &strips[i];
      // set the digital pin as output:
      pinMode(s->pin, OUTPUT);
      s->lastRun = millis();   
    }

  delay(wipeTiming);
}


void Timings()
{
  for (int i = 0; i < NUMSTRIPS; i++) {
    PixelStrip* s = &strips[i];
    // Serial.print(currentTime);
    // Serial.print("\t");    
    // Serial.print(s->lastRun);
    // Serial.print("\t");     
    // Serial.println(currentTime - s->lastRun);
    if ((unsigned long)(currentTime - s->lastRun) > s->timingMS)
    {
      s->lastRun = currentTime;  
      Serial.print("Sending pin ");
      Serial.println(s->pin);
    }
    

    // if(s.countdown == 0)
    // {
    //   //send pin
    //   s.countdown = s.timingMS;
    // }
    // else s.countdown--;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
    currentTime = millis();
    if(currentTime > previousTime){
      Timings();
      previousTime = currentTime;
    }


}
