#include <arduino.h>
int brbrightness = 0;
int blbrightness = 0;
int brsbrightness = 0;
int blsbrightness = 0;
int bmlbrightness = 0;
int frhbrightness = 0;
int frlbrightness = 0;
int frsbrightness = 0;
int frcbrightness = 0;
int flhbrightness = 0;
int fllbrightness = 0;
int flsbrightness = 0;
int flcbrightness = 0;
int setbrsbrightness = 0;
int setblsbrightness = 0;
int setfrsbrightness = 0;
int setflsbrightness = 0;
int setfrlbrightness = 0;
int setfrhbrightness = 0;
int setfllbrightness = 0;
int setflhbrightness = 0;
int randNumberbml = 0;
int randNumberbl = 0;
int randNumberbr = 0;
int randNumberbls = 0;
int randNumberbrs = 0;
int randNumberfls = 0;
int randNumberfrs = 0;
int randNumberflh = 0;
int randNumberfrh = 0;
int randNumberfll = 0;
int randNumberfrl = 0;
int randNumberflc = 0;
int randNumberfrc = 0;
int randNumberColor = 0;

void partyMode(){
  randNumberbml = random(0,2);
  randNumberbl = random(0,2);
  randNumberbr = random(0,2);
  randNumberbls = random(0,2);
  randNumberbrs = random(0,2);
  randNumberfls = random(0,2);
  randNumberfrs = random(0,2);
  randNumberflh = random(0,2);
  randNumberfrh = random(0,2);
  randNumberfll = random(0,2);
  randNumberfrl = random(0,2);
  randNumberflc = random(0,2);
  randNumberfrc = random(0,2);
  if (randNumberbml == 0){
    bmlbrightness = 0;
  }
  if (randNumberbml == 1){
    bmlbrightness = 255;
  }
  if (randNumberbl == 0){
    blbrightness = 0;
  }
  if (randNumberbl == 1){
    blbrightness = 255;
  }
  if (randNumberbr == 0){
    brbrightness = 0;
  }
  if (randNumberbr == 1){
    brbrightness = 255;
  }
  if (randNumberbrs == 0){
    brsbrightness = 0;
  }
  if (randNumberbrs == 1){
    brsbrightness = 255;
  }
  if (randNumberbls == 0){
    blsbrightness = 0;
  }
  if (randNumberbls == 1){
    blsbrightness = 255;
  }
  if (randNumberfls == 0){
    flsbrightness = 0;
  }
  if (randNumberfls == 1){
    flsbrightness = 255;
  }
  if (randNumberfrs == 0){
    frsbrightness = 0;
  }
  if (randNumberfrs == 1){
    frsbrightness = 255;
  }
  if (randNumberflh == 0){
    flhbrightness = 0;
  }
  if (randNumberflh == 1){
    flhbrightness = 255;
  }
  if (randNumberfrh == 0){
    frhbrightness = 0;
  }
  if (randNumberfrh == 1){
    frhbrightness = 255;
  }
  if (randNumberfll == 0){
    fllbrightness = 0;
  }
  if (randNumberfll == 1){
    fllbrightness = 255;
  }
  if (randNumberfrl == 0){
    frlbrightness = 0;
  }
  if (randNumberfrl == 1){
    frlbrightness = 255;
  }
  if (randNumberflc == 0){
    flcbrightness = 0;
  }
  if (randNumberflc == 1){
    flcbrightness = 255;
  }
  if (randNumberfrc == 0){
    frcbrightness = 0;
  }
  if (randNumberfrc == 1){
    frcbrightness = 255;
  }

}
