//includes
#include <Servo.h>
//#include "ParseInput.h"

typedef struct commandIssued {
	int commandName = -1; // 0 = now, 1 = program
	int hour = -1;
	int minute = -1;
	int movement = -1; // 0 = free, 1 = in, 2 = out, 3 = close
	int modifierFlag = -1;
} commandIssued; 

typedef struct nextAction {
	int npetsIn = -1;
	int npetsOut = -1;
	int movement = -1; // 0 = free, 1 = in, 2 = out, 3 = close
} nextAction;

typedef struct scheduleItem {
	unsigned char hour;
	unsigned char minute;
	signed char movement = -1;
} shceduleItem;

//constants
int const buttonPin0 = 2;
int const buttonPin1 = 3;

int const servoPin0 = 9;
int const servoPin1 = 10;
int const ledPin = 13;

int const servoOpen = 0;
int const servoClose = 90;

int const MAX_SCHEDULE = 50;

//hardcoded!!!
int const npets = 3;

//variables
int buttonState0, buttonState1 = 0;
int servoState0, servoState1;
int npetsIn = npets;
int npetsOut = 0;
volatile commandIssued command;
volatile nextAction action;

Servo Servo0;
Servo Servo1;

scheduleItem schedule[MAX_SCHEDULE];
int nschedule = 0;

