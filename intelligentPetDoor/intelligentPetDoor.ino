//includes
#include <Servo.h>
#include <EEPROM.h>
#include "RTClib.h"
//#include <TimerOne.h>

//type definitions
typedef struct commandIssued {
	int commandName = -1; // 0 = now, 1 = program
	int day = -1;
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
	unsigned char day;
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

int const MAX_SCHEDULE = 100;


int const npets = 2; //hardcoded!!!

//variables
int buttonState0, buttonState1 = 0;
int servoState0, servoState1, servoState2;
volatile int npetsIn = npets;
volatile int npetsOut = 0;
commandIssued command;
nextAction action;

Servo Servo0, Servo1, Servo2;

scheduleItem schedule[MAX_SCHEDULE];
int nschedule=0;

RTC_DS3231 rtc;

///checkschedule variables to allow only one check per minute
int minuteBefore;
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 1000;  //the value is a number of milliseconds

//debounce


//setup
void setup() {
	// initialize servos  
	Servo0.attach(servoPin0);
	Servo1.attach(servoPin1);
	Servo2.attach(servoPin2);
	
	Servo0.write(servoClose180);
	Servo1.write(servoClose0);
	Servo2.write(servoClose180);
	
	delay(2000);
	
	Servo0.write(servoOpen);
	Servo1.write(servoOpen);
	Servo2.write(servoOpen);

	delay(2000);

	//Servo0.detach();
	//Servo1.detach();
	//Servo2.detach();
	
	servoState0 = servoOpen;
	servoState1 = servoOpen;
	servoState2 = servoOpen;

	//initalitze interrupts
	attachInterrupt(digitalPinToInterrupt(buttonPin0), buttonInOutISR, FALLING);
	attachInterrupt(digitalPinToInterrupt(buttonPin1), buttonOutInISR, RISING);

	//initialize serial
	Serial.begin(9600);

	//initialize eeprom
	for(int i=2;i<(MAX_SCHEDULE*2);i=i+2){
		if((EEPROM.read(i)&0x3F) != 0x3F){
			nschedule++;
		}
	}

	//initialize date stuff
	rtc.begin();
	minuteBefore = (rtc.now()).minute()-1;
	startMillis=millis();
	  
    //Initialize timer one
    //Timer1.initialize(1000000);//1 seconds
    //Timer1.attachInterrupt(timerOneISR);

	//initialize wifi module
	


}

void loop() {
	
	checkCommand();
	checkNextAction();
	checkSchedule();
	checkButtonsISR();

	//delay(100);
  
}
