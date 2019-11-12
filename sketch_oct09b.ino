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
int const servoPin2 = 11;
int const ledPin = 13;

int const servoOpen = 90;
int const servoClose180 = 180;
int const servoClose0 = 0;

int const MAX_SCHEDULE = 50;

//hardcoded!!!
int const npets = 3;

//variables
volatile int buttonState0, buttonState1 = 0;
int servoState0, servoState1, servoState2;
volatile int npetsIn = npets;
volatile int npetsOut = 0;
volatile commandIssued command;
volatile nextAction action;

Servo Servo0;
Servo Servo1;
Servo Servo2;

scheduleItem schedule[MAX_SCHEDULE];
int nschedule = 0;

void setup() {
	// initialize servos  
	Servo0.attach(servoPin0);
	Servo0.write(servoOpen);
	delay(1000);
	Servo0.detach();
	
	Servo1.attach(servoPin1);
	Servo1.write(servoOpen);
	delay(1000);
	Servo1.detach();
	
	Servo2.attach(servoPin2);
	Servo2.write(servoOpen);
	delay(1000);
	Servo2.detach();
	
	Servo0.attach(servoPin0);
	Servo0.write(servoClose180);
	delay(1000);
	Servo0.detach();
	
	Servo1.attach(servoPin1);
	Servo1.write(servoClose0);
	delay(1000);
	Servo1.detach();
	
	Servo2.attach(servoPin2);
	Servo2.write(servoClose180);
	delay(1000);
	Servo2.detach();


	/*servoState0 = servoOpen;
	servoState1 = servoOpen;
	servoState2 = servoOpen;
	delay(3000);
	Servo0.write(servoClose180);
	Servo1.write(servoClose0);
	Servo2.write(servoClose180);
	servoState0 = servoClose180;
	servoState1 = servoClose0;
	servoState2 = servoClose180;
	delay(3000);
	
	Servo1.detach();
	Servo2.detach();*/

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
			free();
		}
		//in (servo0 = open and servo1 = close)
		else if(command.movement == 1){
			Serial.println(F("in"));
			in();
		}
		//out (servo0 = close and servo1 = open)
		else if(command.movement == 2){
			Serial.println(F("out"));
			out();
		}
		//close (servo0 = close and servo 1 = close)
		else if(command.movement == 3){
			Serial.println(F("close"));
			close();
		}
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

	if(command.commandName == 2){
		Serial.println(F("You entered: /where "));
		if(npetsIn == npets) Serial.println("All of your pets are inside.");
		else if (npetsIn == npets) Serial.println("All of your pets are outside.");
		else{
			Serial.print("There are ");
			Serial.print(npetsIn);
			Serial.print(" pet");
			if(npetsIn!=1) {Serial.print("s");}
			Serial.print(" inside and ");
			Serial.print(npetsOut);
			Serial.print(" pet");
			if(npetsOut!=1) {Serial.print("s");}
			Serial.println(" outside.");
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

	/*if(buttonState0 || buttonState1){
		doAction();
		buttonState0 = 0;
		buttonState1 = 0;
	}*/
	doAction();

	delay(500);
  
}

void doAction(){
	if((action.movement != -1) && (action.npetsIn == npetsIn) && (action.npetsOut == npetsOut)){
		Serial.print("Conditions met. Programmed action ");
		if(action.movement == 0){
			Serial.print(F("free"));
			free();
		}
		//in (servo0 = open and servo1 = close)
		else if(action.movement == 1){
			Serial.print(F("in"));
			in();
		}
		//out (servo0 = close and servo1 = open)
		else if(action.movement == 2){
			Serial.print(F("out"));
			out();
		}
		//close (servo0 = close and servo 1 = close)
		else if(action.movement == 3){
			Serial.print(F("close"));
			close();
		}
		Serial.println(" done.");
		//command done so clear it
		clearNextAction();
	}
}

//in->out button
void buttonPressed0(){
	if(npetsIn > 0) {
		npetsIn--;
		npetsOut++;
		//buttonState0 = 1;
	}
	Serial.print("Button IN->OUT pressed. Pets IN: ");
	Serial.print(npetsIn);
	Serial.print(". Pets OUT: ");
	Serial.print(npetsOut);
	Serial.println(".");



}

//out->in button
void buttonPressed1(){
	if(npetsOut > 0) {
		npetsOut--;
		npetsIn++;
		//buttonState1 = 1;
	}
	Serial.print("Button OUT->IN pressed. Pets IN: ");
	Serial.print(npetsIn);
	Serial.print(". Pets OUT: ");
	Serial.print(npetsOut);
	Serial.println(".");

}

void free(){
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
	//wait servos movment
	delay(2000);
	//detach if proceed
	if(Servo0.attached()){Servo0.detach();}
	if(Servo1.attached()){Servo1.detach();}
}

void in(){
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
		Servo1.write(servoClose0);
		servoState1 = servoClose0;
	}
	else{
		
	}
	//wait servos movment
	delay(2000);
	//detach if proceed
	if(Servo0.attached()){Servo0.detach();}
	if(Servo1.attached()){Servo1.detach();}
}

void out(){
	if(servoState0 == servoOpen){
		Servo0.attach(servoPin0);
		Servo0.write(servoClose180);
		servoState0 = servoClose180;
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
	//wait servos movment
	delay(2000);
	//detach if proceed
	if(Servo0.attached()){Servo0.detach();}
	if(Servo1.attached()){Servo1.detach();}
}

void close(){
	if(servoState0 == servoOpen){
		Servo0.attach(servoPin0);
		Servo0.write(servoClose180);
		servoState0 = servoClose180;
	}
	else{
		
	}
	if(servoState1 == servoOpen){
		Servo1.attach(servoPin1);
		Servo1.write(servoClose0);
		servoState1 = servoClose0;
	}
	else{
		
	}
	//wait servos movment
	delay(2000);
	//detach if proceed
	if(Servo0.attached()){Servo0.detach();}
	if(Servo1.attached()){Servo1.detach();}
}
