/*  Copyright (c) 2023 Brian J Pace
    Email: pacebrian0@gmail.com
*/
#include <TimerOne.h>

typedef struct PixelStrip {
  byte pin;
  byte ledGroup;
} PixelStrip;


/*************************************************************
pin: pin on this controller
ledGroup: used to group up different led strips to begin together e.g. strip 1 & 2, strip 3 & 4... Begin from 0
*************************************************************/

PixelStrip strips[] = {
  //  pin  ledGroup          
    { 6,   0 },
    { 6,   1 },
};

// CONFIGURE-----------------------------
unsigned long GROUPSWAIT = 200; // ms to wait between groups
unsigned long POSTGROUPSWAIT = 5000; // ms to wait after all groups have been triggered
unsigned long WIPETIMING = 5000; //time in ms to wait for wipe to finish
//------------------------------------------


// DO NOT TOUCH-----------------------------
#define NUMSTRIPS (sizeof(strips) / sizeof(strips[0]))
unsigned long PREVTIME = 0;  // to measure loop time per millisecond precisely
unsigned long CURRTIME = 0;  // to keep track of current ms
unsigned long LASTRUNTIME = 0; // to keep track of last group trigger
unsigned long GROUPTIME = 0;  // to keep track of start of run
byte currLedGroup = 0; // to keep track of current group
byte maxLedGroup = 0; // to keep track of max group
bool isFinished = false; 
//------------------------------------------

void setup() {
  Serial.begin(115200);  
  Timer1.initialize(1000); // set the timer to trigger every 1 millisecond (1000 microseconds)
  Timer1.attachInterrupt(timerIsr); // attach the interrupt service routine (ISR)
  pinMode(LED_BUILTIN, OUTPUT);
  
  // put your setup code here, to run once:
    for (int i = 0; i < NUMSTRIPS; i++) {
      PixelStrip* s = &strips[i];
      // set the digital pin as output:
      pinMode(s->pin, OUTPUT);
      if (s->ledGroup > maxLedGroup)
        maxLedGroup = s->ledGroup;
    }
    Serial.println("Waiting for wipe");
  delay(WIPETIMING);
  //CURRTIME = millis();
  LASTRUNTIME = CURRTIME;  
  Serial.println("Wipe done");
}

void timerIsr() {
  CURRTIME++;
}

void Timings()
{
  if(isFinished && (unsigned long)(CURRTIME - GROUPTIME) < POSTGROUPSWAIT) // finished but waiting for post group wait
    return;
  else if (isFinished) // starting now
  {
    isFinished = false;
    GROUPTIME = CURRTIME;
  }

  if((unsigned long)(CURRTIME - LASTRUNTIME) < GROUPSWAIT) // still waiting for delay between groups
    return;

  
  for (int i = 0; i < NUMSTRIPS; i++) {
    PixelStrip* s = &strips[i];
    // Serial.print(CURRTIME);
    // Serial.print("\t");    
    // Serial.print(s->lastRun);
    // Serial.print("\t");     
    // Serial.println(CURRTIME - s->lastRun);

    if (s->ledGroup == currLedGroup)
    {
      LASTRUNTIME = CURRTIME;  
      Serial.print("Sending pin ");
      digitalWrite(LED_BUILTIN, HIGH);  // Onboard LED feedback
      digitalWrite(s->pin, HIGH);
      Serial.println(s->pin);
      digitalWrite(LED_BUILTIN, LOW); 
      digitalWrite(s->pin, LOW);
    }
  }

  currLedGroup++;

  if(currLedGroup > maxLedGroup)
  {
    Serial.print("Done, waiting for ");
    Serial.println(POSTGROUPSWAIT);
    isFinished = true;
    currLedGroup = 0;  
    GROUPTIME = CURRTIME;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
    //CURRTIME = millis();
    if(CURRTIME > PREVTIME){
      Timings();
      PREVTIME = CURRTIME;
    }


}
