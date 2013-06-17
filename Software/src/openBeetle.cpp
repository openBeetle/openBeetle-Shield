#include <arduino.h>
#include "LiquidCrystal_I2C.h"
#include "RTClib.h"
#include "partyMode.h"
const int lsl = 45;
LiquidCrystal_I2C lcd(0x27,20,4);
RTC_DS1307 RTC;
int scrolldirection = 0;
char* hourNumber_string[] = {
  "12", " 1", " 2", " 3", " 4", " 5", " 6", " 7", " 8", " 9", "10", "11", "12", " 1", " 2", " 3", " 4", " 5", " 6", " 7", " 8", " 9", "10", "11", "12"};
char* number_string[] = {
  "00", "01", "02", "03", "04", "05", "06", "07", "08", "09"};
int long previousRunningTime;
float  totalRunningTime;
extern int __bss_end;
extern void *__brkval;
int languageDefault = 0;
const String english[lsl] =  {"serial input: ",
" ",
" ",
" ",
" ",
"**WARNING** RAM below 1000 bytes: ",
"**WARNING** RAM below 500 bytes: ",
"**WARNING**",
"BOOT",
"engine hours: ",
"Free RAM: ",
"Main Menu   ",
"Information        ",
"Modes              ",
"Settings           ",
"GPS                ",
"Engine             ",
"I/O States         ",
"System             ",
"Information ",
"I/O States         ",
"Speed              ",
"Altitude           ",
"Accuracy           ",
"Sats #             ",
"Coordinates        ",
"Hours              ",
"Voltage            ",
"Emergancy          ",
"Random             ",
"Auto Light Delay   ",
"Auto Light Dimmer  ",
"Low Voltage Cut Off",
"Speed Units        ",
"Altitude Units     ",
"Language           ",
"English            ",
"German             ",
"Free RAM           ",
"Free RAM   ",
"Modes       ",
"Settings    ",
"Language    ",
"language default: ",
"System      "};
const String german[lsl] =  {"serial input: ",
" ",
" ",
" ",
" ",
"**VORSICHT** RAM unten 1000 bytes: ",
"**VORSICHT** RAM unten 500 bytes: ",
"**VORSICHT**",
"BOOT",
"motor stunden: ",
"kostenlos RAM: ",
"Hauptmenü   ",
"Informationen      ",
"Modes              ",
"Einstellungen      ",
"GPS                ",
"Motor              ",
"I/O Staaten        ",
"System             ",
"Informationen",
"I/O Staaten        ",
"Beschleunigen      ",
"Höhe               ",
"Genauigkeit        ",
"Satellites #       ",
"Koordinaten        ",
"Stunden            ",
"Spannung           ",
"Notfall            ",
"Zufällige          ",
"Auto Licht Verzöge ",
"Auto-Licht-Dimmer  ",
"Low Voltage Cut Off",
"Geschwindigkeit Ei ",
"Höheneinheiten     ",
"Sprache            ",
"Englisch           ",
"Deutsch            ",
"kostenlos          ",
"kostenlos   ",
"Modes       ",
"Einstellunge",
"Sprache     ",
"Sprache standardmäßig: ",
"System      "};

String language[lsl] =  {};
void rtcSerialPrint(){
  RTC.begin();
  DateTime now = RTC.now();
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print('/');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.print(" ");
}