void setup() {
	// initialize servos  
	Servo0.attach(servoPin0);
	Servo1.attach(servoPin1);
	Servo0.write(servoOpen);
	Servo1.write(servoOpen);
	servoState0 = servoOpen;
	servoState1 = servoOpen;
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
	
	/*if(action.movement != -1){
		Serial.print("You entered: /when ");
		if((action.npetsIn + action.npetsOut) == npets){
			if(action.npetsIn == 0){
				Serial.print("allout ");
			}
			else if(action.npetsIn != 0){
				Serial.print(action.npetsIn);
				Serial.print(" in ");
				Serial.print(action.npetsOut);
				Serial.print(" out ");
			}
			else{
				Serial.print("allin ");
			}
		}
		if(action.movement == 0){Serial.println("free");}
		else if(action.movement == 1){Serial.println("in");}
		else if(action.movement == 2){Serial.println("out");}
		else if(action.movement == 3){Serial.println("close");}
	}*/



	if(command.commandName == 0){
		Serial.print(F("You entered: /now "));
		if(command.movement == 0){
			Serial.println(F("free"));
			if(servoState0 == servoOpen){
				//servo is already open
			}
			else{
				Servo0.attach(servoPin0);
				Servo0.write(servoOpen);
				servoState0 = servoOpen;
			}
			if(servoState1 == servoOpen){
				
			}
			else{
				Servo1.attach(servoPin1);
				Servo1.write(servoOpen);
				servoState1 = servoOpen;
			}
		}
		//in (servo0 = open and servo1 = close)
		else if(command.movement == 1){
			Serial.println(F("in"));
			if(servoState0 == servoOpen){
				//servo is already open
			}
			else{
				Servo0.attach(servoPin0);
				Servo0.write(servoOpen);
				servoState0 = servoOpen;
			}
			if(servoState1 == servoOpen){
				Servo1.attach(servoPin1);
				Servo1.write(servoClose);
				servoState1 = servoClose;
			}
			else{
				
			}
		}
		//out (servo0 = close and servo1 = open)
		else if(command.movement == 2){
			Serial.println(F("out"));
			if(servoState0 == servoOpen){
				Servo0.attach(servoPin0);
				Servo0.write(servoClose);
				servoState0 = servoClose;
			}
			else{
				
			}
			if(servoState1 == servoOpen){
				
			}
			else{
				Servo1.attach(servoPin1);
				Servo1.write(servoOpen);
				servoState1 = servoOpen;
			}
		}
		//close (servo0 = close and servo 1 = close)
		else if(command.movement == 3){
			Serial.println(F("close"));
			if(servoState0 == servoOpen){
				Servo0.attach(servoPin0);
				Servo0.write(servoClose);
				servoState0 = servoClose;
			}
			else{
				
			}
			if(servoState1 == servoOpen){
				Servo1.attach(servoPin1);
				Servo1.write(servoClose);
				servoState1 = servoClose;
			}
			else{
				
			}
		}
		//wait servos movment
		delay(2000);
		//detach if proceed
		if(Servo0.attached()){Servo0.detach();}
		if(Servo1.attached()){Servo1.detach();}
		//command done so clear it
		clearCommand();
	}
	
	if(command.commandName == 1){
		if(command.modifierFlag == 2){
			Serial.println(F("You entered: /program show"));
			if(nschedule >0){
				Serial.println(F("Stored commands:"));
				Serial.println(F(" Hour   Command"));
				Serial.println(F("----------------"));
				for(int i=0;i<MAX_SCHEDULE;i++){
					if(schedule[i].movement != -1){
						Serial.print(" ");
						Serial.print(schedule[i].hour);
						Serial.print(":");
						if(schedule[i].minute<10) Serial.print(F("0"));
						Serial.print(schedule[i].minute);
						Serial.print("  ");
						if(schedule[i].movement == 0){Serial.println("free");}
						else if(schedule[i].movement == 1){Serial.println("in");}
						else if(schedule[i].movement == 2){Serial.println("out");}
						else if(schedule[i].movement == 3){Serial.println("close");}
					}
				}
			}
			else{
				Serial.println("No scheduled commands.");
			}
		}
		else{
			signed char found = -1;
			for(int i=0;i<MAX_SCHEDULE;i++){
				if((schedule[i].hour == command.hour) && (schedule[i].minute == command.minute) && (schedule[i].movement != -1)){
					found = i;
				}
			}

			if(command.modifierFlag == 0){
				Serial.print(F("You entered: /program "));
				Serial.print(command.hour);
				Serial.print(F(":"));
				if(command.minute<10) Serial.print(F("0"));
				Serial.print(command.minute);
				if(command.movement == 0){Serial.println(F(" free"));}
				else if(command.movement == 1){Serial.println(F(" in"));}
				else if(command.movement == 2){Serial.println(F(" out"));}
				else if(command.movement == 3){Serial.println(F(" close"));}
				if(found == -1){
					for(int i=0;i<MAX_SCHEDULE;i++){
						if(schedule[i].movement == -1){
							schedule[i].hour = command.hour;
							schedule[i].minute = command.minute;
							schedule[i].movement = command.movement;
							nschedule++;
							break;
						}
					}
				}
				else{
					Serial.println("You already have a scheduled command at that time. Delete it before.");
				}
			}
			else if(command.modifierFlag == 1){
				Serial.print(F("You entered: /program del "));
				Serial.print(command.hour);
				Serial.print(F(":"));
				Serial.println(command.minute);
				if(found != -1){
					schedule[found].movement = -1;
					nschedule--;
				}
				else{
					Serial.println("Schedule to delete not found. Use \"/program show\" to see stored commands.");
				}
			}
		}
		//command done so clear it
		clearCommand();
	}



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



	clearCommand();
	delay(500);
  
}

//in->out button
void buttonPressed0(){
	if(npetsIn > 0) {
		npetsIn--;
		npetsOut++;
	}
	Serial.print("Button IN->OUT pressed. Pets IN: ");
	Serial.print(npetsIn);
	Serial.print(". Pets OUT: ");
	Serial.print(npetsOut);
	Serial.println(".");


//if(SomethingToDo()){
//  }
}

//out->in button
void buttonPressed1(){
	if(npetsOut > 0) {
		npetsOut--;
		npetsIn++;
	}
	Serial.print("Button OUT->IN pressed. Pets IN: ");
	Serial.print(npetsIn);
	Serial.print(". Pets OUT: ");
	Serial.print(npetsOut);
	Serial.println(".");

}

void somethingToDo(){
	if(action.movement != -1){
		
	}
}




