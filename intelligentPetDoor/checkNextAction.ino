#define TIME_BETWEN_DE_AND_NA	( 2000 )

typedef struct nextAction {
	int npetsIn = -1;
	int npetsOut = -1;
	int movement = -1; // 0 = open, 1 = in, 2 = out, 3 = close
} nextAction;

/*extern unsigned long doorEventEnd;
extern nextAction action;
extern int npetsIn;
extern int npetsOut;*/

nextAction action;

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
