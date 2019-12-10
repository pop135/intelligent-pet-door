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

/* Time to consider a door event ended (Closing door time debounce) */
#define DOOR_EVENT_END_TIME_MS		( 2000 )

/* Opening time debounce. It is the minium time that the door has to be opened to 
 * consider that a pet passed through it. Times under that limit are considered 
 * not a trespassing pet. 
 */
#define DEBOUNCE_OPENING_TIME_MS	( 800 )

/*----------TYPEDEFS------------------------------------------------------------*/

/*----------VARIABLES-----------------------------------------------------------*/

/* Used to count time since last ISR to actual ISR */
unsigned long lastButtonISRTime;
unsigned long buttonISRTime;

/* Used to determine if a door event is still going on */
unsigned long doorEventStart;
unsigned long doorEventEnd;

/* Door event type */
int doorEventType;

/* To step over diferent stages of a door event */
char debounceFlag;

/* Counts the number of pets inside and outside. Initially all pets are inside */
int npetsIn;
int npetsOut;

/* Externally declared sensor ISR flags */
extern volatile char inOutFlag;
extern volatile char outInFlag;

/* Debug code */
#ifdef DEBUG
	int isrCount;
#endif

/*----------FUNCTIONS-----------------------------------------------------------*/

void initCheckButtons(){

	/* Sets door event start and end timers to initial value */
	doorEventStart=0;
	doorEventEnd=0;

	/* No door event going on */
	debounceFlag=0;

	/* All pets inside at the begining */
	npetsIn = npets;
	npetsOut = 0;
	
	/* Door event type init */
	doorEventType = 0;
	
	/* Debug code */
	#ifdef DEBUG
		isrCount=0;
	#endif
}

/* Checks if some sensor ISR has raised */
void checkButtons(){
	
	/* If in->out isr has raised do the job and reset the flag */
	if(inOutFlag){
		
		/* In a closing door debounce phase it's not necessary to call the door event function */
		if(debounceFlag != 2) doorEvent(1);
		
		/* Reset the flag */
		inOutFlag=0;
		
	}
	
	/* If out->in isr has raised do the job and reset the flag */
	if(outInFlag){
		
		/* In a closing door debounce phase it's not necessary to call the door event function */
		if(debounceFlag != 2) doorEvent(0);
		
		/* Reset the flag */
		outInFlag=0;
		
	}
	
	/* Ends a door event if suficient time with no inputs has passed. 
	 * It acts in two cases: If a pet is just scratching the door without crossing, when he/she finished
	 * the door event is reseted and when a pet has crossed it acts as the closing door debounce. 
	 */
	if((doorEventStart != 0) && ((millis()-doorEventStart) > DOOR_EVENT_END_TIME_MS)) {
		
		/* If suficient time has passed and door is closed end the door event. */
		if((digitalRead(BUTTON_PIN_0) == HIGH) && (digitalRead(BUTTON_PIN_1) == HIGH)){

			/* Reset debounce flag and door event star time */
			doorEventStart = 0;
			debounceFlag = 0;
			
			/* Set the door event end time */
			doorEventEnd = millis();
			
			tellUsers(doorEventType);
			doorEventType = 0;
			
			/* Debug code */
			#ifdef DEBUG
				Serial.println("DOOR EVENT ENDED");
				isrCount=0;
			#endif
		}
		
		/* If door is not closed means that a pet is half inside and half outside. Door event continues. */
		else{
			
			/* Set the new door event time */
			doorEventStart = millis();
			
		}
	}
}

