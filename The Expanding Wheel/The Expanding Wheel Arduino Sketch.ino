#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);
// You can also make another motor on port M2
//Adafruit_DCMotor *myOtherMotor = AFMS.getMotor(2);

const int pwPin1 = 2; //ultrasonic sensor
long sensor1, cm, stage, location = 0;
bool closed = true;
int slider;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pwPin1, INPUT);

  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  // turn on motor
  myMotor->run(RELEASE);

  Serial.println("Options Available: ");
  Serial.println("D - Default Mode: Control Wheel with Ultrasonic Sensor ");
  Serial.println("S - Slider Mode: Use Slider to Control Wheel");
  Serial.println("T - Timer Mode: Open and Close for a set time");
  Serial.println("U - Ultrasonic Senosr Debugging Mode");
  Serial.println("Enter the character corresponding to the mode you want to enter: ");
}

void read_sensor(){
  sensor1= pulseIn(pwPin1, HIGH);
  cm = sensor1/58; 
}

void loop() {
  // put your main code here, to run repeatedly:
  char Request = Serial.read();
  
  //Default Mode - Using Ultrasonic Sensor
  if (Request == 'D' || Request == 'd') {
  Serial.println("Entering Default Mode...");
    
    while (true) {
      // Old Method
      read_sensor();
      
      if (cm > 150) {
        stage = 150;
      }
      else if (cm <50) {
        stage = 50;
      }
      else {
        stage = cm;
      }

      if ((150-stage)>location){
        Serial.println("Closing");
        myMotor->run(FORWARD);
        myMotor->setSpeed(240);
        delay (240);
        location+=5;
      }
      else if((150-stage)<location){
        Serial.println("Opening");
        myMotor->run(BACKWARD);
        myMotor->setSpeed(240);
        delay (200);
        location-=5;
      } else {
        myMotor->setSpeed(0);
      }
      
    }
  }

  //Slider Mode - Use the slider to open and close the wheel
  //Red = Close, Green = Open, Middle = Stop
  else if (Request == 's' || Request == 'S') {
  Serial.println("Entering Slider Mode...");
  Serial.println("Red - Close, Middle - No Movement, Green - Open");
    
    while (true) {
      slider = analogRead(0);
     
      if (slider < 250) {
        Serial.println("Forward");
        myMotor->run(FORWARD);
        myMotor->setSpeed(150);
        
        while (slider <250) {
        slider = analogRead(0);
        }
      }
    
      else if (slider < 420) {
        Serial.println("No Movement");
        myMotor->setSpeed(0);
        
        while (slider <420 && slider >= 250) {
         slider = analogRead(0);
        }
      }
    
      else {
        Serial.println("Backward");
        myMotor->run(BACKWARD);
        myMotor->setSpeed(150);
        
        while (slider >=420) {
          slider = analogRead(0);
        }
      }
      
    }// end while
  }// end else if

   //Timer Mode - Open and close for a time duration
   else if (Request == 't' || Request == 'T') {
   Serial.println("Entering Timer Mode...");
     
      //Change this value to set time (Milliseconds)
      int timeValue = 5500;
       
      while(true) {
        Serial.println("Backward");
        myMotor->run(BACKWARD);
        myMotor->setSpeed(240);
        delay (timeValue);
        myMotor->setSpeed(0);
        delay (2000);
  
        Serial.println("Forward");
        myMotor->run(FORWARD);
        myMotor->setSpeed(240);
        delay (timeValue);
        myMotor->setSpeed(0);
        delay (500);      

        //Enter R in Serial to Repeat 
        while (Request != 'R') { 
          Request = Serial.read();
        }
        Request = 'T';
     }
   }

   else if (Request == 'u' || Request == 'U') {
   Serial.println("Entering Ultrasonic Sensor Debugging...");
      
      while(true) {
        read_sensor();
        printall();
        delay (500);
      }
   }
  
}// end loop

//Used for Debugging Ultrasonic Sensor
void printall(){
  Serial.print("S1");
  Serial.print(" = ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
}
