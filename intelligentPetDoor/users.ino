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

uint8_t nUsers;
User users[MAX_USERS];

/*----------FUNCTIONS-----------------------------------------------------------*/

void initUsers(){
	
	/* Sets user count to zero */
	nUsers=0;
	
	/* Initializes user array */
	for(int i=0;i<MAX_USERS;i++) users[i].id=0xFFFFFFFF;
	
	/* Auxiliar variable to store temporary information */
	User userAux;
	
	/* Debug code */
	#ifdef DEBUG
		Serial.print(F("Valid users: "));
	#endif
	
	/* Search users in EEPROM */
	for(int i=USERS_POS;i<USERS_POS+(MAX_USERS*USER_SIZE);i=i+USER_SIZE){
		
		/* Get possible user */
		userAux.id = readEEPROM(i+3) | (uint32_t)readEEPROM(i+2) << 8 | (uint32_t)readEEPROM(i+1) << 16 | (uint32_t)readEEPROM(i) << 24;
		
		/* Stores user info in the users array */
		if(userAux.id != 0xFFFFFFFF){
			
			/* Debug code */
			#ifdef DEBUG
				Serial.print(userAux.id);
				Serial.print(F(", "));
			#endif
			
			users[nUsers].id = userAux.id;
			users[nUsers].errorcount = 0;
			users[nUsers].notificationsEnabled = readEEPROM(i+4); 
			nUsers++;
		}

	}
	
	/* Debug code */
	#ifdef DEBUG
		Serial.println(F(""));
	#endif
	
}

void saveUser(uint32_t id){
	
	/* Auxiliar variable to store temporary information */
	uint32_t userAux;
	
	/* Search users in EEPROM */
	for(int i=USERS_POS;i<USERS_POS+(MAX_USERS*USER_SIZE);i=i+USER_SIZE){
		
		/* Get possible user */
		userAux = readEEPROM(i+3) | (uint32_t)readEEPROM(i+2) << 8 | (uint32_t)readEEPROM(i+1) << 16 | (uint32_t)readEEPROM(i) << 24;
		
		/* Stores user info in the EEPROM and users array */
		if(userAux == 0xFFFFFFFF){
			
			/* Debug code */
			#ifdef DEBUG
				Serial.println((String)F("User with ")+ id + F(" ID saved"));
			#endif
			
			/* Save user ID */
			writeEEPROM(i,(id>>24));
			writeEEPROM(i+1,(id>>16));
			writeEEPROM(i+2,(id>>8));
			writeEEPROM(i+3,id);
			
			/* Notifications enabled by default */
			writeEEPROM(i+4,0x01);
			
			/* Update info in users array */
			users[nUsers].id = id;
			users[nUsers].errorcount = 0;
			users[nUsers].notificationsEnabled = 1; 
			
			/* Increment number of users */
			nUsers++;
			
			/* Job done, bye */
			return;
		}

	}
	
}

void delUser(uint32_t id){
	
	/* Auxiliar variable to store temporary information */
	User userAux;
	
	/* Search user in EEPROM */
	for(int i=USERS_POS;i<USERS_POS+(MAX_USERS*USER_SIZE);i=i+USER_SIZE){
		
		/* Get possible user */
		userAux.id = readEEPROM(i+3) | (uint32_t)readEEPROM(i+2) << 8 | (uint32_t)readEEPROM(i+1) << 16 | (uint32_t)readEEPROM(i) << 24;
		
		/* Stores user info in the EEPROM and users array */
		if(userAux.id == id){
			
			/* Debug code */
			#ifdef DEBUG
				Serial.println((String)F("User with ")+ userAux.id + F(" ID deleted"));
			#endif
			
			/* Delete user ID */
			writeEEPROM(i,0xFF);
			writeEEPROM(i+1,0xFF);
			writeEEPROM(i+2,0xFF);
			writeEEPROM(i+3,0xFF);
			
			/* Update info in users array */
			users[nUsers].id = 0xFFFFFFFF;
			
			/* Decrement number of users */
			nUsers--;
		}

	}
	
}

uint8_t toggleNotifications(uint32_t id){
	
	/* Auxiliar variable to store temporary information */
	User userAux;
	
	/* Search user in EEPROM */
	for(int i=USERS_POS;i<USERS_POS+(MAX_USERS*USER_SIZE);i=i+USER_SIZE){
		
		/* Get possible user */
		userAux.id = readEEPROM(i+3) | (uint32_t)readEEPROM(i+2) << 8 | (uint32_t)readEEPROM(i+1) << 16 | (uint32_t)readEEPROM(i) << 24;
		
		/* Stores user info in the EEPROM and users array */
		if(userAux.id == id){
			
			/* Get actual state of notifications */
			userAux.notificationsEnabled = readEEPROM(i+4);
			
			/* We dont care the value, just toggle notificationsEnabled value stored in EEPROM */
			writeEEPROM(i+4,(userAux.notificationsEnabled^0x01));
			
			/* Debug code */
			#ifdef DEBUG
				Serial.println((String)F("User: ")+ id + F(" EEPROM notifications was ") + userAux.notificationsEnabled + F(" and now are ")+ (userAux.notificationsEnabled^0x01));
			#endif
			
			/* Search for the user in the array */
			for(int i=0;i<nUsers;i++){
				
				/* Once found also toggle the value */
				if(users[i].id == userAux.id){
					
					/* Debug code */
					#ifdef DEBUG
						Serial.print((String)F("User: ")+ id + F(" USERS_ARRAY notifications was ") + users[i].notificationsEnabled);
					#endif
					
					/* XOR value to toggle it */
					users[i].notificationsEnabled = users[i].notificationsEnabled^0x01;
					
					/* Debug code */
					#ifdef DEBUG
						Serial.println((String) F(" and now are ")+ users[i].notificationsEnabled);
					#endif
					
					/* Return new value for notifications enabled */
					return users[i].notificationsEnabled;
				}
			}

		}

	}
	
}

struct User* getUser(uint32_t id){
	for(int i=0;i<nUsers;i++){
		if(users[i].id == id){
			return &users[i];
		}
	}
}


uint8_t validUser(uint32_t u){
	for(int i =0;i<nUsers;i++){
		if(users[i].id == u) return 1;
	}
	return 0;
}

/*----------INTERRUPTS----------------------------------------------------------*/