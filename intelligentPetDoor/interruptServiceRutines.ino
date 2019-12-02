#define DEBUG 1

#define RESET_DEBOUNCE_FLAG_TIME_MS	( 5000 )
#define DEBOUNCE_CLOSING_TIME_MS	( 3000 )
#define DEBOUNCE_OPENING_TIME_MS	( 3000 )

volatile unsigned long lastButtonISRTime=0;
volatile unsigned long buttonISRTime=0;
volatile char debounceFlag=0;

volatile char inOutFlag=0;
volatile char outInFlag=0;

unsigned long doorEventStart=0;

#ifdef DEBUG
	volatile int count=0;
#endif



void checkButtonsISR(){
	
	if(inOutFlag){
		buttonInOut();
		inOutFlag=0;
	}
	if(outInFlag){
		buttonOutIn();
		outInFlag=0;
	}
	
	if((doorEventStart != 0) && (millis()-doorEventStart) > RESET_DEBOUNCE_FLAG_TIME_MS){
		
		attachInterrupt(digitalPinToInterrupt(buttonPin0), buttonInOutISR, FALLING);
		attachInterrupt(digitalPinToInterrupt(buttonPin1), buttonOutInISR, RISING);
		doorEventStart = 0;
		debounceFlag = 0;
		#ifdef DEBUG
			Serial.println("DOOR EVENT ENDED");
		#endif

	}
	
}



//in->out button
void buttonInOut(){
	
	if(doorEventStart == 0){
		doorEventStart = millis();
		#ifdef DEBUG
			Serial.println("DOOR EVENT STARTED");
		#endif
		
	}
	
	#ifdef DEBUG
		Serial.print(count,DEC);
		Serial.print(":IN->OUT");
	#endif
	
	if((debounceFlag == 0)){// || ((debounceFlag == 2) && ((buttonISRTime - lastButtonISRTime) > DEBOUNCE_CLOSING_TIME_MS))){
		
		#ifdef DEBUG
			Serial.print(" debounceFlag=");
			Serial.print(debounceFlag,DEC);
		#endif
		
		debounceFlag = 1;
		lastButtonISRTime = millis();
		buttonISRTime =0;
		
		#ifdef DEBUG
			Serial.print(" lastButtonISRTime=");
			Serial.print(lastButtonISRTime,DEC);
			Serial.print(" buttonISRTime=");
			Serial.println(buttonISRTime,DEC);
		#endif

	}
	else if(debounceFlag == 1){ 
	

	
	
		#ifdef DEBUG
			Serial.print(" debounceFlag=");
			Serial.print(debounceFlag,DEC);
		#endif
	
		buttonISRTime = millis();
		
		#ifdef DEBUG
			Serial.print(" lastButtonISRTime=");
			Serial.print(lastButtonISRTime,DEC);
			Serial.print(" buttonISRTime=");
			Serial.println(buttonISRTime,DEC);
		#endif
		
		if((buttonISRTime - lastButtonISRTime) > DEBOUNCE_OPENING_TIME_MS){
			lastButtonISRTime =millis();
			debounceFlag = 2;
				if(npetsIn > 0) {
				npetsIn--;
				npetsOut++;
				
				//buttonState0 = 1;
			
				Serial.print(F("Button IN->OUT pressed. Pets IN: "));
				Serial.print(npetsIn);
				Serial.print(F(". Pets OUT: "));
				Serial.print(npetsOut);
				Serial.println(F("."));
			}
			else{
				Serial.println(F("All pets are outside. "));
			}
		}
		else{
			Serial.println(F("To fast. Opening door debounce."));
			lastButtonISRTime = buttonISRTime;
		}
		

	}	
	else if(debounceFlag == 2){
		
		#ifdef DEBUG
			Serial.print(" debounceFlag=");
			Serial.print(debounceFlag,DEC);
		#endif
		
		buttonISRTime = millis();
		
		#ifdef DEBUG
			Serial.print(" lastButtonISRTime=");
			Serial.print(lastButtonISRTime,DEC);
			Serial.print(" buttonISRTime=");
			Serial.println(buttonISRTime,DEC);
		#endif
		
		if((buttonISRTime - lastButtonISRTime) > DEBOUNCE_CLOSING_TIME_MS){
			debounceFlag = 0;
		}
		else{
			Serial.println(F("To fast. Closing door debounce."));
			lastButtonISRTime = buttonISRTime;
		}
	}
	
	#ifdef DEBUG
		count++;
	#endif

}

