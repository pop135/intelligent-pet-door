#include <Servo.h>

#define SERVO_OPEN		( 90 )
#define SERVO_CLOSE_180	( 180 )
#define SERVO_CLOSE_0	( 0 )

Servo Servo0, Servo1, Servo2;

int servoState0, servoState1, servoState2;


void initServo(){
	Servo0.attach(SERVO_PIN_0);
	Servo1.attach(SERVO_PIN_1);
	Servo2.attach(SERVO_PIN_2);
	
	/*Servo0.write(SERVO_CLOSE_180);
	Servo1.write(SERVO_CLOSE_0);
	Servo2.write(SERVO_CLOSE_180);
	
	delay(2000);
	
	Servo0.write(SERVO_OPEN);
	Servo1.write(SERVO_OPEN);
	Servo2.write(SERVO_OPEN);

	delay(2000);*/

	//Servo0.detach();
	//Servo1.detach();
	//Servo2.detach();
	
	servoState0 = SERVO_OPEN;
	servoState1 = SERVO_OPEN;
	servoState2 = SERVO_OPEN;
}

void open(){
	if(servoState0 != SERVO_OPEN){
		Servo0.write(SERVO_OPEN);
		servoState0 = SERVO_OPEN;
	}
	
	if(servoState1 != SERVO_OPEN){
		Servo1.write(SERVO_OPEN);
		servoState1 = SERVO_OPEN;
	}
	
	if(servoState2 != SERVO_OPEN){
		Servo2.write(SERVO_OPEN);
		servoState2 = SERVO_OPEN;
	}
	
	//wait servos movment
	delay(2000);
}

void in(){
	if(servoState0 == SERVO_OPEN){
		Servo0.write(SERVO_CLOSE_180);
		servoState0 = SERVO_CLOSE_180;
	}
	
	if(servoState1 != SERVO_OPEN){
		Servo1.write(SERVO_OPEN);
		servoState1 = SERVO_OPEN;
	}
	
	if(servoState2 != SERVO_OPEN){
		Servo2.write(SERVO_OPEN);
		servoState2 = SERVO_OPEN;
	}
	
	//wait servos movment
	delay(2000);
}

void out(){
	if(servoState0 != SERVO_OPEN){
		Servo0.write(SERVO_OPEN);
		servoState0 = SERVO_OPEN;
	}
	
	if(servoState1 == SERVO_OPEN){
		Servo1.write(SERVO_CLOSE_0);
		servoState1 = SERVO_CLOSE_0;
	}
	
	if(servoState2 == SERVO_OPEN){
		Servo2.write(SERVO_CLOSE_180);
		servoState2 = SERVO_CLOSE_180;
	}
	
	//wait servos movment
	delay(2000);
}

void close(){
	if(servoState0 == SERVO_OPEN){
		Servo0.write(SERVO_CLOSE_180);
		servoState0 = SERVO_CLOSE_180;
	}
	
	if(servoState1 == SERVO_OPEN){
		Servo1.write(SERVO_CLOSE_0);
		servoState1 = SERVO_CLOSE_0;
	}
	
	if(servoState2 == SERVO_OPEN){
		Servo2.write(SERVO_CLOSE_180);
		servoState2 = SERVO_CLOSE_180;
	}
	
	//wait servos movment
	delay(2000);

}