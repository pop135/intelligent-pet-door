#define TIME_BETWEN_DE_AND_NA	( 2000 )

extern unsigned long doorEventEnd;

void checkNextAction(){
	
	if((action.movement != -1) && (action.npetsIn == npetsIn) && (action.npetsOut == npetsOut) && (doorEventEnd != 0) && ((millis() - doorEventEnd) > TIME_BETWEN_DE_AND_NA)){
		
		#ifdef DEBUG
			Serial.print("CHECK NEXT ACTION doorEventEnd=");
			Serial.print(doorEventEnd,DEC);
		#endif
		
		Serial.print("Conditions met. Programmed action ");
		if(action.movement == 0){
			Serial.print(F("open"));
			open();
		}
		//in (servo0 = open and servo1 = close)
		else if(action.movement == 1){
			Serial.print(F("in"));
			in();
		}
		//out (servo0 = close and servo1 = open)
		else if(action.movement == 2){
			Serial.print(F("out"));
			out();
		}
		//close (servo0 = close and servo 1 = close)
		else if(action.movement == 3){
			Serial.print(F("close"));
			close();
		}
		Serial.println(" done.");
		
		//command done so clear it
		clearNextAction();
	}
}
