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

/* Number of maxium words that an issued command could have */
#define MAX_WORDS 	( 6 )

/*----------TYPEDEFS------------------------------------------------------------*/

/*----------VARIABLES-----------------------------------------------------------*/



//int errorCount=0;
extern int nUsers;



/*----------FUNCTIONS-----------------------------------------------------------*/



void parseInput(){

	
		
		/* Auxiliar variables to allow input treatment */
		int endFirstWord = 0;
		int nwords=0;
		String word;
		String input;
		
		/* Array to store all the words of the command */
		String words[MAX_WORDS+1];
		
		/* Actually read and store the input */
		input = msg.text; 
		input.trim();
		input.toLowerCase();
		
		#ifdef DEBUG
			Serial.print("User: ");
			Serial.print(msg.id);
			Serial.print(" Msg recieved: \"");
			Serial.print(input);
			Serial.println("\"");
		#endif
		
		/* Initialize the words array */
		for(int i=0;i<MAX_WORDS+1;i++){
			words[i]="";
		}

		/* Getting all the words that conforms the command into separate positions of the array */
		while(endFirstWord != -1 && nwords < MAX_WORDS+1){
			
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
				
		}
		
		/* In case that the string is longer than maxium words command*/
		serialFlush();
		
		
		
		
	if(!validUser(msg.id)){
		if(nUsers < MAX_USERS){
			if((words[0].equals("/start")) && (words[1].equals(""))){
				saveUser(msg.id);
				sendMessage(msg.id,F("Now you are a user!"));
			}
			else{
				sendMessage(msg.id,F("You are not a valid user. Try /start"));
			}
		}
		else{
			sendMessage(msg.id,F("Max users reached"));
		}
	}
	
	else{
		
		/* Understanding the command issued */
		
		if (words[0].equals("/user")){
			if(words[1].equals("del")){
				if(words[2].equals("")){
					delUser(msg.id);
					sendMessage(msg.id,F("You are no longer a valid user."));
				}
				else{
					getUser(msg.id)->errorcount++;
					errorHandler(msg.id,getUser(msg.id)->errorcount);
				return;
			}
			}
			else{
				getUser(msg.id)->errorcount++;
				errorHandler(msg.id,getUser(msg.id)->errorcount);
				return;
			}
		}
		
		else if(words[0].equals("/open")){
			command.commandName = 0;
			command.movement = 0;
			if(words[1].equals("")){
				getUser(msg.id)->errorcount = 0;
				return;
			}
			else{
				getUser(msg.id)->errorcount++;
				errorHandler(msg.id,getUser(msg.id)->errorcount);
				clearCommand();
				return;
			}
		}
		else if(words[0].equals("/in")){
			command.commandName = 0;
			command.movement = 1;
			if(words[1].equals("")){
				getUser(msg.id)->errorcount = 0;
				return;
			}
			else{
				getUser(msg.id)->errorcount++;
				errorHandler(msg.id,getUser(msg.id)->errorcount);
				clearCommand();
				return;
			}
		}
		else if(words[0].equals("/out")){
			command.commandName = 0;
			command.movement = 2;
			if(words[1].equals("")){
				getUser(msg.id)->errorcount = 0;
				return;
			}
			else{
				getUser(msg.id)->errorcount++;
				errorHandler(msg.id,getUser(msg.id)->errorcount);
				clearCommand();
				return;
			}
		}
		else if(words[0].equals("/close")){
			command.commandName = 0;
			command.movement = 3;
			if(words[1].equals("")){
				getUser(msg.id)->errorcount = 0;
				return;
			}
			else{
				getUser(msg.id)->errorcount++;
				errorHandler(msg.id,getUser(msg.id)->errorcount);
				clearCommand();
				return;
			}
		}

		else if (words[0].equals("/prog")){
			command.commandName = 1;
			int modifierFlag = 0;
			
			if(words[1].equals("show")){
				if(words[2].equals("")){
					command.modifierFlag = 2;
					getUser(msg.id)->errorcount = 0;
					return;
				}
				else{
					getUser(msg.id)->errorcount++;
					errorHandler(msg.id,getUser(msg.id)->errorcount);
					clearCommand();
					return;
				}
			}
			
			if (words[1].equals("del")){
				if(words[2].equals("all")){
					if(words[3].equals("")){
						command.modifierFlag = 3;
						getUser(msg.id)->errorcount = 0;
						return;
					}
					else{
						getUser(msg.id)->errorcount++;
						errorHandler(msg.id,getUser(msg.id)->errorcount);
						clearCommand();
						return;
					}
				}
				else{
					command.modifierFlag = 1;
					modifierFlag = 1;
				}
			}
			command.day = stringToInt(words[1+modifierFlag]);
			if((command.day >= 1) && (command.day <=7)) {
				if (checkDateIntegrity(words[2+modifierFlag]) == 0){
					if(!modifierFlag){
						command.modifierFlag = 0;
						if(words[3+modifierFlag].equals("open")){command.movement = 0;}
						else if (words[3+modifierFlag].equals("in")){command.movement = 1;}
						else if (words[3+modifierFlag].equals("out")){command.movement = 2;}
						else if (words[3+modifierFlag].equals("close")){command.movement = 3;}
						else{
							getUser(msg.id)->errorcount++;
							errorHandler(msg.id,getUser(msg.id)->errorcount);
							clearCommand();
							return;
						}
					}
					if(words[4].equals("")){
						getUser(msg.id)->errorcount = 0;
						return;
					}
					else{
						getUser(msg.id)->errorcount++;
						errorHandler(msg.id,getUser(msg.id)->errorcount);
						clearCommand();
						return;
					}
					
				}
				else{
					getUser(msg.id)->errorcount++;
					errorHandler(msg.id,getUser(msg.id)->errorcount);
					clearCommand();
					return;
				}
			}
			else{
				getUser(msg.id)->errorcount++;
				errorHandler(msg.id,getUser(msg.id)->errorcount);
				clearCommand();
				return;
			}
		}
		else if(words[0].equals("/when")){
			int actionIssued = action.movement;
			int movement;
			if(words[1].equals("allin")){
				if(words[2].equals("open")){movement = 0;}
				else if (words[2].equals("in")){movement = 1;}
				else if (words[2].equals("out")){movement = 2;}
				else if (words[2].equals("close")){movement = 3;}
				else{
					getUser(msg.id)->errorcount++;
					errorHandler(msg.id,getUser(msg.id)->errorcount);
					return;
				}
				if(words[3].equals("")){
					if(actionIssued == -1){
						/*AQUEST SERIAL S'HA DE MOURE A CHECKNEXTACTION()*/
						Serial.print(F("You entered: /when allin "));
						if(movement == 0){Serial.println("open");}
						else if(movement == 1){Serial.println("in");}
						else if(movement == 2){Serial.println("out");}
						else if(movement == 3){Serial.println("close");}
						action.movement = movement;
						action.npetsIn = npets;
						action.npetsOut = 0;
						getUser(msg.id)->errorcount = 0;
						sendMessage(msg.id,F("Okey!"));
					}
					else{
						getUser(msg.id)->errorcount = 0;
						errorHandler(msg.id,-1); 
						return;
					}
				}
				else{
					getUser(msg.id)->errorcount++;
					errorHandler(msg.id,getUser(msg.id)->errorcount);
					return;
				}
			}
			else if(words[1].equals("allout")){
				if(words[2].equals("open")){movement = 0;}
				else if (words[2].equals("in")){movement = 1;}
				else if (words[2].equals("out")){movement = 2;}
				else if (words[2].equals("close")){movement = 3;}
				else{
					getUser(msg.id)->errorcount++;
					errorHandler(msg.id,getUser(msg.id)->errorcount);
					return;
				}
				if(words[3].equals("")){
					if(actionIssued == -1){
						/*AQUEST SERIAL S'HA DE MOURE A CHECKNEXTACTION()*/
						Serial.print(F("You entered: /when allout "));
						if(movement == 0){Serial.println("open");}
						else if(movement == 1){Serial.println("in");}
						else if(movement == 2){Serial.println("out");}
						else if(movement == 3){Serial.println("close");}
						action.movement = movement;
						action.npetsIn = 0;
						action.npetsOut = npets;
						getUser(msg.id)->errorcount = 0;
						sendMessage(msg.id,F("Okey!"));
					}
					else{
						getUser(msg.id)->errorcount = 0;
						errorHandler(msg.id,-1); 
						return;
					}
				}
				else{
					getUser(msg.id)->errorcount++;
					errorHandler(msg.id,getUser(msg.id)->errorcount);
					return;
				}
			}
			else if(stringToInt(words[1]) != -1){
				if(words[2].equals("in")){
					if(stringToInt(words[3]) != -1){
						if(words[4].equals("out")){
							if(words[5].equals("open")){movement = 0;}
							else if (words[5].equals("in")){movement = 1;}
							else if (words[5].equals("out")){movement = 2;}
							else if (words[5].equals("close")){movement = 3;}
							else{
								getUser(msg.id)->errorcount++;
								errorHandler(msg.id,getUser(msg.id)->errorcount);
								return;
							}
							if(words[6].equals("")){
								if((words[1].toInt() + words[3].toInt()) == npets){
									if(actionIssued == -1){
										/*AQUEST SERIAL S'HA DE MOURE A CHECKNEXTACTION()*/
										Serial.print(F("You entered: /when "));
										Serial.print(words[1].toInt());
										Serial.print(F(" in "));
										Serial.print(words[3].toInt());
										if(movement == 0){Serial.println(F(" out open"));}
										else if(movement == 1){Serial.println(F(" out in"));}
										else if(movement == 2){Serial.println(F(" out out"));}
										else if(movement == 3){Serial.println(F(" out close"));}
										action.movement = movement;
										action.npetsIn = words[1].toInt();
										action.npetsOut = words[3].toInt();
										getUser(msg.id)->errorcount = 0;
										sendMessage(msg.id,F("Okey!"));
									}
									else{
										getUser(msg.id)->errorcount=0;
										errorHandler(msg.id,-1); 
										return;
									}
								}
								else{
									getUser(msg.id)->errorcount=0 ;
									errorHandler(msg.id,-2);
									return;
								}
							}
							else{
								getUser(msg.id)->errorcount++;
								errorHandler(msg.id,getUser(msg.id)->errorcount); 
								return;
							}
						}
						else{
							getUser(msg.id)->errorcount++;
							errorHandler(msg.id,getUser(msg.id)->errorcount);
							return;
						}
						
					}
					else{
						getUser(msg.id)->errorcount++;
						errorHandler(msg.id,getUser(msg.id)->errorcount); 
						return;
					}
				}
				else if (words[2].equals("out")){
					if(stringToInt(words[3]) != -1){
						if(words[4].equals("in")){
							if(words[5].equals("open")){movement = 0;}
							else if (words[5].equals("in")){movement = 1;}
							else if (words[5].equals("out")){movement = 2;}
							else if (words[5].equals("close")){movement = 3;}
							else{
								getUser(msg.id)->errorcount++;
								errorHandler(msg.id,getUser(msg.id)->errorcount);
								return;
							}
							if(words[6].equals("")){
								if((words[1].toInt() + words[3].toInt()) == npets){
									if(actionIssued == -1){
										/*AQUEST SERIAL S'HA DE MOURE A CHECKNEXTACTION()*/
										Serial.print(F("You entered: /when "));
										Serial.print(words[1].toInt());
										Serial.print(F(" out "));
										Serial.print(words[3].toInt());
										if(movement == 0){Serial.println(F(" in open"));}
										else if(movement == 1){Serial.println(F(" in in"));}
										else if(movement == 2){Serial.println(F(" in out"));}
										else if(movement == 3){Serial.println(F(" in close"));}
										action.movement = movement;
										action.npetsIn = words[3].toInt();
										action.npetsOut = words[1].toInt();
										getUser(msg.id)->errorcount = 0;
										sendMessage(msg.id,F("Okey!"));
									}
									else{
										getUser(msg.id)->errorcount=0;
										errorHandler(msg.id,-1);
										return;
									}
								}
								else{
									getUser(msg.id)->errorcount=0;
									errorHandler(msg.id,-2);
									return;
								}
							}
							else{
								getUser(msg.id)->errorcount++;
								errorHandler(msg.id,getUser(msg.id)->errorcount);
								return;
							}
						}
						else{
							getUser(msg.id)->errorcount++;
							errorHandler(msg.id,getUser(msg.id)->errorcount);
							return;
						}
						
					}
					else{
						getUser(msg.id)->errorcount++;
						errorHandler(msg.id,getUser(msg.id)->errorcount);
						return;
					}
				}
				else{
					getUser(msg.id)->errorcount++;
					errorHandler(msg.id,getUser(msg.id)->errorcount);
					return;
				}
			}
			else if(words[1].equals("show")){
				if(words[2].equals("")){
					if(actionIssued != -1){
						/*AQUEST SERIAL S'HA DE MOURE A CHECKNEXTACTION()*/
						Serial.println(F("You entered: /when show"));
						Serial.print(F("Next action: "));
						if(action.movement == 0){Serial.print(F("open"));}
						else if(action.movement == 1){Serial.print(F("in"));}
						else if(action.movement == 2){Serial.print(F("out"));}
						else if(action.movement == 3){Serial.print(F("close"));}
						Serial.print(F(" when "));
						Serial.print(action.npetsIn);
						Serial.print(F(" IN and "));
						Serial.print(action.npetsOut);
						Serial.println(F(" OUT."));
						getUser(msg.id)->errorcount = 0;
					}
					else{
						getUser(msg.id)->errorcount=0;
						errorHandler(msg.id,-4);
						return;
					}
				}
				else{
					getUser(msg.id)->errorcount++;
					errorHandler(msg.id,getUser(msg.id)->errorcount); 
					return;
				}
			}
			else if(words[1].equals("del")){
				if(words[2].equals("")){
					if(actionIssued != -1){
						/*AQUEST SERIAL S'HA DE MOURE A CHECKNEXTACTION()*/
						Serial.println(F("You entered: /when del"));
						clearNextAction();
						getUser(msg.id)->errorcount = 0;
						sendMessage(msg.id,F("/when command deleted!"));
					}
					else{
						getUser(msg.id)->errorcount =0;
						errorHandler(msg.id,-3);   
						return;
					}
				}
				else{
					getUser(msg.id)->errorcount++;
					errorHandler(msg.id,getUser(msg.id)->errorcount);  
					return;
				}
			}
			else{
				getUser(msg.id)->errorcount++;
				errorHandler(msg.id,getUser(msg.id)->errorcount);
				return;
			}
		
		}
		else if(words[0].equals("/where")){
			if(words[1].equals("")){
				command.commandName = 2;
				getUser(msg.id)->errorcount = 0;
			}
			else{
				getUser(msg.id)->errorcount++;
				errorHandler(msg.id,getUser(msg.id)->errorcount);
				return;
			}
		}
		else if (words[0].equals("/help")){
			if(words[1].equals("") && words[2].equals("")){
				showHelp(msg.id,0);
				getUser(msg.id)->errorcount = 0;
			}
			else if(words[1].equals("usual") && words[2].equals("")){
				showHelp(msg.id,1);
				getUser(msg.id)->errorcount = 0;
			}
			else if(words[1].equals("prog") && words[2].equals("")){
				showHelp(msg.id,2);
				getUser(msg.id)->errorcount = 0;
			}
			else if(words[1].equals("when") && words[2].equals("")){
				showHelp(msg.id,3);
				getUser(msg.id)->errorcount = 0;
			}
			else if(words[1].equals("actions") && words[2].equals("")){
				showHelp(msg.id,4);
				getUser(msg.id)->errorcount = 0;
			}
			else if(words[1].equals("dow") && words[2].equals("")){
				showHelp(msg.id,5);
				getUser(msg.id)->errorcount = 0;
			}
			else{
				getUser(msg.id)->errorcount++;
				errorHandler(msg.id,getUser(msg.id)->errorcount);  
				return;
			}
			
			
		}
		else{
			getUser(msg.id)->errorcount++;
			errorHandler(msg.id,getUser(msg.id)->errorcount);
			return;
		}
	}
}

