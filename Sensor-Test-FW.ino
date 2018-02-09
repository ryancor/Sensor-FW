#include "TimeLib.h"

//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 35;       
 
//the time when the sensor outputs a low impulse
long unsigned int lowIn;        
 
//the amount of milliseconds the sensor has to be low
//before we assume all motion has stopped
long unsigned int pause = 5000; 
 
boolean lockLow = true;
boolean takeLowTime; 
 
int pirPin = 3;    //the digital pin connected to the PIR sensor's output
int ledPin = 13;
 
 
/////////////////////////////
//SETUP
void setup(){
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(pirPin, LOW);
 
  //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
      }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    delay(50);
  }
 
////////////////////////////
//LOOP
void loop(){
     
     if(digitalRead(pirPin) == HIGH){
       digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
       if(lockLow){ 
         //makes sure we wait for a transition to LOW before any further output is made:
         lockLow = false;           
         Serial.println("---");
         Serial.print("Ringo's motion detected at ");
         digitalClockDisplay();
         //Serial.print(millis()/1000);
         //Serial.println(" sec ");
         delay(50);
         }        
         takeLowTime = true;
       }
 
     if(digitalRead(pirPin) == LOW){      
        digitalWrite(ledPin, LOW);  //the led visualizes the sensors output pin state
 
     if(takeLowTime) {
        lowIn = millis();          //save the time of the transition from high to LOW
        takeLowTime = false;       //make sure this is only done at the start of a LOW phase
      }
      
       //if the sensor is low for more than the given pause,
       //we assume that no more motion is going to happen
      if(!lockLow && millis() - lowIn > pause) { 
          //makes sure this block of code is only executed again after
          //a new motion sequence has been detected
          lockLow = true;                       
          Serial.print("Ringo's motion ended at ");      //output
          digitalClockDisplay();
          delay(50);
      }
   }
}

void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(month()+1);
  Serial.print("/");
  Serial.print(day()+6);
  Serial.print("/");
  Serial.print(year()+48); 
  Serial.println(); 
}

void printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
