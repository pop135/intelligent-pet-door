/*int const space = 32;
int const carriageReturn = 10;
int const commandNow = 0;
int const commandProgram = 1;
int const movementFree = 0;
int const movementIn = 1;
int const movementOut = 2;
int const movementClose = 3;*/

int const MAX_WORDS = 6;



void serialEvent(){

	//maintain until processcommands not implemented
	//command.commandName = -1;
	//command.hour = -1;
	//command.minute = -1;
	//command.movement = -1;
	//command.modifierFlag = -1;
	
	//maintain until process when not implemented
	//action.movement = -1;
	//action.npetsIn = -1;
	//action.npetsOut = -1;
  

	int endFirstWord = 0;
	int nwords=0;
	String word;
	String words[MAX_WORDS+1]; 
	String input = Serial.readString(); 
	input.trim();
	input.toLowerCase();
	
	for(int i=0;i<MAX_WORDS+1;i++){
		words[i]="";
	}

	while(endFirstWord != -1 && nwords < MAX_WORDS+1){
		
		//getting the word
		endFirstWord = input.indexOf(' ');
		if(endFirstWord != -1){
			word = input.substring(0,endFirstWord);
		}
		else{
			word = input;
		}
		input.remove(0,endFirstWord);
		input.trim();
		words[nwords] = word;
		nwords++;
		
		/*Serial.print("Word: ");
		Serial.println(word);
		Serial.print("Input remaining: ");
		Serial.println(input);
		Serial.print("nwords: ");
		Serial.println(nwords);
		Serial.print("endfirstword: ");
		Serial.println(endFirstWord);
		Serial.println("");
		for(int i=0;i<MAX_WORDS+1;i++){
			Serial.print("words[");
			Serial.print(i);
			Serial.print("] ->");
			Serial.print(words[i]);
			Serial.println("<-");
		}
		Serial.println("");*/
		
	}
	
	//in case that the string is longer than maxium words command
	serialFlush();
	
	//actually understanding the word
	if(words[0].equals("/now")){
		command.commandName = 0;
		if(words[1].equals("free")){command.movement = 0;}
		else if (words[1].equals("in")){command.movement = 1;}
		else if (words[1].equals("out")){command.movement = 2;}
		else if (words[1].equals("close")){command.movement = 3;}
		else{
			Serial.println(F("Incorrect action. /usage to help.")); 
			clearCommand();
			return;
		}
		if(words[2].equals("")){return;}
		else{
			Serial.println(F("Something weird after command. /usage to help."));  
			clearCommand();
			return;
		}
	}
	else if (words[0].equals("/program")){
		command.commandName = 1;
		int modifierFlag = 0;
		
		if(words[1].equals("show")){
			command.modifierFlag = 2;
			if(words[2].equals("")){return;}
			else{
				Serial.println("Something weird after command. /usage to help.");  
				clearCommand();
				return;
			}
		}
		
		if (words[1].equals("del")){
			command.modifierFlag = 1;
			modifierFlag = 1;
		}
		
		if (checkDateIntegrity(words[1+modifierFlag]) == 0){
			if(!modifierFlag){
				command.modifierFlag = 0;
				if(words[2+modifierFlag].equals("free")){command.movement = 0;}
				else if (words[2+modifierFlag].equals("in")){command.movement = 1;}
				else if (words[2+modifierFlag].equals("out")){command.movement = 2;}
				else if (words[2+modifierFlag].equals("close")){command.movement = 3;}
				else{
					Serial.println(F("Incorrect action. /usage to help.")); 
					clearCommand();
					return;
				}
			}
			if(words[3].equals("")){return;}
			else{
				Serial.println(F("Something weird after command. /usage to help."));  
				clearCommand();
				return;
			}
			

		}
		else{
			Serial.println(F("Correct format for hour is HH:MM. /usage to help.")); 
			clearCommand();
			return;
		}
	}
	else if(words[0].equals("/when")){
		int actionIssued = action.movement;
		int movement;
		if(words[1].equals("allin")){
			if(words[2].equals("free")){movement = 0;}
			else if (words[2].equals("in")){movement = 1;}
			else if (words[2].equals("out")){movement = 2;}
			else if (words[2].equals("close")){movement = 3;}
			else{
				Serial.println(F("Incorrect action. /usage to help.")); 
				return;
			}
			if(words[3].equals("")){
				if(actionIssued == -1){
					Serial.print(F("You entered: /when allin "));
					if(movement == 0){Serial.println("free");}
					else if(movement == 1){Serial.println("in");}
					else if(movement == 2){Serial.println("out");}
					else if(movement == 3){Serial.println("close");}
					action.movement = movement;
					action.npetsIn = npets;
					action.npetsOut = 0;
				}
				else{
					Serial.println(F("There is a /when next action issued. Use /when show to show it or /when del to delete it. /usage to help."));  
					return;
				}
			}
			else{
				Serial.println(F("Something weird after command. /usage to help."));  
				return;
			}
		}
		else if(words[1].equals("allout")){
			if(words[2].equals("free")){movement = 0;}
			else if (words[2].equals("in")){movement = 1;}
			else if (words[2].equals("out")){movement = 2;}
			else if (words[2].equals("close")){movement = 3;}
			else{
				Serial.println(F("Incorrect action. /usage to help.")); 
				return;
			}
			if(words[3].equals("")){
				if(actionIssued == -1){
					Serial.print(F("You entered: /when allout "));
					if(movement == 0){Serial.println("free");}
					else if(movement == 1){Serial.println("in");}
					else if(movement == 2){Serial.println("out");}
					else if(movement == 3){Serial.println("close");}
					action.movement = movement;
					action.npetsIn = 0;
					action.npetsOut = npets;
				}
				else{
					Serial.println(F("There is a \"/when\" next action issued. Use \"/when show\" to show it or \"/when del\" to delete it. /usage to help."));  
					return;
				}
			}
			else{
				Serial.println(F("Something weird after command. /usage to help."));  
				return;
			}
		}
		else if(stringToInt(words[1]) != -1){
			if(words[2].equals("in")){
				if(stringToInt(words[3]) != -1){
					if(words[4].equals("out")){
						if(words[5].equals("free")){movement = 0;}
						else if (words[5].equals("in")){movement = 1;}
						else if (words[5].equals("out")){movement = 2;}
						else if (words[5].equals("close")){movement = 3;}
						else{
							Serial.println(F("Incorrect action. /usage to help.")); 
							return;
						}
						if(words[6].equals("")){
							if((words[1].toInt() + words[3].toInt()) == npets){
								if(actionIssued == -1){
									Serial.print(F("You entered: /when "));
									Serial.print(words[1].toInt());
									Serial.print(F(" in "));
									Serial.print(words[3].toInt());
									if(movement == 0){Serial.println(F(" out free"));}
									else if(movement == 1){Serial.println(F(" out in"));}
									else if(movement == 2){Serial.println(F(" out out"));}
									else if(movement == 3){Serial.println(F(" out close"));}
									action.movement = movement;
									action.npetsIn = words[1].toInt();
									action.npetsOut = words[3].toInt();
								}
								else{
									Serial.println(F("There is a \"/when\" next action issued. Use \"/when show\" to show it or \"/when del\" to delete it. /usage to help."));  
									return;
								}
							}
							else{
								Serial.print(F("Bad number of pets. The quantity of in/out pets should be "));
								Serial.print(npets);
								Serial.println(".");
								return;
							}
						}
						else{
							Serial.println(F("Something weird after command. /usage to help."));  
							return;
						}
					}
					else{
						Serial.println(F("Incorrect command. /usage to help.")); 
						return;
					}
					
				}
				else{
					Serial.println(F("Incorrect command. /usage to help.")); 
					return;
				}
			}
			else if (words[2].equals("out")){
				if(stringToInt(words[3]) != -1){
					if(words[4].equals("in")){
						if(words[5].equals("free")){movement = 0;}
						else if (words[5].equals("in")){movement = 1;}
						else if (words[5].equals("out")){movement = 2;}
						else if (words[5].equals("close")){movement = 3;}
						else{
							Serial.println(F("Incorrect action. /usage to help.")); 
							return;
						}
						if(words[6].equals("")){
							if((words[1].toInt() + words[3].toInt()) == npets){
								if(actionIssued == -1){
									Serial.print(F("You entered: /when "));
									Serial.print(words[1].toInt());
									Serial.print(F(" out "));
									Serial.print(words[3].toInt());
									if(movement == 0){Serial.println(F(" in free"));}
									else if(movement == 1){Serial.println(F(" in in"));}
									else if(movement == 2){Serial.println(F(" in out"));}
									else if(movement == 3){Serial.println(F(" in close"));}
									action.movement = movement;
									action.npetsIn = words[3].toInt();
									action.npetsOut = words[1].toInt();
								}
								else{
									Serial.println(F("There is a \"/when\" next action issued. Use \"/when show\" to show it or \"/when del\" to delete it. /usage to help."));  
									return;
								}
							}
							else{
								Serial.print(F("Bad number of pets. The quantity of in/out pets should be "));
								Serial.print(npets);
								Serial.println(".");
								return;
							}
						}
						else{
							Serial.println(F("Something weird after command. /usage to help."));  
							return;
						}
					}
					else{
						Serial.println(F("Incorrect command. /usage to help.")); 
						return;
					}
					
				}
				else{
					Serial.println(F("Incorrect command. /usage to help.")); 
					return;
				}
			}
			else{
				Serial.println(F("Incorrect command. /usage to help.")); 
				return;
			}
		}
		else if(words[1].equals("show")){
			if(words[2].equals("")){
				if(actionIssued != -1){
					Serial.println(F("You entered: /when show"));
					Serial.print(F("Next action: "));
					if(action.movement == 0){Serial.print(F("free"));}
					else if(action.movement == 1){Serial.print(F("in"));}
					else if(action.movement == 2){Serial.print(F("out"));}
					else if(action.movement == 3){Serial.print(F("close"));}
					Serial.print(F(" when "));
					Serial.print(action.npetsIn);
					Serial.print(F(" IN and "));
					Serial.print(action.npetsOut);
					Serial.println(F(" OUT."));
				}
				else{
					Serial.println(F("There is not a \"/when\" next action issued. /usage to help."));  
					return;
				}
			}
			else{
				Serial.println(F("Something weird after command. /usage to help."));  
				return;
			}
		}
		else if(words[1].equals("del")){
			if(words[2].equals("")){
				if(actionIssued != -1){
					Serial.println(F("You entered: /when del"));
					clearNextAction();
				}
				else{
					Serial.println(F("There is not a \"/when\" next action issued. /usage to help."));  
					return;
				}
			}
			else{
				Serial.println(F("Something weird after command. /usage to help."));  
				return;
			}
		}
		else{
			Serial.println(F("Incorrect command. /usage to help.")); 
			return;
		}
	
	}
	else if(words[0].equals("/where")){
		if(words[1].equals("")){
			command.commandName = 2;
		}
		else{
			Serial.println(F("Something weird after command. /usage to help."));  
			return;
		}
	}
	else if (words[0].equals("/usage")){
		if(words[1].equals("")){showUsage();}
		else{
			Serial.println(F("Something weird after command. /usage to help."));  
			return;
		}
		
		
	}
	else{
		Serial.println(F("Incorrect command. /usage to help."));
		return;
	}
  
}