/* Function to handle a door event */
void doorEvent(char direction){
	
	/* Sets the door event starting time */
	doorEventStart = millis();
	
	/* Reset the door event ending time */
	doorEventEnd = 0;
		
	/* Debug code */
	#ifdef DEBUG
		Serial.print(isrCount,DEC);
	#endif
	
	/* Starting phase of a door event. */
	if((debounceFlag == 0)){
		
		/* Debug code */
		#ifdef DEBUG
			Serial.print(": debounceFlag=");
			Serial.print(debounceFlag,DEC);
		#endif
		
		/* Sets the flag to advance trough the next door event phase if we recieve more sensors interrupts */
		debounceFlag = 1;
		
		/* Store the time of the first interrupt of the door event.*/
		lastButtonISRTime = millis();
		
		/* Debug code */
		#ifdef DEBUG
			Serial.print(" lastButtonISRTime=");
			Serial.print(lastButtonISRTime,DEC);
			Serial.println(" buttonISRTime= 0");
		#endif

	}
	
	/* Secont phase of a door event. In this phase we determine if the event is a pet actually crossing or a openning door debounce. */
	else if(debounceFlag == 1){ 
	
		/* Debug code */
		#ifdef DEBUG
			Serial.print(" debounceFlag=");
			Serial.print(debounceFlag,DEC);
		#endif
	
		/* Store the time of this interrupt */
		buttonISRTime = millis();
		
		/* Debug code */
		#ifdef DEBUG
			Serial.print(" lastButtonISRTime=");
			Serial.print(lastButtonISRTime,DEC);
			Serial.print(" buttonISRTime=");
			Serial.println(buttonISRTime,DEC);
		#endif
		
		/* If the time between the two last interrupts is greater than the limit. Consider it a pet crossing. */
		if((buttonISRTime - lastButtonISRTime) > DEBOUNCE_OPENING_TIME_MS){
			
			/* Stores the time of the pet crossing to allow next phase comparision */
			//lastButtonISRTime = buttonISRTime;//millis();
			
			/* Advance of door event phase */
			debounceFlag = 2;
			
			/* Call the functions to handle pet counters */
			if(direction) inOutDoorEvent();
			else outInDoorEvent();
			
		}
		
		/* If the time between two last interupts is less than the limit. Treat that interrupts as an opening door debounce. In that case we don't advance to the next phase. */
		else{
			Serial.println(F("To fast. Opening door debounce."));
			
			/* Update the last interrupt time to allow further comparision */
			lastButtonISRTime = buttonISRTime;
		}
	}	
	
	/* Debug code */
	#ifdef DEBUG
		isrCount++;
	#endif

}

/* Adjust the number of pets counters */
void inOutDoorEvent(){
	
	if(npetsIn > 0) {
		npetsOut++;
		npetsIn--;
		doorEventType = 1;
	
		/* Debug code */
		#ifdef DEBUG
			Serial.print(F("Button IN->OUT pressed. Pets IN: "));
			Serial.print(npetsIn);
			Serial.print(F(". Pets OUT: "));
			Serial.print(npetsOut);
			Serial.println(F("."));
		#endif
		
		
		
	}
	
	else{
		
		/* Debug code */
		#ifdef DEBUG
			Serial.println(F("All pets are outside. "));
		#endif
	}
	
}

/* Adjust the number of pets counters */
void outInDoorEvent(){
	
	if(npetsOut > 0) {
		npetsOut--;
		npetsIn++;
		doorEventType = 2;
		
		/* Debug code */
		#ifdef DEBUG
			Serial.print(F("Button OUT->IN pressed. Pets IN: "));
			Serial.print(npetsIn);
			Serial.print(F(". Pets OUT: "));
			Serial.print(npetsOut);
			Serial.println(F("."));
		#endif
		
	}
	
	else{
		
		/* Debug code */
		#ifdef DEBUG
			Serial.println(F("All pets are inside. "));
		#endif
	}
	
}

void tellUsers(int e){
	if(e==1){
		
		/* Debug code */
		#ifdef DEBUG
			Serial.println(F("Someone has gone outside."));
		#endif
		
		sendMessageToAllUsers("Someone has gone outside 😸");
	}
	else if(e == 2){
		
		/* Debug code */
		#ifdef DEBUG
			Serial.println(F("Someone has gone inside."));
		#endif
		
		sendMessageToAllUsers("Someone has returned home");// 😻");
	}
	
}

/*----------INTERRUPTS----------------------------------------------------------*/

