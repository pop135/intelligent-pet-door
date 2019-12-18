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

/*----------VARIABLES------------------------------------------------------------*/

/* Variable to store the command it is filled by serialEvent interrupt */
commandIssued command;

/*----------EXTERN---------------------------------------------------------------*/

/* Extern variables used here */
extern SoftwareSerial ESP8266Serial;
extern simpleTBMessage msg;
extern uint8_t nschedule;
extern uint8_t npets;
extern uint8_t servoState0;
extern uint8_t servoState1;
extern uint8_t servoState2;

/*----------FUNCTIONS------------------------------------------------------------*/

/* Init function */
void initCheckCommand(){
	clearCommand();
}

/* Checks for a user issued command */
void checkCommand(){
	
	/* Depends on the command issued calls proper function */
	if (command.commandName == 0) now();
	else if (command.commandName == 1) program();
	else if (command.commandName == 2) where();
	else if (command.commandName == 3) status();
	
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
		
		/* Actually do the action */
		open();
		
		/* Inform the user */
		sendMessage(msg.id,F("Door opened! Purrfect!"));
		
	}
	
	/* In action */
	else if(command.movement == 1){
		
		/* Debug code */
		#ifdef DEBUG
			Serial.println(F("in"));
		#endif
		
		/* Actually do the action */
		in();
		
		/* Inform the user */
		sendMessage(msg.id,F("They can just go inside now!"));
		
	}
	
	/* Out action */
	else if(command.movement == 2){
		
		/* Debug code */
		#ifdef DEBUG
			Serial.println(F("out"));
		#endif
		
		/* Actually do the action */
		out();
		
		/* Inform the user */
		sendMessage(msg.id,F("They can just go outside now!"));
		
	}
	/* Close action */
	else if(command.movement == 3){
		
		/* Debug code */
		#ifdef DEBUG
			Serial.println(F("close"));
		#endif
		
		/* Actually do the action */
		close();
		
		/* Inform the user */
		sendMessage(msg.id,F("Door closed!"));
		
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
			Serial.print(F("You entered: /prog "));
			Serial.print(command.day);
			Serial.print(F(" "));
			if(command.hour<10) Serial.print(F("0"));
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
			for(int i=SCHEDULE_POS;i<MAX_SCHEDULE*SCHEDULE_SIZE;i=i+SCHEDULE_SIZE){
				schedule1 = readEEPROM(i);
				if((schedule1&0x3F)==0x3F){
					schedule1=command.movement<<6;
					writeEEPROM(i,(schedule1|command.minute));
					schedule2=command.hour<<3;
					writeEEPROM(i+1,(schedule2|command.day));
					break;
				}
			}
			
			/* Increment the number of scheduled commands */
			nschedule++;
			
			/* Inform the user */
			sendMessage(msg.id,F("Scheduled command saved!"));
		}
		else if(found == -2) {
			
			/* Debug code */
			#ifdef DEBUG
				Serial.println(F("Max schedules per day reached"));
			#endif
			
			/* Inform the user */
			sendMessage(msg.id,F("Max schedules per day reached"));
			
		}
		else{
			
			/* Debug code */
			#ifdef DEBUG
				Serial.println(F("Time slot full."));
			#endif
			
			/* Inform the user */
			sendMessage(msg.id,F("Time slot already full!"));
		}
		
	}
	
	/* To delete a scheduled command */
	else if(command.modifierFlag == 1){
		
		/* Debug code */
		#ifdef DEBUG
			Serial.print(F("You entered: /prog del "));
			Serial.print(command.day);
			Serial.print(F(" "));
			Serial.print(command.hour);
			Serial.print(F(":"));
			Serial.println(command.minute);
		#endif
		
		/* Search for the command */
		found = scheduledCommandSearch(command);
		
		/* If command found */
		if((found != -1)){
			
			/* Delete it */
			writeEEPROM(found,0x3F);
			
			/* Decrement the number of scheduled commands */
			nschedule--;
			
			/* Inform the user */
			sendMessage(msg.id,F("Scheduled command deleted!"));
		}
		else{
			
			/* Debug code */
			#ifdef DEBUG
				Serial.println(F("Schedule to delete not found. Use \"/program show\" to see stored commands."));
			#endif
			
			/* Inform the user */
			sendMessage(msg.id,F("Nothing saved in that time slot"));
		}
	}

	/* To show all scheduled commands */
	else if(command.modifierFlag == 2){
		
		/* Debug code */
		#ifdef DEBUG 
			Serial.println(F("You entered: /prog show"));
		#endif
		
		/* If we got something to show */
		if(nschedule >0){
				
			#ifdef DEBUG 
				Serial.print(F("Comands saved: "));
				Serial.println(nschedule);
			#endif
			
			/* List to store scheduled commands */
			struct EEPROMStoredCommand sortedScheduledCommands[MAX_SCHEDULE];
			int nReadCommands=0;
			
			/* Read EEPROM and store scheduled commands in a list */
			for(int i=SCHEDULE_POS;i<(MAX_SCHEDULE*SCHEDULE_SIZE);i=i+SCHEDULE_SIZE){
				schedule1 = readEEPROM(i);
				if((schedule1&0x3F) != 0x3F){
					schedule2 = readEEPROM(i+1);
					sortedScheduledCommands[nReadCommands].day = schedule2&0x07;
					sortedScheduledCommands[nReadCommands].hour = schedule2>>3;
					sortedScheduledCommands[nReadCommands].minute = schedule1&0x3F;
					sortedScheduledCommands[nReadCommands].movement = schedule1>>6;
					
					/* Debug code */
					#ifdef DEBUG
						Serial.print(F("# read commands:"));
						Serial.print(nReadCommands);
						Serial.print(F(" Day: "));
						Serial.print(sortedScheduledCommands[nReadCommands].day);
						Serial.print(F(" Hour: "));
						Serial.print(sortedScheduledCommands[nReadCommands].hour);
						Serial.print(F(" Minute: "));
						Serial.print(sortedScheduledCommands[nReadCommands].minute);
						Serial.print(F(" action: "));
						Serial.println(sortedScheduledCommands[nReadCommands].movement);
					#endif
					
					/* Increment readed commands */
					nReadCommands++;
					
					/* Nothing else to read */
					if(nschedule == nReadCommands) break;
					
				}
			}
			
			/* Sort the array first by day, second by hour and finally by minute */
			bool swapped; 
			EEPROMStoredCommand auxCommand;
			for (int i = 0; i < nReadCommands-1; i++){
				swapped = false; 
				for (int j = 0; j < nReadCommands-i-1; j++) {
					/* If day pos j greater than day pos j+1 swap */
					if (sortedScheduledCommands[j].day > sortedScheduledCommands[j+1].day) {
						swapCommands(&sortedScheduledCommands[j], &sortedScheduledCommands[j+1]); 
						swapped = true; 
					} 
					else if (sortedScheduledCommands[j].day == sortedScheduledCommands[j+1].day){
						/* In case of equal days check hour */
						if (sortedScheduledCommands[j].hour > sortedScheduledCommands[j+1].hour) {
							swapCommands(&sortedScheduledCommands[j], &sortedScheduledCommands[j+1]); 
							swapped = true; 
						} 
						else if (sortedScheduledCommands[j].hour == sortedScheduledCommands[j+1].hour) {
							/* In case of equal days and hours check minute */
							if (sortedScheduledCommands[j].minute > sortedScheduledCommands[j+1].minute) {
								swapCommands(&sortedScheduledCommands[j], &sortedScheduledCommands[j+1]); 
								swapped = true; 
									
							}
						}
					}
					
				} 

				/* If no elements were swaped break */ 
				if (swapped == false) break; 
				
			} 
			
			/* Debug code */
			#ifdef DEBUG 
				Serial.println(F("Stored commands:"));
				Serial.println(F(" Day   Hour   Command"));
				Serial.println(F("---------------------"));
				
				for(int i=0;i<nReadCommands;i++){
					Serial.print(F("   "));
					Serial.print(sortedScheduledCommands[i].day);
					Serial.print(F("  "));
					Serial.print(sortedScheduledCommands[i].hour);
					Serial.print(F(":"));
					if(sortedScheduledCommands[i].minute<10) Serial.print(F("0"));
					Serial.print(sortedScheduledCommands[i].minute);
					Serial.print(F("   "));
					if(sortedScheduledCommands[i].movement == 0){Serial.println(F("open"));}
					else if(sortedScheduledCommands[i].movement == 1){Serial.println(F("in"));}
					else if(sortedScheduledCommands[i].movement == 2){Serial.println(F("out"));}
					else if(sortedScheduledCommands[i].movement == 3){Serial.println(F("close"));}
					
				}
			#endif
			
			/* Once sorted send it to the user */
			String aux;
			uint8_t scheduledCommandsFound;
			
			for(int i=1;i<8;i++){
				scheduledCommandsFound = 0;
				/* First part of message (day) */
				if(i==1) aux = F("Monday\n");
				else if (i == 2) aux = F("Tuesday\n");
				else if (i == 3) aux = F("Wednesday\n");
				else if (i == 4) aux = F("Thursday\n");
				else if (i == 5) aux = F("Friday\n");
				else if (i == 6) aux = F("Saturday\n");
				else if (i == 7) aux = F("Sunday\n");
				
				/* Second part of message (scheduled commands) */
				for (int j = 0; j<nReadCommands;j++){
					if(sortedScheduledCommands[j].day == i){
						scheduledCommandsFound = 1;
						aux = aux + F(" - ");
						if(sortedScheduledCommands[j].hour<10) aux = aux + F("0");
						aux = aux + sortedScheduledCommands[j].hour + ":";
						if(sortedScheduledCommands[j].minute<10) aux = aux+ F("0");
						aux = aux + sortedScheduledCommands[j].minute;
						if(sortedScheduledCommands[j].movement == 0) aux = aux + F(" open\n");
						else if(sortedScheduledCommands[j].movement == 1) aux = aux + F(" in\n");
						else if(sortedScheduledCommands[j].movement == 2) aux = aux + F(" out\n");
						else if(sortedScheduledCommands[j].movement == 3) aux = aux + F(" close\n");
					}
				}
				
				/* Send one message per day */
				if(scheduledCommandsFound) {
					
					#ifdef DEBUG
						Serial.println(aux);
					#endif	
					
					/* Send message */
					sendMessage(msg.id,aux);
					delay(3000);
					
				}
			}	
			
		}
		else{
			
			/* Debug code */
			#ifdef DEBUG 
				Serial.println(F("No scheduled commands."));
			#endif
			
			/* Inform the user */
			sendMessage(msg.id,F("No commands scheduled atm"));
		}
	}
	
	/* To deal with delete all scheduled commands */
	else if(command.modifierFlag == 3){
		
		/* Debug code */
		#ifdef DEBUG 
			Serial.println(F("You entered: /prog del all"));
		#endif
		
		/* If something has to be deleted */
		if(nschedule > 0){
			for(int i=SCHEDULE_POS;i<(MAX_SCHEDULE*SCHEDULE_SIZE);i=i+SCHEDULE_SIZE){
				schedule1 = readEEPROM(i);
				if((schedule1&0x3F) != 0x3F){
					writeEEPROM(i,0x3F);
					nschedule--;
					if(nschedule==0) break;
				}
			}
			
			/* Inform the user */
			sendMessage(msg.id,F("All scheduled commands deleted!"));
		}
		else{
			/* Debug code */
			#ifdef DEBUG 
				Serial.println(F("No scheduled commands."));
			#endif
			
			/* Inform the user */
			 sendMessage(msg.id,F("No commands scheduled atm"));
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
			Serial.println(F("All of your pets are inside."));
		#endif
		
		/* Inform the user */
		sendMessage(msg.id,F("We are all inside home! 😻"));
		
	}
	else if (npetsOut == npets) {
		
		/* Debug code */
		#ifdef DEBUG 
			Serial.println(F("All of your pets are outside"));
		#endif
		
		/* Inform the user */
		sendMessage(msg.id,F("We are all outside! 😼"));
	}
	else{
					
		/* Debug code */
		#ifdef DEBUG 
			Serial.print(F("There are "));
			Serial.print(npetsIn);
			Serial.print(F(" pet"));
			if(npetsIn!=1) {Serial.print(F("s"));}
			Serial.print(F(" inside and "));
			Serial.print(npetsOut);
			Serial.print(F(" pet"));
			if(npetsOut!=1) {Serial.print(F("s"));}
			Serial.println(F(" outside."));
		#endif
		
		/* If to deal with is/are verb tenses */
		if (npets == 2){
			/* Inform the user */
			sendMessage(msg.id,(String)F("1 of us is at home and 1 is roaming outside"));
		}
		else if (npetsIn == 1){
			/* Inform the user */
			sendMessage(msg.id,(String)F("1 of us is at home and ") + npetsOut + F(" are roaming outside"));
		}
		else if (npetsOut == 1){
			/* Inform the user */
			sendMessage(msg.id,(String) npetsIn + F(" of us are at home and 1 is roaming outside"));
		}
		else{
			/* Inform the user */
			sendMessage(msg.id,(String)npetsIn + F(" of us are at home and ") + npetsOut + F(" are roaming outside"));
		}
		
		
	}

}

