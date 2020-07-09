#include <dht.h>
#include <MyRealTimeClock.h>
#include <TimeLib.h> 
#include <LiquidCrystal.h>

#define DHT11_PIN 9
dht DHT;

MyRealTimeClock myRTC(11, 12, 13); 
LiquidCrystal lcd(8, 7, 5, 4, 3, 2);

const int led = 6;
const int buzzer = 10;
const int blinkTime = 20; 
const int oneSecond = 1000;
const int oneMin = 60;
const int numTones = 2;
const int tones[] = {261, 277, 294, 311, 330, 349, 370, 392, 415, 440}; 
const String days[7] = { "MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN" };

String temp ="";
int counterMin = 0;
int counter = 0;


// ----------------------------------------------------------------------------
void setup() {  

  // seconds, minutes, hours, day of the week, day of the month, month, year
  // myRTC.setDS1302Time(00, 35, 11, 4, 9, 7, 2020);
                 
  Serial.begin(9600);
  Serial.println("starting application");
  
  lcd.begin(16, 2);  
  lcd.print("starting...");
  
  pinMode(led, OUTPUT); 
  pinMode(buzzer, OUTPUT);   
  
  startTone();
  delay(oneSecond);
  blinkTone();

} 

// ----------------------------------------------------------------------------
void loop() {  

  increaseMins(counter);
  myRTC.updateTime();
  
  turnOnOffLed(counter);    
  lcdPrint(0,0, getTime());    
  lcdPrint(0,1, getTemp(counter));
  blinking(blinkTime, counterMin);  
  lcdPrintBlink(counterMin);  

  delay(oneSecond);  
  counter++;
  resetCounter(); 
  resetMins();
  
}

// ----------------------------------------------------------------------------
void lcdPrintBlink(int c){
  lcd.setCursor(10, 1);
  lcd.print("B:");
  lcd.setCursor(12, 1);
  lcd.print(getMins(c));
}

// ----------------------------------------------------------------------------
void increaseMins(int c){
  
  if (c == (oneMin -1)){ // 59 seconds
    counterMin++;  
  }
  
}

// ----------------------------------------------------------------------------
void resetMins(){
  
  if (counterMin == blinkTime) {
    counterMin = 0;
  }
    
}

// ----------------------------------------------------------------------------
void resetCounter(){
  
  if (counter == oneMin) {
    counter = 0;
  }
    
}

// ----------------------------------------------------------------------------
String getTemp(int counter) {

  String tmp = "";
  if (counter == 0 || (counter % 30) == 0){
    DHT.read11(DHT11_PIN);
    int t = DHT.temperature;
    int h = DHT.humidity;    
    tmp = String("T:" + String(t) + " H:" + String(h));      
    temp = tmp;
  } else {
    tmp = temp;
  }  
  
  return tmp;
  
}

// ----------------------------------------------------------------------------
String getTime() {  
  return String(getDayOfTheWeek() + " "+ myRTC.dayofmonth + " " + getHours() + ":" + getMinutes() + ":" + getSeconds());
}

// ----------------------------------------------------------------------------
void blinking(int blinkMins, int cm) {   
  if (cm == blinkMins) {
    blinkTone();    
  }  
}

// ----------------------------------------------------------------------------
void tone20(int buzzer, int soundTone){
  tone(buzzer, soundTone);
  delay(500);
  noTone(buzzer);  
}

// ----------------------------------------------------------------------------
void blinkTone(){
   tone20(buzzer, 100);
   tone20(buzzer, 200);  
   tone20(buzzer, 450);     
}

// ----------------------------------------------------------------------------
void turnOnLed(int led){
  digitalWrite(led, HIGH);
}

// ----------------------------------------------------------------------------
void turnOffLed(int led){
  digitalWrite(led, LOW);
}

// ----------------------------------------------------------------------------
void lcdPrint(int row, int col, String text){
  lcd.setCursor(row, col);
  lcd.print(text);
}

// ----------------------------------------------------------------------------
void turnOnOffLed(int counter){
  if ((counter % 2) == 0){
    turnOnLed(led);
  } else {
    turnOffLed(led);
  }
}

// ----------------------------------------------------------------------------
String getSeconds() {
  int seconds = myRTC.seconds;
  String sec;
  if (seconds < 10){
    sec = String("0" + String(seconds));
  } else {
    sec = String(seconds);
  }
  return sec;
}

// ----------------------------------------------------------------------------
String getMinutes() {
  int minutes = myRTC.minutes;
  String mins;
  if (minutes < 10){
    mins = String("0" + String(minutes));
  } else {
    mins = String(minutes);
  }
  return mins;
}

// ----------------------------------------------------------------------------
String getHours() {

  int hours = myRTC.hours;
  if (hours > 12) {
    hours = hours - 12;
  }
  
  String hr;
  if (hours < 10){
    hr = String("0" + String(hours));
  } else {
    hr = String(hours);
  }
  return hr;
}

// ----------------------------------------------------------------------------
String getDayOfTheWeek() {
  int dayOfTheWeek = myRTC.dayofweek;  
  return days[dayOfTheWeek -1];
}

// ----------------------------------------------------------------------------
void startTone() {
  for (int i = 0; i < numTones; i++) {
      tone(buzzer, tones[i]);
      delay(100);
   }
   noTone(buzzer);
}

// ----------------------------------------------------------------------------
String getMins(int c) {
  String mins;
  if (c < 10){
    mins = String("0" + String(c));
  } else {
    mins = String(c);
  }
  return mins;
}