void errorHandler(uint32_t id, int e){
	if(e == 1) sendMessage(id,F("Oops! You misspelled something...😹"));
	else if(e == 2) sendMessage(id,F("Again...🙀"));
	else if(e == 3) sendMessage(id,F("Really???😿 Chek /help pls..."));
	else if(e >= 4) sendMessage(id,F("Check /help and stop bothering me...😾"));
	else if(e == -1) sendMessage(id,F("There is already a \"/when\" next action issued. Use \"/when show\" or \"/when del\" before."));
	else if(e == -2) sendMessage(id,F("Do you know how many pets live with you?"));
	else if(e == -3) sendMessage(id,F("Nothing to delete. There is not a \"/when\" next action issued."));
	else if(e == -4) sendMessage(id,F("Nothing to show. There is not a \"/when\" next action issued."));
	
	
}

void showHelp(uint32_t id, int commands){
	if(commands == 0){
		sendMessage(id,F("Intelligent pet door help:\n\n/help usual - Usual commands help.\n/help prog - /prog help\n/help when - /when help\n/help actions - Available actions\n/help dow - Days of the week"));
	}
	else if(commands == 1){
		sendMessage(id,F("Usual commands:\n\n/open - Opens the door\n/in - Allows pets go in but not go out\n/out - Allows pets go out but not go in\n/close - Closes the door\n/where - Shows where are the pets"));
	}
	else if(commands == 2){
		sendMessage(id,F("Program commands:\n\n/prog {DoW} HH:MM {action} - Save command\n/prog del {DoW} HH:MM - Delete command\n/prog del all - Delete all commands\n/prog show - Show commands"));
	}
	else if(commands == 3){
		sendMessage(id,F("When commands:\n\n/when {allin | allout | X in Y out} {action} - When conditons met do the action\n/when show - Shows the next action\n/when del - Deletes the next action"));
	}
	else if(commands == 4){
		sendMessage(id,F("Available actions:\n\nopen - pets have free movment\nin - pets can go in but not go out.\nout - pets can go out but not go in\nclose - the door is closed"));
	}
	else if(commands == 5){
		sendMessage(id,F("Days of week:\n\n1 - Monday\n2 - Tuesday\n3 - Wednesday\n4 - Thursday\n5 - Friday\n6 - Saturday\n7 - Sunday"));
	}
}

