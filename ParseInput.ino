int const space = 32;
int const carriageReturn = 10;
int const commandNow = 0;
int const commandProgram = 1;
int const movementFree = 0;
int const movementIn = 1;
int const movementOut = 2;
int const movementClose = 3;



commandIssued parseInput(){
  int incomingByte;
  commandIssued command;
  command.commandName = -1;
  command.hour = -1;
  command.minute = -1;
  command.movement = -1;
  
  if(Serial.available() > 0){
    incomingByte = Serial.read();
    if(incomingByte != '/'){
      serialFlush();
      Serial.println("Incorrect command. /usage to help."); 
    }
    else{
      char input[100];
      int i = 0;
      
      while (Serial.available() > 0){
        //reading word
        incomingByte = Serial.read();
        if ((incomingByte != space) && (incomingByte != carriageReturn)){
          input[i] = incomingByte;
          i++;  
        }
        //understanding word
        else {
          input[i] = '\0';
          //we have no command 
          if(command.commandName == -1){
            if(strcmp(input,"now")==0){
              command.commandName = 0;
            }
            else if(strcmp(input,"program")==0){
              command.commandName = 1;
            }
            else if(strcmp(input,"usage")==0){
              serialFlush();
              showUsage();
              break;
            }
            else{
              serialFlush();
              Serial.println("Incorrect command. /usage to help."); 
              break;
            }
          }
          //we have NOW command and we have to read a parameter
          else if (command.commandName == 0){
            if(command.movement == -1){
              if(strcmp(input,"free")==0){
                command.movement = 0;
                serialFlush();
                return command;
              }
              else if (strcmp(input,"in")==0){
                command.movement = 1;
                serialFlush();
                return command;
              }
              else if (strcmp(input,"out")==0){
                command.movement = 2;
                serialFlush();
                return command;
              }
              else if (strcmp(input,"close")==0){
                command.movement = 3;
                serialFlush();
                return command;
              }
              else{
                serialFlush();
                Serial.println("Incorrect action. /usage to help.");  
                break;
              }
            }
          }
        //we have PROGRAM command and we have to read two parameters
          else if (command.commandName == 1){
            //hour and minutes not set.
            if (command.hour == -1 && command.minute == -1){
              char *token;
              token = strtok(input,":");
              char* hour = token;
              token = strtok(NULL,":");
              char* minute = token;
              token = strtok(NULL,":");
              //if there are more than two tokens error
              if (token != NULL){
                serialFlush();
                Serial.println("Correct format for hour is HH:MM. /usage to help.");  
                break;
              }
              //check tokens integrity
              int h = checkIntegrity(hour);
              int m = checkIntegrity(minute);
  
              /*if((h == -1) || (m == -1)){
                serialFlush();
                Serial.println("Correct format for hour is HH:MM. /usage to help."); 
                break;
              }*/
  
              if(h >= 0 && h <= 23 && m >= 0 && m <= 59){
                command.hour = h;
                command.minute = m;
              }
              else{
                serialFlush();
                Serial.println("Correct format for hour is HH:MM. /usage to help.");
                break;
              }
            }
            //hour and minutes already set so lets take movement
            else{
              if(strcmp(input,"free")==0){
                command.movement = 0;
                serialFlush();
                return command;
              }
              else if (strcmp(input,"in")==0){
                command.movement = 1;
                serialFlush();
                return command;
              }
              else if (strcmp(input,"out")==0){
                command.movement = 2;
                serialFlush();
                return command;
              }
              else if (strcmp(input,"close")==0){
                command.movement = 3;
                serialFlush();
                return command;
              }
              else{
                serialFlush();
                Serial.println("Incorrect action. /usage to help.");   
                break;
              }
            }
          }
          i=0;
        }
      }
    }
  }

  //in case of error return empty structure 
  command.commandName = -1;
  command.hour = -1;
  command.minute = -1;
  command.movement = -1;
  return command;
  
}

void showUsage(){
  Serial.println("USAGE:");
  Serial.println("Available commands are : /now /program /usage");
  Serial.println(" - /now {action}: When no pet is crossing the door performs the desired action.");
  Serial.println(" - /program HH:MM {action}: Program the desired action in the desired time of the day. Just one action per time.");
  Serial.println(" - /usage: Shows this help.");
  Serial.println("Available actions are:");
  Serial.println(" - free: pets have free movment.");
  Serial.println(" - in: pets can enter but not exit.");
  Serial.println(" - out: pets can exit but not enter.");
  Serial.println(" - close: the door is closed.");
}

void serialFlush(){
    while(Serial.available() > 0){
      int incomingByte = Serial.read();
    }
}

int checkIntegrity(char *st){
  int n;
  if(sscanf(st, "%d") == 1){
  return atoi(st);  
  } 
  else return -1;
}
