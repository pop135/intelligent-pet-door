//includes
#include <Servo.h>

//type definitions
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


int const npets = 3; //hardcoded!!!

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

//setup
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
	
	checkCommand();
	checkNextAction();

	delay(500);
  
}