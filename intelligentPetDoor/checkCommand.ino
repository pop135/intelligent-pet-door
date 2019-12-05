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

/*----------TYPEDEFS------------------------------------------------------------*/

/* Data type to store a command. */
typedef struct commandIssued {
	
	/* 0 = /now
	 * 1 = /program
	 * 2 = /where
	 */
	int commandName;
	
	int day;
	int hour;
	int minute;
	
	/* 0 = open
	 * 1 = in
	 * 2 = out
	 * 3 = close 
	 */
	int movement; 
	
	/* 0 = schedule command
 	 * 1 = delete scheduled command
 	 * 2 = show scheduled commands
	 * 3 = delete all scheduled commands 
	 */
	int modifierFlag;
	
} commandIssued; 

/*----------PROTOTIPES----------------------------------------------------------*/

int scheduledCommandSearch(commandIssued c);

/*----------VARIABLES-----------------------------------------------------------*/

/* Variable to store the command it is filled by serialEvent interrupt */
commandIssued command;

/*----------FUNCTIONS-----------------------------------------------------------*/

void initCheckCommand(){
	clearCommand();
}

void checkCommand(){
	
	/* Depends on the command issued calls proper function */
	if (command.commandName == 0) now();
	else if (command.commandName == 1) program();
	else if (command.commandName == 2) where();
	
	/* Command done so reset the structure to allow further commands */
	clearCommand();
	
}

/* To deal with now command */
void now(){
	
	/* Debug code */
	#ifdef DEBUG
		Serial.print(F("You entered: /now "));
	#endif
	
	/* Open action */
	if(command.movement == 0){
		
		/* Debug code */
		#ifdef DEBUG
			Serial.println(F("open"));
		#endif
		
		open();
		
	}
	
	/* In action */
	else if(command.movement == 1){
		
		/* Debug code */
		#ifdef DEBUG
			Serial.println(F("in"));
		#endif
		
		in();
		
	}
	
	/* Out action */
	else if(command.movement == 2){
		
		/* Debug code */
		#ifdef DEBUG
			Serial.println(F("out"));
		#endif

		out();
		
	}
	/* Close action */
	else if(command.movement == 3){
		
		/* Debug code */
		#ifdef DEBUG
			Serial.println(F("close"));
		#endif
		
		close();
		
	}
}
	