void showUsage(){
	Serial.println(F("USAGE:"));
	Serial.println(F("Available commands are : /now /program /when /usage"));
	Serial.println(F(" - /now {action}: When no pet is crossing the door performs the desired action."));
	Serial.print(F(" - /program HH:MM {action}: Schedule the desired action in the desired time of the day (max scheduled actions = "));
	Serial.print(MAX_SCHEDULE);
	Serial.println(F(")."));
	Serial.println(F(" - /program del HH:MM: Delete the programed action in the given time."));
	Serial.println(F(" - /program show: Show scheduled commands."));
	Serial.println(F(" - /when {allin|allout} {action}: When all pets are in or out perform the desired action."));
	Serial.println(F(" - /when X in Y out {action}: When X pets are in and Y pets are out perform the desired action."));
	Serial.println(F(" - /usage: Shows this help."));
	Serial.println(F("Available actions are:"));
	Serial.println(F(" - free: pets have free movment."));
	Serial.println(F(" - in: pets can enter but not exit."));
	Serial.println(F(" - out: pets can exit but not enter."));
	Serial.println(F(" - close: the door is closed."));
}

void clearCommand(){
	command.commandName = -1;
	command.hour = -1;
	command.minute = -1;
	command.movement = -1;
	command.modifierFlag = -1;
}

