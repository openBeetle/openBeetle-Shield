
#include "openBeetle.h"
void setup(){
  programStart();
}
void loop(){
  unsigned long partycurrentMillis = millis();
  unsigned long currentMillis = millis();
  rtcmenuPrint();
  if (Serial.available() > 0) {
  serialRead();
  }
  lcdPrint();
  engineRunningTime();
  memoryFree();
}
