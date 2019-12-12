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

/*----------VARIABLES-----------------------------------------------------------*/

/* Stores the next action if any */
nextAction action;

/* Externally declared variables used on this file */
extern unsigned long doorEventEnd;
extern int npetsIn;
extern int npetsOut;

/*----------FUNCTIONS-----------------------------------------------------------*/

/* Next action init */
void initCheckNextAction(){
	
	/* Initialize the variable */
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
		
		/* Time between door event and next action has passed */
		((millis() - doorEventEnd) > TIME_BETWEN_DE_AND_NA)){
		
		#ifdef DEBUG
			Serial.print(F("CHECK NEXT ACTION doorEventEnd="));
			Serial.print(doorEventEnd,DEC);
		#endif
		
		/* Open movement */
		if(action.movement == 0){
			
			/* Debug code */
			#ifdef DEBUG
				Serial.print(F("Conditions met. Programmed action open done"));
			#endif
			
			/* Actually do the action */
			open();
			
			/* Inform user */
			sendMessageToAllUsers(F("/when Conditions met. Door opened!"));
		
		}
		/* In movement */
		else if(action.movement == 1){
			
			/* Debug code */
			#ifdef DEBUG
				Serial.print(F("Conditions met. Programmed action in done."));
			#endif
			
			/* Actually do the action */
			in();
			
			/* Inform user */
			sendMessageToAllUsers(F("/when Conditions met. Pets can just come in now!"));
		
		}
		/* Out movement */
		else if(action.movement == 2){
			
			/* Debug code */
			#ifdef DEBUG
				Serial.print(F("Conditions met. Programmed action out done."));
			#endif
			
			/* Actually do the action */
			out();
			
			/* Inform user */
			sendMessageToAllUsers(F("/when Conditions met. Pets can just go out now!"));
		}
		/* Close movement */
		else if(action.movement == 3){
			
			/* Debug code */
			#ifdef DEBUG
				Serial.print(F("Conditions met. Programmed action close done."));
			#endif
			
			/* Actually do the action */
			close();
			
			/* Inform user */
			sendMessageToAllUsers(F("/when Conditions met. Door closed!"));
		}

		
		/* Next action already done, so clear the variable */
		clearNextAction();
	}
}

/* Sets next action variable to default value */
void clearNextAction(){
	action.movement = 0xFF;
}

/*----------INTERRUPTS----------------------------------------------------------*/
