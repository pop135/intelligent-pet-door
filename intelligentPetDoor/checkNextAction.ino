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

/*----------EXTERN--------------------------------------------------------------*/

extern unsigned long doorEventEnd;
extern uint8_t npetsIn;
extern uint8_t npetsOut;

/*----------FUNCTIONS-----------------------------------------------------------*/

/* Next action init */
void initCheckNextAction(){
	
	/* Initialize the variable */
	clearNextAction();
	
}

/* Checks if there are next action issued */
void checkNextAction(){
	
	/* If next action issued  */
	if((action.movement != 0xFF) && 
		
		/* Number of pets inside and outside is correct */
		(action.npetsIn == npetsIn) && (action.npetsOut == npetsOut) && 
		
		/* Not actually in a door event */
		(doorEventEnd != 0) && 
		
		/* Time between door event and next action has passed */
		((millis() - doorEventEnd) > TIME_BETWEN_DE_AND_NA)){
		
		#ifdef DEBUG
			Serial.print(F("CHECK NEXT ACTION doorEventEnd="));
			Serial.println(doorEventEnd,DEC);
		#endif
		
		/* Auxiliar variable */
		String aux;
		
		/* All pets inside */
		if(action.npetsIn == npets) aux = F("All your pets are inside now. ");
		
		/* All pets outside */
		else if (action.npetsOut == npets) aux = F("All your pets are outside now. ");
		
		/* Some pets inside and some outside */
		else{
			/* If to deal with is/are verb tenses */
			if (npets == 2) aux = F("1 of your pets is inside and 1 is outside. ");
			else if (action.npetsIn == 1) aux = (String) F("1 of your pets is inside and ") + action.npetsOut + F(" are outside. ");
			else if (action.npetsOut == 1) aux = (String) action.npetsIn + F(" of your pets are inside and 1 is outside. ");
			else aux = (String) action.npetsIn + F(" of your pets are inside and ") + action.npetsOut + F(" are outside. ");
		}
	
		/* Complete string with action and actually do the action*/
		if(action.movement == 0){
			
			/* Debug code */
			#ifdef DEBUG
				Serial.println(F("Conditions met. Programmed action open done"));
			#endif
			
			/* Actually do the action */
			open();
			
			/* complete string */
			aux = aux + F("Condifions met, door was opened!");
		
		}
		else if(action.movement == 1){
			
			/* Debug code */
			#ifdef DEBUG
				Serial.println(F("Conditions met. Programmed action in done."));
			#endif
			
			/* Actually do the action */
			in();
			
			/* complete string */
			aux = aux + F("Condifions met, door was set at in position!");
		
		}
		else if(action.movement == 2){
			
			/* Debug code */
			#ifdef DEBUG
				Serial.println(F("Conditions met. Programmed action out done."));
			#endif
			
			/* Actually do the action */
			out();
			
			/* complete string */
			aux = aux + F("Condifions met, door was set at out position!");
		}
		else if(action.movement == 3){
			
			/* Debug code */
			#ifdef DEBUG
				Serial.println(F("Conditions met. Programmed action close done."));
			#endif
			
			/* Actually do the action */
			close();
			
			/* complete string */
			aux = aux + F("Condifions met, door was closed!");
		}
		
		/* Send to all users */
		sendMessageToAllUsers(aux);
		
		/* Next action already done, so clear the variable */
		clearNextAction();
	}
}

/* Sets next action variable to default value */
void clearNextAction(){
	action.movement = 0xFF;
}

/*----------INTERRUPTS----------------------------------------------------------*/