void rtcmenuPrint(){
  RTC.begin();
  DateTime now = RTC.now();
  lcd.setCursor(12, 0);
  lcd.print(hourNumber_string[now.hour()]);
  lcd.print(':');
  if (now.minute() < 10){
    lcd.print(number_string[now.minute()]);
  }
  if (now.minute() > 9){
    lcd.print(now.minute());
  }
  lcd.print(':');
  if (now.second() < 10){
    lcd.print(number_string[now.second()]);
  }
  if (now.second() > 9){
    lcd.print(now.second());
  } 
}
int xSelect = 1;
int ySelect = 1;
int Select = 1;
//int serialinString;
float engineRunningTime(){
  DateTime now = RTC.now();
  int long currentRunningTime = now.unixtime();
  totalRunningTime =  currentRunningTime - previousRunningTime + totalRunningTime;
  float adjTotalRunningTime = totalRunningTime/60/60;
  previousRunningTime = currentRunningTime;
  return adjTotalRunningTime;
}
int memoryFree(){
  int freeValue; 
  if((int)__brkval == 0)
    freeValue = ((int)&freeValue) - ((int)&__bss_end);
  else
    freeValue = ((int)&freeValue) - ((int)__brkval);
  if (freeValue <= 1000 && freeValue > 500){
    rtcSerialPrint();
    Serial.print(language[5]);
    Serial.println(freeValue);
  }
  if (freeValue <= 500){
    rtcSerialPrint();
    Serial.print(language[6]);
    Serial.println(freeValue);
    Serial.print(language[7]);
  }
  return freeValue;
}
// read serial inputs (using to simulate buttons for development)
void serialRead(){
  String serialinString = "";
  int h = Serial.available();
  for (int i=0;i<h;i++){
    serialinString += (char)Serial.read();
  }
  delay(100);
  rtcSerialPrint();
  Serial.print(language[0]);    
  Serial.println(serialinString);
  //hours
  if (serialinString == "hours"){
    ySelect = 1;
    xSelect = 1121;
    scrolldirection = 0;
    rtcSerialPrint();
  Serial.print(language[9]);
  Serial.print(totalRunningTime/60/60);
  Serial.println(" hrs");
  }
  //ram
  if (serialinString == "ram"){
    ySelect = 1;
    xSelect = 1141;
    scrolldirection = 0;
    rtcSerialPrint();
    int freeValue; 
  if((int)__brkval == 0)
    freeValue = ((int)&freeValue) - ((int)&__bss_end);
  else
    freeValue = ((int)&freeValue) - ((int)__brkval);
    Serial.print(language[10]);
    Serial.println(freeValue);
  }
  //w
  if (serialinString == "w"){
    ySelect = ySelect - 1; 
    scrolldirection = 0;
    if (ySelect == 0){
      ySelect = 1;
    }
  }
  //s
  if (serialinString == "s"){
    ySelect = ySelect + 1;
    scrolldirection = 1; 
  }
  //a
  if (serialinString == "a"){
    xSelect = xSelect / 10;
    ySelect = 1;
    if (xSelect < 10){
      xSelect = 1;
    }
  }
  //d
  if (serialinString == "d"){
    xSelect = xSelect * 10 + ySelect;
    ySelect = 1;  
  }
  //q
  if (serialinString == "q"){
    Select = 0;
    Serial.print("buttonpress: ");
    Serial.println(Select);
  }
  //german
  if (serialinString == "german"){
for (int lst = 0; lst <= lsl; lst++)  {
      language[lst] = german[lst];
    }    
  languageDefault = 1;
    rtcSerialPrint();
  Serial.print(language[43]);
  Serial.println(languageDefault);
}
  //english
  if (serialinString == "english"){
for (int lst = 0; lst <= lsl; lst++)  {
      language[lst] = english[lst];
    }    
  languageDefault = 0;
    rtcSerialPrint();
  Serial.print(language[43]);
  Serial.println(languageDefault);
}
}
void programStart(){
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 1);
  lcd.print("     openBeetle     ");
  lcd.setCursor(0, 3);
  lcd.print("     v.20130614     ");
  RTC.begin();
  DateTime now = RTC.now();
  previousRunningTime = now.unixtime();
  delay(2500);
  if (languageDefault == 0){
  for (int lst = 0; lst <= lsl; lst++)  {
      language[lst] = english[lst];
    }
  }
  if (languageDefault == 1){
  for (int lst = 0; lst <= lsl; lst++)  {
      language[lst] = german[lst];
    }
  }
  Serial.println();
  Serial.println();
  rtcSerialPrint();
  Serial.println(language[8]);
  Serial.println("--------------------");
  Serial.println();
  rtcSerialPrint();
  Serial.print(language[43]);
  Serial.println(languageDefault);
  rtcSerialPrint();
  Serial.print(language[9]);
  Serial.print(totalRunningTime);
  Serial.println(" hrs");
  rtcSerialPrint();
  Serial.print(language[10]);
  Serial.print(memoryFree());
  Serial.println(" bytes");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("openBeetle");
}
void lcdPrint(){
  
  //1
  if (xSelect == 1){
    lcd.setCursor(17, 1);
    lcd.print(ySelect);
    lcd.print("/");
    lcd.print("3");
  }

  if (xSelect == 1 && ySelect == 1){
    lcd.setCursor(3, 1);
    lcd.print(language[11]);
    lcd.setCursor(0, 2);
    lcd.print("*");
    lcd.setCursor(1, 2);
    lcd.print(language[12]);
    lcd.setCursor(0, 3);
    lcd.print(" ");
    lcd.setCursor(1, 3);
    lcd.print(language[13]);
    if (Select == 0){
    } 
  }
  //1
  if (xSelect == 1 && ySelect == 2){
    lcd.setCursor(3, 1);
    lcd.print(language[11]);
    if (scrolldirection == 1){
      lcd.setCursor(0, 2);
      lcd.print(" ");
      lcd.setCursor(1, 2);
      lcd.print(language[12]);
      lcd.setCursor(0, 3);
      lcd.print("*");
      lcd.setCursor(1, 3);
      lcd.print(language[13]);
    }
    if (scrolldirection == 0){
      lcd.setCursor(0, 2);
      lcd.print("*");
      lcd.setCursor(1, 2);
      lcd.print(language[13]);
      lcd.setCursor(0, 3);
      lcd.print(" ");
      lcd.setCursor(1, 3);
      lcd.print(language[14]);
    }
    if (Select == 0){
    } 
  }
  if (xSelect == 1 && ySelect == 3){
    lcd.setCursor(3, 1);
    lcd.print(language[11]);
    lcd.setCursor(0, 2);
    lcd.print(" ");
    lcd.setCursor(1, 2);
    lcd.print(language[13]);
    lcd.setCursor(0, 3);
    lcd.print("*");
    lcd.setCursor(1, 3);
    lcd.print(language[14]);
    if (Select == 0){
    }
  }
  if (xSelect == 1 && ySelect > 3){
    ySelect = 3;
  }
  //11
  if (xSelect == 11){
    lcd.setCursor(17, 1);
    lcd.print(ySelect);
    lcd.print("/");
    lcd.print("4");
  }
  if (xSelect == 11 && ySelect == 1){
    lcd.setCursor(3, 1);
    lcd.print(language[19]);
    lcd.setCursor(0, 2);
    lcd.print("*");
    lcd.setCursor(1, 2);
    lcd.print(language[15]);
    lcd.setCursor(0, 3);
    lcd.print(" ");
    lcd.setCursor(1, 3);
    lcd.print(language[16]);
    if (Select == 0){
    } 
  }
  if (xSelect == 11 && ySelect == 2){
    lcd.setCursor(3, 1);
    lcd.print(language[19]);
    if (scrolldirection == 1){
      lcd.setCursor(0, 2);
      lcd.print(" ");
      lcd.setCursor(1, 2);
      lcd.print(language[15]);
      lcd.setCursor(0, 3);
      lcd.print("*");
      lcd.setCursor(1, 3);
      lcd.print(language[16]);
    }
    if (scrolldirection == 0){
      lcd.setCursor(0, 2);
      lcd.print("*");
      lcd.setCursor(1, 2);
      lcd.print(language[16]);
      lcd.setCursor(0, 3);
      lcd.print(" ");
      lcd.setCursor(1, 3);
      lcd.print(language[17]);
    }
    if (Select == 0){
    }
  }
  if (xSelect == 11 && ySelect == 3){
    lcd.setCursor(3, 1);
    lcd.print(language[19]);
    if (scrolldirection == 1){
      lcd.setCursor(0, 2);
      lcd.print(" ");
      lcd.setCursor(1, 2);
      lcd.print(language[16]);
      lcd.setCursor(0, 3);
      lcd.print("*");
      lcd.setCursor(1, 3);
      lcd.print(language[17]);
    }
    if (scrolldirection == 0){
      lcd.setCursor(0, 2);
      lcd.print("*");
      lcd.setCursor(1, 2);
      lcd.print(language[17]);
      lcd.setCursor(0, 3);
      lcd.print(" ");
      lcd.setCursor(1, 3);
      lcd.print(language[18]);
    }
    if (Select == 0){
    }
  }
  if (xSelect == 11 && ySelect == 4){
    lcd.setCursor(3, 1);
    lcd.print(language[19]);
    lcd.setCursor(0, 2);
    lcd.print(" ");
    lcd.setCursor(1, 2);
    lcd.print(language[17]);
    lcd.setCursor(0, 3);
    lcd.print("*");
    lcd.setCursor(1, 3);
    lcd.print(language[18]);
    if (Select == 0){
    }
  }
  if (xSelect == 11 && ySelect > 4){
    ySelect = 4;
  }
  //111
  if (xSelect == 111){
    lcd.setCursor(17, 1);
    lcd.print(ySelect);
    lcd.print("/");
    lcd.print("5");
  }
  if (xSelect == 111 && ySelect == 1){
    lcd.setCursor(3, 1);
    lcd.print(language[15]);
    lcd.setCursor(0, 2);
    lcd.print("*");
    lcd.setCursor(1, 2);
    lcd.print(language[21]);
    lcd.setCursor(0, 3);
    lcd.print(" ");
    lcd.setCursor(1, 3);
    lcd.print(language[22]);
    if (Select == 0){
    } 
  }

  if (xSelect == 111 && ySelect == 2){
    lcd.setCursor(3, 1);
    lcd.print(language[15]);
    if (scrolldirection == 1){
      lcd.setCursor(0, 2);
      lcd.print(" ");
      lcd.setCursor(1, 2);
      lcd.print(language[21]);
      lcd.setCursor(0, 3);
      lcd.print("*");
      lcd.setCursor(1, 3);
      lcd.print(language[22]);
    }
    if (scrolldirection == 0){
      lcd.setCursor(0, 2);
      lcd.print("*");
      lcd.setCursor(1, 2);
      lcd.print(language[22]);
      lcd.setCursor(0, 3);
      lcd.print(" ");
      lcd.setCursor(1, 3);
      lcd.print(language[23]);
    }
    if (Select == 0){
    }
  }
  if (xSelect == 111 && ySelect == 3){
    lcd.setCursor(3, 1);
    lcd.print(language[15]);
    if (scrolldirection == 1){
      lcd.setCursor(0, 2);
      lcd.print(" ");
      lcd.setCursor(1, 2);
      lcd.print(language[22]);
      lcd.setCursor(0, 3);
      lcd.print("*");
      lcd.setCursor(1, 3);
      lcd.print(language[23]);
    }
    if (scrolldirection == 0){
      lcd.setCursor(0, 2);
      lcd.print("*");
      lcd.setCursor(1, 2);
      lcd.print(language[23]);
      lcd.setCursor(0, 3);
      lcd.print(" ");
      lcd.setCursor(1, 3);
      lcd.print(language[24]);
    }
    if (Select == 0){
    }
  }
  if (xSelect == 111 && ySelect == 4){
    lcd.setCursor(3, 1);
    lcd.print(language[15]);
    if (scrolldirection == 1){
      lcd.setCursor(0, 2);
      lcd.print(" ");
      lcd.setCursor(1, 2);
      lcd.print(language[23]);
      lcd.setCursor(0, 3);
      lcd.print("*");
      lcd.setCursor(1, 3);
      lcd.print(language[24]);
    }
    if (scrolldirection == 0){
      lcd.setCursor(0, 2);
      lcd.print("*");
      lcd.setCursor(1, 2);
      lcd.print(language[24]);
      lcd.setCursor(0, 3);
      lcd.print(" ");
      lcd.setCursor(1, 3);
      lcd.print(language[25]);
    }
    if (Select == 0){
    }
  }
  if (xSelect == 111 && ySelect == 5){
    lcd.setCursor(3, 1);
    lcd.print(language[15]);
    lcd.setCursor(0, 2);
    lcd.print(" ");
    lcd.setCursor(1, 2);
    lcd.print(language[24]);
    lcd.setCursor(0, 3);
    lcd.print("*");
    lcd.setCursor(1, 3);
    lcd.print(language[25]);
    if (Select == 0){
    }
  }
  if (xSelect == 111 && ySelect > 5){
    ySelect = 5;
  }
  //112
  if (xSelect == 112){
    lcd.setCursor(17, 1);
    lcd.print(ySelect);
    lcd.print("/");
    lcd.print("2");
  }
  if (xSelect == 112 && ySelect == 1){
    lcd.setCursor(3, 1);
    lcd.print(language[16]);
    lcd.setCursor(0, 2);
    lcd.print("*");
    lcd.setCursor(1, 2);
    lcd.print(language[26]);
    lcd.setCursor(0, 3);
    lcd.print(" ");
    lcd.setCursor(1, 3);
    lcd.print(language[27]);
    if (Select == 0){
    }
  }
  if (xSelect == 112 && ySelect == 2){
    lcd.setCursor(3, 1);
    lcd.print(language[16]);
    lcd.setCursor(0, 2);
    lcd.print(" ");
    lcd.setCursor(1, 2);
    lcd.print(language[26]);
    lcd.setCursor(0, 3);
    lcd.print("*");
    lcd.setCursor(1, 3);
    lcd.print(language[27]);
    if (Select == 0){
    }
  }
  if (xSelect == 112 && ySelect > 2){
    ySelect = 2;
  }
  //1121
  if (xSelect == 1121){
    lcd.setCursor(17, 1);
    lcd.print("   ");
  }
  if (xSelect == 1121 && ySelect == 1){
    lcd.setCursor(3, 1);
    lcd.print(language[26]);
    lcd.setCursor(0, 2);
    lcd.print(" ");
    lcd.setCursor(1, 2);
    lcd.print("                   ");
    lcd.setCursor(1, 2);
    lcd.print(engineRunningTime());
    lcd.print(" Hr");
    lcd.setCursor(0, 3);
    lcd.print(" ");
    lcd.setCursor(1, 3);
    lcd.print("                   ");
    if (Select == 0){
    }
  }
  if (xSelect == 112 && ySelect > 1){
    ySelect = 1;
  }
  //113
  if (xSelect == 113){
    lcd.setCursor(17, 1);
    lcd.print(ySelect);
    lcd.print("/");
    lcd.print("2");
  }
  if (xSelect == 113 && ySelect == 1){
    lcd.setCursor(3, 1);
    lcd.print(language[20]);
    lcd.setCursor(0, 2);
    lcd.print("*");
    lcd.setCursor(1, 2);
    lcd.print("(list of states)   ");
    lcd.setCursor(0, 3);
    lcd.print(" ");
    lcd.setCursor(1, 3);
    lcd.print("(list of states)   ");
    if (Select == 0){
    }
  }
  if (xSelect == 113 && ySelect == 2){
    lcd.setCursor(3, 1);
    lcd.print(language[20]);
    lcd.setCursor(0, 2);
    lcd.print(" ");
    lcd.setCursor(1, 2);
    lcd.print("(list of states)   ");
    lcd.setCursor(0, 3);
    lcd.print("*");
    lcd.setCursor(1, 3);
    lcd.print("(list of states)   ");
    if (Select == 0){
    }
  }
  if (xSelect == 113 && ySelect > 2){
    ySelect = 2;
  }
  //114
  if (xSelect == 114){
    lcd.setCursor(17, 1);
    lcd.print(ySelect);
    lcd.print("/");
    lcd.print("1");
  }
  if (xSelect == 114 && ySelect == 1){
    lcd.setCursor(3, 1);
    lcd.print(language[44]);
    lcd.setCursor(0, 2);
    lcd.print("*");
    lcd.setCursor(1, 2);
    lcd.print(language[38]);
    lcd.setCursor(0, 3);
    lcd.print(" ");
    lcd.setCursor(1, 3);
    lcd.print("                   ");
    if (Select == 0){
    }
  }
  if (xSelect == 114 && ySelect > 1){
    ySelect = 1;
  }
  //1141
  if (xSelect == 1141){
    lcd.setCursor(17, 1);
    lcd.print("   ");
  }
  if (xSelect == 1141 && ySelect == 1){
    lcd.setCursor(3, 1);
    lcd.print(language[39]);
    lcd.setCursor(0, 2);
    lcd.print(" ");
    lcd.setCursor(1, 2);
    lcd.print("                  ");
    lcd.setCursor(1, 2);
    lcd.print(memoryFree());
    lcd.print(" bytes");
    lcd.setCursor(0, 3);
    lcd.print(" ");
    lcd.setCursor(1, 3);
    lcd.print("                  ");
    if (Select == 0){
    }
  }
  if (xSelect == 1141 && ySelect > 1){
    ySelect = 1;
  }
  //12
  if (xSelect == 12){
    lcd.setCursor(17, 1);
    lcd.print(ySelect);
    lcd.print("/");
    lcd.print("2");
  }
  if (xSelect == 12 && ySelect == 1){
    lcd.setCursor(3, 1);
    lcd.print(language[40]);
    lcd.setCursor(0, 2);
    lcd.print("*");
    lcd.setCursor(1, 2);
    lcd.print(language[28]);
    lcd.setCursor(0, 3);
    lcd.print(" ");
    lcd.setCursor(1, 3);
    lcd.print(language[29]);
    if (Select == 0){
    }
  }
  if (xSelect == 12 && ySelect == 2){
    lcd.setCursor(3, 1);
    lcd.print(language[40]);
    lcd.setCursor(0, 2);
    lcd.print(" ");
    lcd.setCursor(1, 2);
    lcd.print(language[28]);
    lcd.setCursor(0, 3);
    lcd.print("*");
    lcd.setCursor(1, 3);
    lcd.print(language[29]);
    if (Select == 0){
    }
  }
  if (xSelect == 12 && ySelect > 2){
    ySelect = 2;
  }
  //13
  if (xSelect == 13){
    lcd.setCursor(17, 1);
    lcd.print(ySelect);
    lcd.print("/");
    lcd.print("6");
  }
  if (xSelect == 13 && ySelect == 1){
    lcd.setCursor(3, 1);
    lcd.print(language[41]);
    lcd.setCursor(0, 2);
    lcd.print("*");
    lcd.setCursor(1, 2);
    lcd.print(language[30]);
    lcd.setCursor(0, 3);
    lcd.print(" ");
    lcd.setCursor(1, 3);
    lcd.print(language[31]);
    if (Select == 0){
    }
  }
  if (xSelect == 13 && ySelect == 2){
    lcd.setCursor(3, 1);
    lcd.print(language[41]);
    if (scrolldirection == 1){
      lcd.setCursor(0, 2);
      lcd.print(" ");
      lcd.setCursor(1, 2);
      lcd.print(language[30]);
      lcd.setCursor(0, 3);
      lcd.print("*");
      lcd.setCursor(1, 3);
      lcd.print(language[31]);
    }
    if (scrolldirection == 0){
      lcd.setCursor(0, 2);
      lcd.print("*");
      lcd.setCursor(1, 2);
      lcd.print(language[31]);
      lcd.setCursor(0, 3);
      lcd.print(" ");
      lcd.setCursor(1, 3);
      lcd.print(language[32]);
    }
    if (Select == 0){
    }
  }
  if (xSelect == 13 && ySelect == 3){
    lcd.setCursor(3, 1);
    lcd.print(language[41]);
    if (scrolldirection == 1){
      lcd.setCursor(0, 2);
      lcd.print(" ");
      lcd.setCursor(1, 2);
      lcd.print(language[31]);
      lcd.setCursor(0, 3);
      lcd.print("*");
      lcd.setCursor(1, 3);
      lcd.print(language[32]);
    }
    if (scrolldirection == 0){
      lcd.setCursor(0, 2);
      lcd.print("*");
      lcd.setCursor(1, 2);
      lcd.print(language[32]);
      lcd.setCursor(0, 3);
      lcd.print(" ");
      lcd.setCursor(1, 3);
      lcd.print(language[33]);
    }
    if (Select == 0){
    }
  }
  if (xSelect == 13 && ySelect == 4){
    lcd.setCursor(3, 1);
    lcd.print(language[41]);
    if (scrolldirection == 1){
      lcd.setCursor(0, 2);
      lcd.print(" ");
      lcd.setCursor(1, 2);
      lcd.print(language[32]);
      lcd.setCursor(0, 3);
      lcd.print("*");
      lcd.setCursor(1, 3);
      lcd.print(language[33]);
    }
    if (scrolldirection == 0){
      lcd.setCursor(0, 2);
      lcd.print("*");
      lcd.setCursor(1, 2);
      lcd.print(language[33]);
      lcd.setCursor(0, 3);
      lcd.print(" ");
      lcd.setCursor(1, 3);
      lcd.print(language[34]);
    }
    if (Select == 0){
    }
  }
  if (xSelect == 13 && ySelect == 5){
    lcd.setCursor(3, 1);
    lcd.print(language[41]);
    if (scrolldirection == 1){
      lcd.setCursor(0, 2);
      lcd.print(" ");
      lcd.setCursor(1, 2);
      lcd.print(language[33]);
      lcd.setCursor(0, 3);
      lcd.print("*");
      lcd.setCursor(1, 3);
      lcd.print(language[34]);
    }
    if (scrolldirection == 0){
      lcd.setCursor(0, 2);
      lcd.print("*");
      lcd.setCursor(1, 2);
      lcd.print(language[34]);
      lcd.setCursor(0, 3);
      lcd.print(" ");
      lcd.setCursor(1, 3);
      lcd.print(language[35]);
    }
    if (Select == 0){
    }
  }
  if (xSelect == 13 && ySelect == 6){
    lcd.setCursor(3, 1);
    lcd.print(language[41]);
    lcd.setCursor(0, 2);
    lcd.print(" ");
    lcd.setCursor(1, 2);
    lcd.print(language[34]);
    lcd.setCursor(0, 3);
    lcd.print("*");
    lcd.setCursor(1, 3);
    lcd.print(language[35]);
    if (Select == 0){
    }
  }
  if (xSelect == 13 && ySelect > 6){
    ySelect = 6;
  }
  //136
  if (xSelect == 136){
    lcd.setCursor(17, 1);
    lcd.print(ySelect);
    lcd.print("/");
    lcd.print("2");
  }
  if (xSelect == 136 && ySelect == 1){
    lcd.setCursor(3, 1);
    lcd.print(language[42]);
    lcd.setCursor(0, 2);
    lcd.print("*");
    lcd.setCursor(1, 2);
    lcd.print(language[36]);
    lcd.setCursor(0, 3);
    lcd.print(" ");
    lcd.setCursor(1, 3);
    lcd.print(language[37]);
    if (Select == 0){
      for (int lst = 0; lst <= lsl; lst++)  {
      language[lst] = english[lst];
    }
    languageDefault = 0;
    }
  }
  if (xSelect == 136 && ySelect == 2){
    lcd.setCursor(3, 1);
    lcd.print(language[42]);
    lcd.setCursor(0, 2);
    lcd.print(" ");
    lcd.setCursor(1, 2);
    lcd.print(language[36]);
    lcd.setCursor(0, 3);
    lcd.print("*");
    lcd.setCursor(1, 3);
    lcd.print(language[37]);
    if (Select == 0){
for (int lst = 0; lst <= lsl; lst++)  {
      language[lst] = german[lst];
    }    
  }
  languageDefault = 1;
  }
  if (xSelect == 136 && ySelect > 2){
    ySelect = 2;
  }
  Select = 1;
}


