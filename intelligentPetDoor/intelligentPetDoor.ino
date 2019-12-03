//includes



//#include <TimerOne.h>

//constants

#define BUTTON_PIN_0 	( 2 )
#define BUTTON_PIN_1 	( 3 )

#define SERVO_PIN_0		( 9 )
#define SERVO_PIN_1		( 10 )
#define SERVO_PIN_2		( 11 )







/*typedef struct scheduleItem {
	unsigned char day;
	unsigned char hour;
	unsigned char minute;
	signed char movement = -1;
} shceduleItem;
*/



int npets = 1; //hardcoded!!!

//variables







//scheduleItem schedule[MAX_SCHEDULE];






//debounce


//setup
void setup() {
	// initialize servos  
	initServo();

	//initalitze interrupts
	initISR();


	//initialize serial
	Serial.begin(9600);

	//initialize eeprom
	initEEPROM();

	//initialize date stuff
	initRTC();
	
	//initialize check schedule
	initCheckSchedule();
	  
    //Initialize timer one
    //Timer1.initialize(1000000);//1 seconds
    //Timer1.attachInterrupt(timerOneISR);

	//initialize wifi module
	
	Serial.print("buttonPin0 (IN->OUT): ");
	Serial.print(digitalRead(BUTTON_PIN_0));
	Serial.print(" buttonPin1 (OUT->IN): ");
	Serial.println(digitalRead(BUTTON_PIN_1));


}

void loop() {
	
	checkCommand();
	checkNextAction();
	checkSchedule();
	checkButtonsISR();

	//delay(100);
  
}
