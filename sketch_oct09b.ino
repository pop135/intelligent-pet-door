//includes
#include <Servo.h>
//#include "ParseInput.h"

typedef struct command {
  int commandName;
  int hour;
  int minute;
  int movement;
} commandIssued; //jjjhgk

//constants
int const buttonPin0 = 2;
int const buttonPin1 = 3;

int const servoPin0 = 9;
int const servoPin1 = 10;
int const ledPin = 13;

int const servoOpen = 0;
int const servoClose = 90;

//hardcoded!!!
int const npets = 3;

//variables
int buttonState0, buttonState1 = 0;
int servoState0, servoState1 = 0;
int npetsIn = npets;
int npetsOut = 0;
commandIssued command;

Servo Servo0;
Servo Servo1;

void setup() {
  // initialize servos  
  Servo0.attach(servoPin0);
  Servo1.attach(servoPin1);
  Servo0.write(servoOpen);
  Servo1.write(servoOpen);
  delay(2000);
  Servo0.write(servoClose);
  Servo1.write(servoClose);
  delay(2000);
  Servo0.detach();
  Servo1.detach();

  //initialize led
  pinMode(ledPin, OUTPUT);

  //initialize serial
  Serial.begin(9600);

  //initalitze interrupts
  attachInterrupt(digitalPinToInterrupt(buttonPin0), buttonPressed0, RISING);
  attachInterrupt(digitalPinToInterrupt(buttonPin1), buttonPressed1, RISING);

  //initialize wifi module

  //initialize date module


}

void loop() {


	command = parseInput();
	if(command.commandName != -1){
		Serial.print("You entered: /");
		if(command.commandName ==0){Serial.print("now ");}
		else if (command.commandName ==1){Serial.print("program ");}
		if(command.hour != -1){
			Serial.print(command.hour);
			Serial.print(":");
			Serial.print(command.minute);
			Serial.print(" ");
		}
		if(command.movement == 0){Serial.println("free");}
		else if(command.movement == 1){Serial.println("in");}
		else if(command.movement == 2){Serial.println("out");}
		else if(command.movement == 3){Serial.println("close");}
	}
	delay(500);

  /*buttonState0 = digitalRead(buttonPin0);
  buttonState1 = digitalRead(buttonPin1);
  //servoState = myServo.read();

  if(buttonState0 == HIGH){
   
    Servo0.attach(servoPin0);
    digitalWrite(ledPin,HIGH);
    
    if(servoState0 == close){
      Servo0.write(open);
      servoState0 = open;
    }
    else if(servoState0 == open){
      Servo0.write(close);
      servoState0 = close;
    }
    delay(1000);
    Servo0.detach();
    digitalWrite(ledPin, LOW);
    Serial.print("Button: ");
    Serial.print(buttonState0);
    Serial.print(" Servo: ");
    Serial.println(servoState0);
  }*/




  
}

  void buttonPressed0(){
    Serial.println("Button 0: Pressed");
    
    //if(SomethingToDo()){
    //  }
  }

  void buttonPressed1(){
    Serial.println("Button 1: Pressed");
  }