//out->in button
void buttonOutIn(){
	
		if(doorEventStart == 0){
		doorEventStart = millis();
		#ifdef DEBUG
			Serial.println("DOOR EVENT STARTED");
		#endif
	}
	
	#ifdef DEBUG
		Serial.print(count,DEC);
		Serial.print(":OUT->IN");
	#endif
	
	if((debounceFlag == 0)){// || ((debounceFlag == 2) && ((buttonISRTime - lastButtonISRTime) > DEBOUNCE_CLOSING_TIME_MS))){
		
		#ifdef DEBUG
			Serial.print(" debounceFlag=");
			Serial.print(debounceFlag,DEC);
		#endif
		
		
		debounceFlag=1;
		lastButtonISRTime =millis();
		buttonISRTime =0;
		
		
		#ifdef DEBUG
			Serial.print(" lastButtonISRTime=");
			Serial.print(lastButtonISRTime,DEC);
			Serial.print(" buttonISRTime=");
			Serial.println(buttonISRTime,DEC);
		#endif
		

	}
	else if(debounceFlag == 1){ 
	

	
		#ifdef DEBUG
			Serial.print(" debounceFlag=");
			Serial.print(debounceFlag,DEC);
		#endif
	
	
		buttonISRTime = millis();
		
		#ifdef DEBUG
			Serial.print(" lastButtonISRTime=");
			Serial.print(lastButtonISRTime,DEC);
			Serial.print(" buttonISRTime=");
			Serial.println(buttonISRTime,DEC);
		#endif
		
		if((buttonISRTime - lastButtonISRTime) > DEBOUNCE_OPENING_TIME_MS){
			lastButtonISRTime =millis();
			debounceFlag = 2;
				if(npetsOut > 0) {
				npetsOut--;
				npetsIn++;
				
				//buttonState1 = 1;
			
				Serial.print(F("Button OUT->IN pressed. Pets IN: "));
				Serial.print(npetsIn);
				Serial.print(F(". Pets OUT: "));
				Serial.print(npetsOut);
				Serial.println(F("."));
			}
			else{
				Serial.println(F("All pets are inside. "));
			}
		}
		else{
			Serial.println(F("To fast. Opening door debounce."));
			lastButtonISRTime = buttonISRTime;
		}
		
	}	
	else if(debounceFlag == 2){
		
		#ifdef DEBUG
			Serial.print(" debounceFlag=");
			Serial.print(debounceFlag,DEC);
		#endif
		
		
		buttonISRTime = millis();
		
		#ifdef DEBUG
			Serial.print(" lastButtonISRTime=");
			Serial.print(lastButtonISRTime,DEC);
			Serial.print(" buttonISRTime=");
			Serial.println(buttonISRTime,DEC);
		#endif
		
		if((buttonISRTime - lastButtonISRTime) > DEBOUNCE_CLOSING_TIME_MS){
			debounceFlag = 0;
		}
		else{
			
			Serial.println(F("To fast. Closing door debounce."));
			lastButtonISRTime = buttonISRTime;
		}
	}
	

	#ifdef DEBUG
		count++;
	#endif

}

void buttonInOutISR(){
	detachInterrupt(digitalPinToInterrupt(buttonPin1));
	inOutFlag = 1;
}

void buttonOutInISR(){
	detachInterrupt(digitalPinToInterrupt(buttonPin0));
	outInFlag=1;
}



