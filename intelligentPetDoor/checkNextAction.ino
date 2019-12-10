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

/*----------DEFINES--------------------------------------------------------------*/

#define TIME_BETWEN_DE_AND_NA	( 2000 )

/*----------TYPEDEFS------------------------------------------------------------*/

/* Data type to store a next action command */
typedef struct nextAction {
	
	/* 0 = open
	 * 1 = in
	 * 2 = out
	 * 3 = close 
	 */
	int movement;
	
	int npetsIn;
	int npetsOut;
	
} nextAction;

/*----------VARIABLES-----------------------------------------------------------*/

/* Stores the next action if any */
nextAction action;

/* Externally declared variables used on this file */
extern unsigned long doorEventEnd;
extern int npetsIn;
extern int npetsOut;

/*----------FUNCTIONS-----------------------------------------------------------*/

void initCheckNextAction(){
	clearNextAction();
}

/* Checks if there are next action issued */
void checkNextAction(){
	
	/* If next action issued  */
	if((action.movement != -1) && 
		
		/* Number of pets inside and outside is correct */
		(action.npetsIn == npetsIn) && (action.npetsOut == npetsOut) && 
		
		/* Not actually in a door event */
		(doorEventEnd != 0) && 
		
		/* Time between door event and next action  */
		((millis() - doorEventEnd) > TIME_BETWEN_DE_AND_NA)){
		
		#ifdef DEBUG
			Serial.print(F("CHECK NEXT ACTION doorEventEnd="));
			Serial.print(doorEventEnd,DEC);
		#endif
		

		if(action.movement == 0){
			#ifdef DEBUG
				Serial.print(F("Conditions met. Programmed action open done"));
			#endif
			
			open();
			sendMessageToAllUsers(F("/when Conditions met. Door opened!"));
		
		}
		//in (servo0 = open and servo1 = close)
		else if(action.movement == 1){
			#ifdef DEBUG
				Serial.print(F("Conditions met. Programmed action in done."));
			#endif
			
			in();
			sendMessageToAllUsers(F("/when Conditions met. Pets can just come in now!"));
		
		}
		//out (servo0 = close and servo1 = open)
		else if(action.movement == 2){
			#ifdef DEBUG
				Serial.print(F("Conditions met. Programmed action out done."));
			#endif
			
			out();
			sendMessageToAllUsers(F("/when Conditions met. Pets can just go out now!"));
		}
		//close (servo0 = close and servo 1 = close)
		else if(action.movement == 3){
			#ifdef DEBUG
				Serial.print(F("Conditions met. Programmed action close done."));
			#endif
			
			close();
			sendMessageToAllUsers(F("/when Conditions met. Door closed!"));
		}

		
		//command done so clear it
		clearNextAction();
	}
}

/* Resets next action variable to default value */
void clearNextAction(){
	action.movement = -1;
}

/*----------INTERRUPTS----------------------------------------------------------*/
