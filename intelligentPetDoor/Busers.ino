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

#define MAX_USERS	( 4 )
#define USER_SIZE	( 4 )

/*----------TYPEDEFS------------------------------------------------------------*/

typedef struct User {
	uint32_t id;
	int errorcount;
} User;

/*----------VARIABLES-----------------------------------------------------------*/

int nUsers;
User users[MAX_USERS];

/*----------FUNCTIONS-----------------------------------------------------------*/

void initUsers(){
	
	nUsers=0;
	for(int i=0;i<MAX_USERS;i++) users[i].id=0xFFFFFFFF;
	
	uint32_t userAux;
	
	Serial.print("Valid users: ");
	
	for(int i=USERS_POS;i<USERS_POS+(MAX_USERS*USER_SIZE);i=i+USER_SIZE){
		
		userAux = readEEPROM(i+3) | (uint32_t)readEEPROM(i+2) << 8 | (uint32_t)readEEPROM(i+1) << 16 | (uint32_t)readEEPROM(i) << 24;
		
		Serial.print(userAux);
		Serial.print(", ");
		
		if(userAux != 0xFFFFFFFF){
			users[nUsers].id=userAux;
			users[nUsers].errorcount=0;
			nUsers++;
		}

	}
}

int saveUser(uint32_t id){
	
	for(int i=0;i<MAX_USERS;i++){
		
		if(users[i].id == 0xFFFFFFFF){
			writeEEPROM(USERS_POS+(i*USER_SIZE),(id>>24));
			writeEEPROM(USERS_POS+(i*USER_SIZE)+1,(id>>16));
			writeEEPROM(USERS_POS+(i*USER_SIZE)+2,(id>>8));
			writeEEPROM(USERS_POS+(i*USER_SIZE)+3,id);
			users[i].id=id;
			users[i].errorcount=0;
			return 1;
		}
	}
	
	return 0;
}

int delUser(uint32_t id){
	
	for(int i=0;i<MAX_USERS;i++){
		
		if(users[i].id == id){
			writeEEPROM(USERS_POS+(i*USER_SIZE),0xFF);
			writeEEPROM(USERS_POS+(i*USER_SIZE)+1,0xFF);
			writeEEPROM(USERS_POS+(i*USER_SIZE)+2,0xFF);
			writeEEPROM(USERS_POS+(i*USER_SIZE)+3,0xFF);
			users[i].id=0xFFFFFFFF;
			return 1;
		}
	}
	
	return 0;
}

struct User* getUser(uint32_t id){
	for(int i=0;i<MAX_USERS;i++){
		
		if(users[i].id == id){
			return &users[i];
		}
	}

}


int validUser(uint32_t u){
	for(int i =0;i<MAX_USERS;i++){
		if(users[i].id == u) return 1;
	}
	return 0;
}




/*----------INTERRUPTS----------------------------------------------------------*/