/* To deal with program command */
void program(){

	/* To store the EEPROM raw info readed */
	unsigned char schedule1,schedule2;
	
	/* To store the EEPROM stored info */
	int day,hour,minute,movement;

	/* To store the result of a scheduled command search */
	int found;

	/* To schedule a command */
	if(command.modifierFlag == 0){
		
		/* Debug code */
		#ifdef DEBUG
			Serial.print(F("You entered: /program "));
			Serial.print(command.day);
			Serial.print(F(" "));
			Serial.print(command.hour);
			Serial.print(F(":"));
			if(command.minute<10) Serial.print(F("0"));
			Serial.print(command.minute);
			if(command.movement == 0){Serial.println(F(" open"));}
			else if(command.movement == 1){Serial.println(F(" in"));}
			else if(command.movement == 2){Serial.println(F(" out"));}
			else if(command.movement == 3){Serial.println(F(" close"));}
		#endif
		
		/* Search for the command */
		found = scheduledCommandSearch(command);
		
		/* If not found it can be scheduled */
		if(found == -1){
			
			/* Search for an empty EEPROM space and store the command there */
			for(int i=2;i<MAX_SCHEDULE*2;i=i+2){
				schedule1 = EEPROM.read(i);
				if((schedule1&0x3F)==0x3F){
					schedule1=command.movement<<6;
					EEPROM.write(i,(schedule1|command.minute));
					schedule2=command.hour<<3;
					EEPROM.write(i+1,(schedule2|command.day));
					break;
				}
			}
			
			/* Increment the number of scheduled commands */
			nschedule++;
		}
		
		else{
			
			/* Debug code */
			#ifdef DEBUG
				Serial.println(F("You already have a scheduled command at that time. Delete it before."));
			#endif
		}
		
	}
	
	/* To delete a scheduled command */
	else if(command.modifierFlag == 1){
		
		/* Debug code */
		#ifdef DEBUG
			Serial.print(F("You entered: /program del "));
			Serial.print(command.day);
			Serial.print(F(" "));
			Serial.print(command.hour);
			Serial.print(F(":"));
			Serial.println(command.minute);
		#endif
		
		/* Search for the command */
		found = scheduledCommandSearch(command);
		
		/* If command found */
		if(found != -1){
			
			/* Delete it */
			EEPROM.write(found,0x3F);
			
			/* Decrement the number of scheduled commands */
			nschedule--;
		}
		else{
			
			/* Debug code */
			#ifdef DEBUG
				Serial.println(F("Schedule to delete not found. Use \"/program show\" to see stored commands."));
			#endif
		}
	}

	/* To show all scheduled commands */
	else if(command.modifierFlag == 2){
		
		/* Debug code */
		#ifdef DEBUG 
			Serial.println(F("You entered: /program show"));
		#endif
		
		/* If we got something to show */
		if(nschedule >0){
			
			/* Debug code */
			#ifdef DEBUG 
				Serial.println(F("Stored commands:"));
				Serial.println(F(" Day   Hour   Command"));
				Serial.println(F("---------------------"));
				
				for(int i=2;i<(MAX_SCHEDULE*2);i=i+2){
					schedule1 = EEPROM.read(i);
					if((schedule1&0x3F) != 0x3F){
						schedule2 = EEPROM.read(i+1);
						day = schedule2&0x07;
						hour = schedule2>>3;
						minute = schedule1&0x3F;
						movement = schedule1>>6;
						Serial.print(F("   "));
						Serial.print(day);
						Serial.print(F("  "));
						Serial.print(hour);
						Serial.print(F(":"));
						if(minute<10) Serial.print(F("0"));
						Serial.print(minute);
						Serial.print(F("   "));
						if(movement == 0){Serial.println(F("open"));}
						else if(movement == 1){Serial.println(F("in"));}
						else if(movement == 2){Serial.println(F("out"));}
						else if(movement == 3){Serial.println(F("close"));}
					}
				}
			#endif
		}
		else{
			
			/* Debug code */
			#ifdef DEBUG 
				Serial.println(F("No scheduled commands."));
			#endif
		}
	}
	
	/* To deal with deleta all scheduled commands */
	else if(command.modifierFlag == 3){
		
		/* Debug code */
		#ifdef DEBUG 
			Serial.println(F("You entered: /program del all"));
		#endif
		
		/* If something has to be deleted */
		if(nschedule > 0){
			for(int i=2;i<(MAX_SCHEDULE*2);i=i+2){
				schedule1 = EEPROM.read(i);
				if((schedule1&0x3F) != 0x3F){
					EEPROM.write(i,0x3F);
					nschedule--;
					if(nschedule==0) break;
				}
			}
		}
		else{
			/* Debug code */
			#ifdef DEBUG 
				Serial.println(F("No scheduled commands."));
			#endif
		}
	}
}

/* To deal with where command */
void where(){
	
	/* Debug code */
	#ifdef DEBUG 
		Serial.println(F("You entered: /where "));
	#endif
	
	if(npetsIn == npets){ 
	
		/* Debug code */
		#ifdef DEBUG 
			Serial.println("All of your pets are inside.");
		#endif
		
	}
	else if (npetsOut == npets) {
		
		/* Debug code */
		#ifdef DEBUG 
			Serial.println("All of your pets are outside.");
		#endif
		
	}
	else{
		
		/* Debug code */
		#ifdef DEBUG 
			Serial.print("There are ");
			Serial.print(npetsIn);
			Serial.print(" pet");
			if(npetsIn!=1) {Serial.print("s");}
			Serial.print(" inside and ");
			Serial.print(npetsOut);
			Serial.print(" pet");
			if(npetsOut!=1) {Serial.print("s");}
			Serial.println(" outside.");
		#endif
		
	}

}

/* Search in the EEPROM for a scheduled command  */
int scheduledCommandSearch(commandIssued c){
	
	/* To store the EEPROM raw info readed */
	unsigned char schedule1, schedule2;
	
	/* To store the EEPROM readed info */
	int day, hour, minute;
	
	/* To count the number of schedules found */
	char schedulesFound=0;
	
	/* Search in the EEPROM */
	for(int i=2;i<(MAX_SCHEDULE*2);i=i+2){
		schedule1 = EEPROM.read(i);
		if((schedule1&0x3F) != 0x3F){
			schedule2 = EEPROM.read(i+1);
			day = schedule2&0x07;
			hour = schedule2>>3;
			minute = schedule1&0x3F;
			
			/* If scheduled command found return the EEPROM position where it is */
			if((day == c.day) && (hour == c.hour) && (minute == c.minute)){
				return i;
			}
			else{
				/* One schedule found and it's not the one we are looking for. Increment the counter */
				schedulesFound++;
				
				/* If there are no more schedules stored that can be found return */
				if(schedulesFound == nschedule) return -1;
			}
		}
	}
	
	/* Just to be sure that something is returned */
	return -1;
}
	
/* Resets command issued variable to default value */
void clearCommand(){
	command.commandName = -1;
}

/*----------INTERRUPTS----------------------------------------------------------*/