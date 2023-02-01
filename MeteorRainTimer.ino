/*  Copyright (c) 2023 Brian J Pace
    Email: pacebrian0@gmail.com
*/

typedef struct PixelStrip {
  byte pin;
  unsigned long timingMS;
  unsigned long initTimingMS;
  unsigned long lastRun;
  bool isOn;
} PixelStrip;


/*************************************************************
pin: pin on this controller
initTimingMS: time in ms to wait on the first loop
timingMS: time in ms to wait until next trigger
lastRun: internal time to wait for next trigger. Leave as-is.
isOn: internal check to indicate pin is on
*************************************************************/

PixelStrip strips[] = {
  //  pin   timingMS  initTimingMS  lastRun isOn        
    {   6,  200,     0,            0,      false},// we have 5 steps
    //{   7,  1000,   10000,            0,    false},
};

#define NUMSTRIPS (sizeof(strips) / sizeof(strips[0]))

const int wipeTiming = 10000; //time in ms to wait for wipe to finish
unsigned long previousTime = 0;  // to measure loop time per millisecond precisely
unsigned long currentTime = 0;  // to keep track of current ms

void setup() {
  Serial.begin(115200);  
  pinMode(LED_BUILTIN, OUTPUT);
  
  // put your setup code here, to run once:
    for (int i = 0; i < NUMSTRIPS; i++) {
      PixelStrip* s = &strips[i];
      // set the digital pin as output:
      pinMode(s->pin, OUTPUT);
      s->lastRun = millis() + s->initTimingMS + wipeTiming;   // for the start delay
      Serial.print(s->lastRun);      
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
      digitalWrite(LED_BUILTIN, HIGH);  // Onboard LED feedback
      digitalWrite(s->pin, HIGH);
      Serial.println(s->pin);
      digitalWrite(LED_BUILTIN, LOW); 
      digitalWrite(s->pin, LOW);
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
