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

/* Library to define an alternative serial */
#include <SoftwareSerial.h>

/*----------VARIABLES-----------------------------------------------------------*/

/* Software serial object */
SoftwareSerial ESP8266Serial(SOFTWARE_RX, SOFTWARE_TX);

/* To store received messages from ESP8266 */
simpleTBMessage msg;

/*----------EXTERN--------------------------------------------------------------*/

extern void parseInput();
extern User users[MAX_USERS];

/*----------FUNCTIONS-----------------------------------------------------------*/

void initSerial(){
	
	/* If debug flag set initialize serial */
	#ifdef DEBUG
		Serial.begin(9600);
	#endif

	/* Initialize comunication with ESP8266 */
	ESP8266Serial.begin(9600);
	
}

void checkSerial(){
	
	/* If ESP8266 sent something to Arduino */
	if(ESP8266Serial.available()>0){
		
		/* Read it */
		ESP8266Serial.readBytes((uint8_t *) &msg, (uint16_t) sizeof(msg));
		
		/* And parse it */
		parseInput();
	}
	
}

void sendMessage(uint32_t id, String text){
	
	/* To store the outgoing message */
	simpleTBMessage sendingSmsg;
	
	/* Fill the id */
	sendingSmsg.id = id;
	
	/* Initialize text */
	//for(int i=0;i<MESSAGE_TEXT_LENGTH;i++) sendingSmsg.text[i] = '\0';
	
	/* Fill the text */
	text.toCharArray(sendingSmsg.text,MESSAGE_TEXT_LENGTH);
	
	/* Debug code */
	#ifdef DEBUG
		Serial.print(F("sendMessage("));
		Serial.print(sendingSmsg.id);
		Serial.print(F(","));
		Serial.print(sendingSmsg.text);
		Serial.println(F(")"));
	#endif
	
	/* Send struct to ESP8266 */
	ESP8266Serial.write((uint8_t *) &sendingSmsg, (uint16_t) sizeof(sendingSmsg));
	
	/* Wait for reception */
	delay(1000);
	
}

/* Send message to all users with notifications enabled */
void sendMessageToAllUsers(String text){
	
	for(int i=0;i<MAX_USERS;i++){
		
		/* If user exists and has notifications enabled */
		if(users[i].id != 0xFFFFFFFF && (users[i].notificationsEnabled & 0x01)){
			
			/* Debug code */
			#ifdef DEBUG
				Serial.print(F("Message sent to: "));
				Serial.print(users[i].id);
				Serial.print(F(" text: \""));
				Serial.print(text);
				Serial.println(F("\""));
			#endif
			
			/* Send message */
			sendMessage(users[i].id,text);
			
			/* Give enougth time to ESP8266 to send messages */
			delay(4000);
		}
	}

}

/* Empties the serial input buffer */
void serialFlush(){
    while(ESP8266Serial.available() > 0){
      int incomingByte = Serial.read();
    }
}

/*----------INTERRUPTS----------------------------------------------------------*/