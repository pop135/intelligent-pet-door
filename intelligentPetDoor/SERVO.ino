/*
 * Copyright (C) 2019 Universitat Oberta de Catalunya - http://www.uoc.edu/
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Universitat Oberta de Catalunya nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*----------INCLUDES-------------------------------------------------------------*/

/* The servo default arduino library */
#include <Servo.h>

/*----------VARIABLES-----------------------------------------------------------*/

/* The servo objects */
Servo Servo0, Servo1, Servo2;

/* To know in which position the servos are */
uint8_t servoState0, servoState1, servoState2;

/*----------FUNCTIONS-----------------------------------------------------------*/

void initServo(){
	
	/* Attach the servo object to the given pcb pin */
	Servo0.attach(SERVO_PIN_0);
	Servo1.attach(SERVO_PIN_1);
	Servo2.attach(SERVO_PIN_2);
	
	/* Section to make sure that the servos are running and are oppened when the program starts */
	/*Servo0.write(SERVO_CLOSE_180);
	Servo1.write(SERVO_CLOSE_0);
	Servo2.write(SERVO_CLOSE_180);*/
	
	delay(2000);
	
	Servo0.write(SERVO_OPEN);
	Servo1.write(SERVO_OPEN);
	Servo2.write(SERVO_OPEN);

	delay(2000);
	
	Servo0.detach();
	Servo1.detach();
	Servo2.detach();
	
	/* Initial servos state */
	servoState0 = SERVO_OPEN;
	servoState1 = SERVO_OPEN;
	servoState2 = SERVO_OPEN;
}

/* Move the servos to open the door */
void open(){
	
	if(servoState0 != SERVO_OPEN){
		Servo0.attach(SERVO_PIN_0);
		Servo0.write(SERVO_OPEN);
		servoState0 = SERVO_OPEN;
	}
	
	if(servoState1 != SERVO_OPEN){
		Servo1.attach(SERVO_PIN_1);
		Servo1.write(SERVO_OPEN);
		servoState1 = SERVO_OPEN;
	}
	
	if(servoState2 != SERVO_OPEN){
		Servo2.attach(SERVO_PIN_2);
		Servo2.write(SERVO_OPEN);
		servoState2 = SERVO_OPEN;
	}
	
	/* Wait servos movement */
	delay(2000);
	
	if(Servo0.attached()) Servo0.detach();
	if(Servo1.attached()) Servo1.detach();
	if(Servo2.attached()) Servo2.detach();
}

/* Move the servos to let pets go in but not go out */
void in(){
	
	if(servoState0 == SERVO_OPEN){
		Servo0.attach(SERVO_PIN_0);
		Servo0.write(SERVO_CLOSE_180);
		servoState0 = SERVO_CLOSE_180;
	}
	
	if(servoState1 != SERVO_OPEN){
		Servo1.attach(SERVO_PIN_1);
		Servo1.write(SERVO_OPEN);
		servoState1 = SERVO_OPEN;
	}
	
	if(servoState2 != SERVO_OPEN){
		Servo2.attach(SERVO_PIN_2);
		Servo2.write(SERVO_OPEN);
		servoState2 = SERVO_OPEN;
	}
	
	/* Wait servos movement */
	delay(2000);
	
	if(Servo0.attached()) Servo0.detach();
	if(Servo1.attached()) Servo1.detach();
	if(Servo2.attached()) Servo2.detach();
}

/* Move the servos to let pets go out but not go in */
void out(){
	
	if(servoState0 != SERVO_OPEN){
		Servo0.attach(SERVO_PIN_0);
		Servo0.write(SERVO_OPEN);
		servoState0 = SERVO_OPEN;
	}
	
	if(servoState1 == SERVO_OPEN){
		Servo1.attach(SERVO_PIN_1);
		Servo1.write(SERVO_CLOSE_0);
		servoState1 = SERVO_CLOSE_0;
	}
	
	if(servoState2 == SERVO_OPEN){
		Servo2.attach(SERVO_PIN_2);
		Servo2.write(SERVO_CLOSE_180);
		servoState2 = SERVO_CLOSE_180;
	}
	
	/* Wait servos movement */
	delay(2000);
	
	if(Servo0.attached()) Servo0.detach();
	if(Servo1.attached()) Servo1.detach();
	if(Servo2.attached()) Servo2.detach();
	
}

/* Move the servos to close the door */
void close(){
	
	if(servoState0 == SERVO_OPEN){
		Servo0.attach(SERVO_PIN_0);
		Servo0.write(SERVO_CLOSE_180);
		servoState0 = SERVO_CLOSE_180;
	}
	
	if(servoState1 == SERVO_OPEN){
		Servo1.attach(SERVO_PIN_1);
		Servo1.write(SERVO_CLOSE_0);
		servoState1 = SERVO_CLOSE_0;
	}
	
	if(servoState2 == SERVO_OPEN){
		Servo2.attach(SERVO_PIN_2);
		Servo2.write(SERVO_CLOSE_180);
		servoState2 = SERVO_CLOSE_180;
	}
	
	/* Wait servos movement */
	delay(2000);
	
	if(Servo0.attached()) Servo0.detach();
	if(Servo1.attached()) Servo1.detach();
	if(Servo2.attached()) Servo2.detach();

}

/* Returns door status */
uint8_t doorStatus(){
	
	/* Open position */
	if((servoState0 == SERVO_OPEN) && (servoState1 == SERVO_OPEN) && (servoState2 == SERVO_OPEN)){
		
		return 0;
		
	}
	
	/* In position */
	else if((servoState0 == SERVO_CLOSE_180) && (servoState1 == SERVO_OPEN) && (servoState2 == SERVO_OPEN)){
		
		return 1;
		
	}
	
	/* Out position */
	else if((servoState0 == SERVO_OPEN) && (servoState1 == SERVO_CLOSE_0) && (servoState2 == SERVO_CLOSE_180)){
		
		return 2;
		
	}
	
	/* Close position */
	else if((servoState0 == SERVO_CLOSE_180) && (servoState1 == SERVO_CLOSE_0) && (servoState2 == SERVO_CLOSE_180)){
		
		return 3;
		
	}
}

/*----------INTERRUPTS----------------------------------------------------------*/