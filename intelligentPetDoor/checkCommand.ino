void checkCommand(){
	if (command.commandName == 0) now();
	else if (command.commandName == 1) program();
	else if (command.commandName == 2) where();
	
	//command done so clear it
	clearCommand();
}

void now(){
	Serial.print(F("You entered: /now "));
	if(command.movement == 0){
		Serial.println(F("free"));
		free();
	}
	//in (servo0 = open and servo1 = close)
	else if(command.movement == 1){
		Serial.println(F("in"));
		in();
	}
	//out (servo0 = close and servo1 = open)
	else if(command.movement == 2){
		Serial.println(F("out"));
		out();
	}
	//close (servo0 = close and servo 1 = close)
	else if(command.movement == 3){
		Serial.println(F("close"));
		close();
	}

}
	
void program(){

	unsigned char schedule1,schedule2,day,hour,minute,movement;

	if(command.modifierFlag == 2){
		Serial.println(F("You entered: /program show"));
		if(nschedule >0){
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
					Serial.print("   ");
					Serial.print(day);
					Serial.print("  ");
					Serial.print(hour);
					Serial.print(":");
					if(minute<10) Serial.print(F("0"));
					Serial.print(minute);
					Serial.print("   ");
					if(movement == 0){Serial.println("free");}
					else if(movement == 1){Serial.println("in");}
					else if(movement == 2){Serial.println("out");}
					else if(movement == 3){Serial.println("close");}
				}
			}
		}
		else{
			Serial.println(F("No scheduled commands."));
		}
	}
	else if(command.modifierFlag == 3){
		Serial.println(F("You entered: /program del all"));
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
			Serial.println(F("No scheduled commands."));
		}
	}
	else{
		signed char found = -1;
		char loopBreaker=0;
		for(int i=2;i<(MAX_SCHEDULE*2);i=i+2){
			schedule1 = EEPROM.read(i);
			if((schedule1&0x3F) != 0x3F){
				schedule2 = EEPROM.read(i+1);
				day = schedule2&0x07;
				hour = schedule2>>3;
				minute = schedule1&0x3F;
				movement = schedule1>>6;
				if((day == command.day) && (hour == command.hour) && (minute == command.minute)){
					found = i;
				}
				loopBreaker++;
				if(loopBreaker == nschedule) break;
			}
		}
		/*for(int i=0;i<MAX_SCHEDULE;i++){
			if((schedule[i].day == command.day) && (schedule[i].hour == command.hour) && (schedule[i].minute == command.minute) && (schedule[i].movement != -1)){
				found = i;
			}
		}*/

		if(command.modifierFlag == 0){
			Serial.print(F("You entered: /program "));
			Serial.print(command.day);
			Serial.print(F(" "));
			Serial.print(command.hour);
			Serial.print(F(":"));
			if(command.minute<10) Serial.print(F("0"));
			Serial.print(command.minute);
			if(command.movement == 0){Serial.println(F(" free"));}
			else if(command.movement == 1){Serial.println(F(" in"));}
			else if(command.movement == 2){Serial.println(F(" out"));}
			else if(command.movement == 3){Serial.println(F(" close"));}
			if(found == -1){
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
				nschedule++;
			}
			else{
				Serial.println("You already have a scheduled command at that time. Delete it before.");
			}
		}
		else if(command.modifierFlag == 1){
			Serial.print(F("You entered: /program del "));
			Serial.print(command.day);
			Serial.print(F(" "));
			Serial.print(command.hour);
			Serial.print(F(":"));
			Serial.println(command.minute);
			if(found != -1){
				EEPROM.write(found,0x3F);
				nschedule--;
			}
			else{
				Serial.println("Schedule to delete not found. Use \"/program show\" to see stored commands.");
			}
		}
	}

}

void where(){
		Serial.println(F("You entered: /where "));
		if(npetsIn == npets) Serial.println("All of your pets are inside.");
		else if (npetsIn == npets) Serial.println("All of your pets are outside.");
		else{
			Serial.print("There are ");
			Serial.print(npetsIn);
			Serial.print(" pet");
			if(npetsIn!=1) {Serial.print("s");}
			Serial.print(" inside and ");
			Serial.print(npetsOut);
			Serial.print(" pet");
			if(npetsOut!=1) {Serial.print("s");}
			Serial.println(" outside.");
		}
	
	}