void clearNextAction(){
	action.npetsIn = -1;
	action.npetsOut = -1;
	action.movement = -1;
}
void serialFlush(){
    while(Serial.available() > 0){
      int incomingByte = Serial.read();
    }
}

int checkDateIntegrity(String st){
	
	int h=-1;
	int m=-1;
	String aux = "0";
	
	//not correct length
	if((st.length() != 4) && (st.length() != 5)) {return -1;}
	
	//if no zero before we should add it
	if(st.length() == 4) {aux.concat(st);}
	else {
		aux = st;
		if(!isDigit(aux.charAt(0))) {return -1;}
	}
	if(!isDigit(aux.charAt(1))) {return -1;}
	if(!(aux.charAt(2) == ':') || (aux.charAt(2) == '.')) {return -1;}
	if(!isDigit(aux.charAt(3))) {return -1;}
	if(!isDigit(aux.charAt(4))) {return -1;}
	
	h = aux.substring(0,2).toInt();
	m = aux.substring(3,5).toInt();
	
	if(h >= 0 && h < 24 && m >= 0 && m < 60){
		command.hour = h;
		command.minute = m;
		return 0;
	} 
  else{
    return -1;
  }

}

int stringToInt( String st) {
    int val;
    char *next;
	char aux[10];
	st.toCharArray(aux,10);

        val = strtol (aux, &next, 10);

        // Check for empty string and characters left after conversion.

        if ((next == "") || (*next != '\0')) {
            return -1;
        } else {
            return val;
        }

}