/* To deal with status command */
void status(){
	
	/* Debug code */
	#ifdef DEBUG 
		Serial.println(F("You entered: /status "));
	#endif
	
	/* Open position */
	if(doorStatus() == 0){
		
		/* Send message to the user */
		sendMessage(msg.id,F("Door is currently open!"));
		
		/* Debug code */
		#ifdef DEBUG 
			Serial.println(F("Door is currently open!"));
		#endif
		
	}
	
	/* In position */
	else if(doorStatus() == 1){
		
		/* Send message to the user */
		sendMessage(msg.id,F("Door is currently at IN position!"));
		
		/* Debug code */
		#ifdef DEBUG 
			Serial.println(F("Door is currently at IN position!"));
		#endif
		
	}
	
	/* Out position */
	else if(doorStatus() == 2){
		
		/* Send message to the user */
		sendMessage(msg.id,F("Door is currently at OUT position!"));
		
		/* Debug code */
		#ifdef DEBUG 
			Serial.println(F("Door is currently at OUT position!"));
		#endif
		
	}
	
	/* Close position */
	else if(doorStatus() == 3){
		
		/* Send message to the user */
		sendMessage(msg.id,F("Door is currently closed!"));
		
		/* Debug code */
		#ifdef DEBUG 
			Serial.println(F("Door is currently closed!"));
		#endif
		
	}
}

