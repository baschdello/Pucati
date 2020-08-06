#include <Arduino.h>
#include <Ticker.h>

#define WATERPUMP     PB3
#define FLOATSWITCH   PB4

volatile uint8_t lock;
volatile uint8_t time;
void timerISR();
Ticker timer(timerISR, 1000);

void setup() {
  // put your setup code here, to run once:
  pinMode(PB0,INPUT_PULLUP);
  pinMode(PB1,INPUT_PULLUP);
  pinMode(PB2,INPUT_PULLUP);
  digitalWrite(WATERPUMP,HIGH);
  pinMode(FLOATSWITCH,INPUT_PULLUP);
  //timer.interval(1000); //ms
  timer.stop();
  wdt_enable(WDTO_1S);
}

void timerISR() {
  if(time-- == 0) {
    pinMode(WATERPUMP,INPUT);
    timer.stop();
    lock = 0;
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  if(!digitalRead(FLOATSWITCH) && !lock)
  {
    time = !digitalRead(PB2)*40 + !digitalRead(PB1)*20 + !digitalRead(PB0)*10;
    pinMode(WATERPUMP,OUTPUT);
    timer.start();
    lock = 1;
  }

  wdt_reset();
  timer.update();
}
