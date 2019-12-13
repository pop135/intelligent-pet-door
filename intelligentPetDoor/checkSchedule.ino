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

/* Stores the minute before actual to allow further comparision */ 
int minuteBefore;

/* To allow comparision between started time and current time */
unsigned long startMillis;
unsigned long currentMillis;

/* Number of scheduled stored commands */
int nschedule;

/*----------FUNCTIONS-----------------------------------------------------------*/

void initSchedule(){
	
	/* Get the minute before now */
	minuteBefore = (rtc.now()).minute()-1;
	
	/* Get the actual time */
	startMillis=millis();
	
	/* Starts from zero scheduled stored commands */
	nschedule=0;
	
	/* Read the EEPROM looking for previous scheduled stored commands */
	for(int i=SCHEDULE_POS;i<(MAX_SCHEDULE*SCHEDULE_SIZE);i=i+SCHEDULE_SIZE){
		if((readEEPROM(i)&0x3F) != 0x3F){
			nschedule++;
		}
	}
	
}

/* checks the scheduled commands every minute. If somthing was scheduled do it. */
void checkSchedule(){
	
	/* To store the EEPROM read values */
	unsigned char schedule1;
	unsigned char schedule2;
	
	/* To store the actual time */
	int dayOfWeek,hour,minute;
	
	/* Used to store the information of the scheduled command if any. */
	int scheduledDayOfWeek,scheduledHour,scheduledMinute,movement;
	
	/* Used to store the number of schedules found */
	int schedulesFound=0;

	/* Store actual time */
	currentMillis=millis();
	
	/* If we have some scheduled command and one second has passed */
	if((nschedule != 0) && (currentMillis - startMillis > ONE_SECOND_MS)){
		
		/* Update value of startmillis to allow further comparision */
		startMillis = currentMillis;
		
		/* Get the actual time */
		DateTime time= rtc.now();
		minute = time.minute();
		
		/* Compare minute before and actual minute to check if schedule was already checked that minute. */
		if(minute == minuteBefore) return;
		else minuteBefore = minute;
		
		/* If schedule not checked that minute get day of week and hour values */
		dayOfWeek = time.dayOfTheWeek();
		hour = time.hour();
		
		/* Trick to allow compatibility between user input and DS3231 output */
		if(dayOfWeek == 0) dayOfWeek = 7;

		/* Debug code */
		#ifdef DEBUG
			Serial.print(F("Day: "));
			Serial.print(dayOfWeek);
			Serial.print(F(" Hour: "));
			Serial.print(hour);
			Serial.print(F(":"));
			Serial.print(minute);
		#endif
		
		/* Reads all scheduled commands space of EEPROM */
		for(int i=SCHEDULE_POS;i<(MAX_SCHEDULE*SCHEDULE_SIZE);i=i+SCHEDULE_SIZE){
			
			/* If found a shceduled command */
			schedule1 = readEEPROM(i);
			if((schedule1&0x3F) != 0x3F){
				
				/* Extract all information from the scheduled command that has found */
				schedule2 = readEEPROM(i+1);
				scheduledDayOfWeek = schedule2&0x07;
				scheduledHour = schedule2>>3;
				scheduledMinute = schedule1&0x3F;
				movement = schedule1>>6;
				
				/* Compare time values between scheduled command and actual time */
				if((dayOfWeek == scheduledDayOfWeek) && (hour == scheduledHour) && (minute == scheduledMinute)){
					
					/* If that times are equal means that scheduled command has to be done. Check action and do it. */
					if(movement == 0) {
						
						/* Do the proper action */
						open();
						
						/* Debug code */
						#ifdef DEBUG
							Serial.println(F(" -> Found a open scheduled command."));
						#endif
						
					}
					else if(movement == 1) {
						
						/* Do the proper action */
						in();
						
						/* Debug code */
						#ifdef DEBUG
							Serial.println(F(" -> Found a in scheduled command."));
						#endif
						
					}
					else if(movement == 2) {
						
						/* Do the proper action */
						out();
						
						/* Debug code */
						#ifdef DEBUG
							Serial.println(F(" -> Found a out scheduled command."));
						#endif
						
					}
					else {
						
						/* Do the proper action */
						close();
						
						/* Debug code */
						#ifdef DEBUG
							Serial.println(F(" -> Found a close scheduled command."));
						#endif
						
					}
					
					/* Action done so return. (It's not allowed to store two actions in the same dow, hour & minute) */
					return;
					
				}
				
				/* Scheduled command found not corresponding to the actual dow, hour & minute */
				else{
					
					/* Increase counter of scheduled commands found */
					schedulesFound++;
					
					/* If schedules found are equal to total scheduled commands it's no need to keep reading EEPROM. */
					if(nschedule == schedulesFound) { 
						
						/* Debug code */
						#ifdef DEBUG
							Serial.println(F(" -> No scheduled commands found."));
						#endif
						
						/* No more schedules stored in EEPROM. */
						return;
					}
				}
			}
		}
	}
}

/*----------INTERRUPTS----------------------------------------------------------*/