/* Search in the EEPROM for a scheduled command  */
int scheduledCommandSearch(commandIssued c){
	
	/* To store the EEPROM raw info readed */
	unsigned char schedule1, schedule2;
	
	/* To store the EEPROM readed info */
	int day, hour, minute;
	
	/* To count the total number of schedules found */
	char schedulesFound=0;
	
	char schedulesFoundSameDay = 0;
	
	/* Search in the EEPROM */
	for(int i=SCHEDULE_POS;i<(MAX_SCHEDULE*SCHEDULE_SIZE);i=i+SCHEDULE_SIZE){
		schedule1 = readEEPROM(i);
		if((schedule1&0x3F) != 0x3F){
			schedule2 = readEEPROM(i+1);
			day = schedule2&0x07;
			hour = schedule2>>3;
			minute = schedule1&0x3F;
			
			/* If scheduled command found return the EEPROM position where it is */
			if((day == c.day) && (hour == c.hour) && (minute == c.minute)){
				
				/* Return position of scheduled command */
				return i;
			}
			else{

				/* One more schedule found for that day */
				if(day == c.day) schedulesFoundSameDay++;
				
				/* If max schedules for day was reached return */
				if (schedulesFoundSameDay == MAX_SCHEDULE_A_DAY) return -2;
				
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

/* Implement swap function for bubble sort the list */
void swapCommands(EEPROMStoredCommand *commandA, EEPROMStoredCommand *commandB) { 
	EEPROMStoredCommand temp = *commandA; 
	*commandA = *commandB; 
	*commandB = temp; 
}

/*----------INTERRUPTS----------------------------------------------------------*/