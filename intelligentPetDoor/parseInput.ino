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

extern uint8_t nUsers;
extern uint8_t npets;

/*----------FUNCTIONS-----------------------------------------------------------*/

/* Checks input from user, understands it and do the proper actions */
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
		Serial.print(F("User: "));
		Serial.print(msg.id);
		Serial.print(F(" Msg recieved: \""));
		Serial.print(input);
		Serial.println(F("\""));
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
		
	/* First you have to be a registered user */	
	if(!validUser(msg.id)){
		if(nUsers < MAX_USERS){
			if((words[0].equals(F("/start"))) && (words[1].equals(F("")))){
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
	
	/* Second you have to set up number of pets */
	else if(npets == 0xFF){
		if(words[0].equals(F("/pet"))){
			if((words[1].equals(F("number"))) || (words[1].equals(F("n")))){
				int npetsAux = stringToInt(words[2]);
				if(npetsAux > 0 && npetsAux < 255){
					if(words[3].equals(F(""))){
						definePetsNumber(npetsAux);
					}
					else{
						sendMessage(msg.id,F("No defined pets. Try /pet number {your number of pets}"));
						return;
					}
				}
				else{
					sendMessage(msg.id,F("No defined pets. Try /pet number {your number of pets}"));
					return;
				}
			}
			else{
				sendMessage(msg.id,F("No defined pets. Try /pet number {your number of pets}"));
				return;
			}
		}
		else{
			sendMessage(msg.id,F("No defined pets. Try /pet number {your number of pets}"));
			return;
		}
	}
	
	/* Now you can send commands */
	else{
		
		/* Understanding the command issued */
		if ((words[0].equals(F("/user"))) || (words[0].equals(F("/u")))){
			if(words[1].equals(F("del"))){
				if(words[2].equals(F(""))){
					delUser(msg.id);
					sendMessage(msg.id,F("You are no longer a valid user."));
				}
				else{
					getUser(msg.id)->errorcount++;
					errorHandler(msg.id,getUser(msg.id)->errorcount);
					return;
				}
			}
			else if((words[1].equals(F("tnotifications"))) || (words[1].equals(F("tn")))){
				if(words[2].equals(F(""))){
					uint8_t newNotificationsValue = toggleNotifications(msg.id);
					if(newNotificationsValue & 0x01) sendMessage(msg.id,F("Notifications enabled 💪"));
					else sendMessage(msg.id,F("Notifications disabled 👎"));
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
		
		else if(words[0].equals(F("/pet"))){
			if((words[1].equals(F("number"))) || (words[1].equals(F("n")))){
				int npetsAux = stringToInt(words[2]);
				if(npetsAux > 0 && npetsAux < 255){
					if(words[3].equals(F(""))){
						definePetsNumber(npetsAux);
						
						#ifdef DEBUG
							Serial.println((String) F("Number of pets updated to ") + npets);
						#endif
						
						sendMessage(msg.id,(String) F("Number of pets updated to ") + npets);
						
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
			else if((words[1].equals(F("show"))) || (words[1].equals(F("sh")))){
				
				if(words[2].equals(F(""))){
					#ifdef DEBUG
						Serial.println((String) F("You have ") + npets + F("pets defined."));
					#endif
					
					sendMessage(msg.id,(String) F("You have ") + npets + F("pets defined."));
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
		
		else if(words[0].equals(F("/open"))){
			command.commandName = 0;
			command.movement = 0;
			if(words[1].equals(F(""))){
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
		else if(words[0].equals(F("/in"))){
			command.commandName = 0;
			command.movement = 1;
			if(words[1].equals(F(""))){
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
		else if(words[0].equals(F("/out"))){
			command.commandName = 0;
			command.movement = 2;
			if(words[1].equals(F(""))){
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
		else if(words[0].equals(F("/close"))){
			command.commandName = 0;
			command.movement = 3;
			if(words[1].equals(F(""))){
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

		else if (words[0].equals(F("/prog")) || words[0].equals(F("/program")) || words[0].equals(F("/p"))){
			command.commandName = 1;
			int modifierFlag = 0;
			
			if(words[1].equals(F("show")) || words[1].equals(F("sh"))){
				if(words[2].equals(F(""))){
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
			
			if (words[1].equals(F("del"))){
				if(words[2].equals(F("all"))){
					if(words[3].equals(F(""))){
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
						if(words[3+modifierFlag].equals(F("open"))){command.movement = 0;}
						else if (words[3+modifierFlag].equals(F("in"))){command.movement = 1;}
						else if (words[3+modifierFlag].equals(F("out"))){command.movement = 2;}
						else if (words[3+modifierFlag].equals(F("close"))){command.movement = 3;}
						else{
							getUser(msg.id)->errorcount++;
							errorHandler(msg.id,getUser(msg.id)->errorcount);
							clearCommand();
							return;
						}
					}
					if(words[4].equals(F(""))){
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
		else if(words[0].equals(F("/when"))){
			uint8_t actionIssued = action.movement;
			int movement;
			if(words[1].equals(F("allin"))){
				if(words[2].equals(F("open"))){movement = 0;}
				else if (words[2].equals(F("in"))){movement = 1;}
				else if (words[2].equals(F("out"))){movement = 2;}
				else if (words[2].equals(F("close"))){movement = 3;}
				else{
					getUser(msg.id)->errorcount++;
					errorHandler(msg.id,getUser(msg.id)->errorcount);
					return;
				}
				if(words[3].equals(F(""))){
					if(actionIssued == 0xFF){
						#ifdef DEBUG
							Serial.print(F("You entered: /when allin "));
							if(movement == 0){Serial.println(F("open"));}
							else if(movement == 1){Serial.println(F("in"));}
							else if(movement == 2){Serial.println(F("out"));}
							else if(movement == 3){Serial.println(F("close"));}
						#endif
						
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
			else if(words[1].equals(F("allout"))){
				if(words[2].equals(F("open"))){movement = 0;}
				else if (words[2].equals(F("in"))){movement = 1;}
				else if (words[2].equals(F("out"))){movement = 2;}
				else if (words[2].equals(F("close"))){movement = 3;}
				else{
					getUser(msg.id)->errorcount++;
					errorHandler(msg.id,getUser(msg.id)->errorcount);
					return;
				}
				if(words[3].equals(F(""))){
					if(actionIssued == 0xFF){
						#ifdef DEBUG
							Serial.print(F("You entered: /when allout "));
							if(movement == 0){Serial.println(F("open"));}
							else if(movement == 1){Serial.println(F("in"));}
							else if(movement == 2){Serial.println(F("out"));}
							else if(movement == 3){Serial.println(F("close"));}
						#endif
						
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
				if(words[2].equals(F("in"))){
					if(stringToInt(words[3]) != -1){
						if(words[4].equals(F("out"))){
							if(words[5].equals(F("open"))){movement = 0;}
							else if (words[5].equals(F("in"))){movement = 1;}
							else if (words[5].equals(F("out"))){movement = 2;}
							else if (words[5].equals(F("close"))){movement = 3;}
							else{
								getUser(msg.id)->errorcount++;
								errorHandler(msg.id,getUser(msg.id)->errorcount);
								return;
							}
							if(words[6].equals(F(""))){
								if((words[1].toInt() + words[3].toInt()) == npets){
									if(actionIssued == 0xFF){
										#ifdef DEBUG
											Serial.print(F("You entered: /when "));
											Serial.print(words[1].toInt());
											Serial.print(F(" in "));
											Serial.print(words[3].toInt());
											if(movement == 0){Serial.println(F(" out open"));}
											else if(movement == 1){Serial.println(F(" out in"));}
											else if(movement == 2){Serial.println(F(" out out"));}
											else if(movement == 3){Serial.println(F(" out close"));}
										#endif
										
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
				else if (words[2].equals(F("out"))){
					if(stringToInt(words[3]) != -1){
						if(words[4].equals(F("in"))){
							if(words[5].equals(F("open"))){movement = 0;}
							else if (words[5].equals(F("in"))){movement = 1;}
							else if (words[5].equals(F("out"))){movement = 2;}
							else if (words[5].equals(F("close"))){movement = 3;}
							else{
								getUser(msg.id)->errorcount++;
								errorHandler(msg.id,getUser(msg.id)->errorcount);
								return;
							}
							if(words[6].equals(F(""))){
								if((words[1].toInt() + words[3].toInt()) == npets){
									if(actionIssued == 0xFF){
										#ifdef DEBUG
											Serial.print(F("You entered: /when "));
											Serial.print(words[1].toInt());
											Serial.print(F(" out "));
											Serial.print(words[3].toInt());
											if(movement == 0){Serial.println(F(" in open"));}
											else if(movement == 1){Serial.println(F(" in in"));}
											else if(movement == 2){Serial.println(F(" in out"));}
											else if(movement == 3){Serial.println(F(" in close"));}
										#endif
										
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
			else if(words[1].equals(F("show"))){
				if(words[2].equals(F(""))){
					if(actionIssued != 0xFF){
						#ifdef DEBUG
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
						#endif
						
						if(action.npetsIn == npets) {
							if(action.movement == 0) sendMessage(msg.id,F("When all pets are inside I will OPEN the door"));
							else if(action.movement == 1) sendMessage(msg.id,F("When all pets are inside I will set the door at IN position"));
							else if(action.movement == 2) sendMessage(msg.id,F("When all pets are inside I will set the door at OUT position"));
							else if(action.movement == 3) sendMessage(msg.id,F("When all pets are inside I will CLOSE the door"));
						}
						else if (action.npetsOut == npets) {
							if(action.movement == 0) sendMessage(msg.id,F("When all pets are outside I will OPEN the door"));
							else if(action.movement == 1) sendMessage(msg.id,F("When all pets are outside I will set the door at IN position"));
							else if(action.movement == 2) sendMessage(msg.id,F("When all pets are outside I will set the door at OUT position"));
							else if(action.movement == 3) sendMessage(msg.id,F("When all pets are outside I will CLOSE the door"));
						}
						else{
							if(action.movement == 0) sendMessage(msg.id,(String)F("When ")+ action.npetsIn +F(" pets are inside and ")+action.npetsOut+F(" are outside I will OPEN the door"));
							else if(action.movement == 1) sendMessage(msg.id,(String)F("When ")+ action.npetsIn +F("pets are inside and ")+action.npetsOut+F(" are outside I will set the door at IN position"));
							else if(action.movement == 2) sendMessage(msg.id,(String)F("When ")+ action.npetsIn +F("pets are inside and ")+action.npetsOut+F(" are outside I will set the door at OUT position"));
							else if(action.movement == 3) sendMessage(msg.id,(String)F("When ")+ action.npetsIn +F("pets are inside and ")+action.npetsOut+F(" are outside I will CLOSE the door"));
						}
						
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
			else if(words[1].equals(F("del"))){
				if(words[2].equals(F(""))){
					if(actionIssued != 0xFF){
						#ifdef DEBUG
							Serial.println(F("You entered: /when del"));
						#endif
						
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
		else if(words[0].equals(F("/where"))){
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
		else if (words[0].equals(F("/help"))){
			if(words[1].equals("") && words[2].equals("")){
				showHelp(msg.id,0);
				getUser(msg.id)->errorcount = 0;
			}
			else if(words[1].equals(F("usual")) && words[2].equals("")){
				showHelp(msg.id,1);
				getUser(msg.id)->errorcount = 0;
			}
			else if(words[1].equals(F("prog")) && words[2].equals("")){
				showHelp(msg.id,2);
				getUser(msg.id)->errorcount = 0;
			}
			else if(words[1].equals(F("when")) && words[2].equals("")){
				showHelp(msg.id,3);
				getUser(msg.id)->errorcount = 0;
			}
			else if(words[1].equals(F("actions")) && words[2].equals("")){
				showHelp(msg.id,4);
				getUser(msg.id)->errorcount = 0;
			}
			else if(words[1].equals(F("dow")) && words[2].equals("")){
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
