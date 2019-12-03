#define ONE_SECOND_MS	( 1000 )


//checkschedule variables to allow only one check per minute
int minuteBefore;
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;

void initCheckSchedule(){
	minuteBefore = (rtc.now()).minute()-1;
	startMillis=millis();
}

void checkSchedule(){
	
	unsigned char schedule1,schedule2;
	int dayOfWeek,hour,minute,scheduledDayOfWeek,scheduledHour,scheduledMinute,movement;
	int schedulesFound=0;

	currentMillis=millis();
	if((nschedule != 0) && (currentMillis - startMillis >= ONE_SECOND_MS)){
		
		startMillis = currentMillis;
		
		DateTime time= rtc.now();
		minute = time.minute();

		if(minute == minuteBefore) return;
		else minuteBefore = minute;
		

		dayOfWeek = time.dayOfTheWeek();
		if(dayOfWeek == 0) dayOfWeek = 7;
		hour = time.hour();

		
		
		Serial.print("Day: ");
		Serial.print(dayOfWeek);
		Serial.print(" Hour: ");
		Serial.print(hour);
		Serial.print(":");
		Serial.print(minute);
		
		for(int i=2;i<(MAX_SCHEDULE*2);i=i+2){
			schedule1 = EEPROM.read(i);
			if((schedule1&0x3F) != 0x3F){
				schedule2 = EEPROM.read(i+1);
				scheduledDayOfWeek = schedule2&0x07;
				scheduledHour = schedule2>>3;
				scheduledMinute = schedule1&0x3F;
				movement = schedule1>>6;
				
				if((dayOfWeek == scheduledDayOfWeek) && (hour == scheduledHour) && (minute == scheduledMinute)){
					
					if(movement == 0) {open();Serial.println(F(" -> Found a open scheduled command."));}
					else if(movement == 1) {in();Serial.println(F(" -> Found a in scheduled command."));}
					else if(movement == 2) {out();Serial.println(F(" -> Found a out scheduled command."));}
					else {close();Serial.println(F(" -> Found a close scheduled command."));}
					return;
				}
				else{
					schedulesFound++;
					if(nschedule == schedulesFound) { Serial.println(F(" -> No scheduled commands found."));return;}
				}
			}
		}
	}
}