int checkDateIntegrity(String st){
	
	/* Auxiliar variables */
	int h;
	int m;
	int timeSpacer = -1;
	
	/* Hour incorrect length */
	if((st.length() < 3) || (st.length() > 5)) return -1;
	
	/* Get time spacer position */
	timeSpacer = st.indexOf(':');
	if (timeSpacer == -1) timeSpacer = st.indexOf('.');
	
	/* If we got it  */
	if(timeSpacer > 0 ){
		
		/* Check all positions for a digit */
		for(int i=0;i<st.length();i++){
			
			/* Skip time spacer position */
			if (i==timeSpacer)i++;
			
			if (!isDigit(st.charAt(i))) return -1;
		}
				
		/* If all is correct get hour and minute */
		h = st.substring(0,timeSpacer).toInt();
		m = st.substring(timeSpacer+1,st.length()).toInt();

	}
	else return -1;
	
	/* Last check for a correct numbers */
	if(h >= 0 && h < 24 && m >= 0 && m < 60){
		
			/* All correct fill command struct */
			command.hour = h;
			command.minute = m;
			return 0;
	} 
	else{
		return -1;
	}

}

/* Converts number in string to int data type */
int stringToInt( String st) {
    int val;
    char *next;
	char aux[10];
	st.toCharArray(aux,10);

	val = strtol (aux, &next, 10);

	/* Check for empty string and characters left after conversion. */
	if ((next == "") || (*next != '\0')) {
		return -1;
	} else {
		return val;
	}

}

/*----------INTERRUPTS----------------------------------------------------------*/
