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

/*----------DEFINES--------------------------------------------------------------*/

/* Maxium message length */
#define MESSAGE_TEXT_LENGTH	( 200 )

/* Software RX and TX */
#define SOFTWARE_RX	( 5 )
#define SOFTWARE_TX	( 4 )

/*----------TYPEDEFS------------------------------------------------------------*/

/* To store messages sent to ESP8266 */
typedef struct simpleTBMessage {
	uint32_t id;
	char text[MESSAGE_TEXT_LENGTH];
} simpleTBMessage;

/*----------VARIABLES-----------------------------------------------------------*/

/* Software serial object */
SoftwareSerial ESP8266Serial(SOFTWARE_RX, SOFTWARE_TX);

/* To store received messages from ESP8266 */
simpleTBMessage msg;

/*----------FUNCTIONS-----------------------------------------------------------*/

void initSerial(){
	
	#ifdef DEBUG
		Serial.begin(9600);
	#endif

	ESP8266Serial.begin(9600);
	
}

void checkSerial(){
	
	if(ESP8266Serial.available()>0){
		
		ESP8266Serial.readBytes((uint8_t *) &msg, (uint16_t) sizeof(msg));
		
		parseInput();
	}
	
}

void sendMessage(uint32_t id, String text){
	
	simpleTBMessage sendingSmsg;
	
	sendingSmsg.id = id;
	text.toCharArray(sendingSmsg.text,MESSAGE_TEXT_LENGTH);
	
	ESP8266Serial.write((uint8_t *) &sendingSmsg, (uint16_t) sizeof(sendingSmsg));
	
	delay(1000);
	
}

void sendMessageToAllUsers(String text){
	
	for(int i=0;i<MAX_USERS;i++){
		if(users[i].id != 0xFFFFFFFF){
			Serial.print("Message sent to: ");
			Serial.print(users[i].id);
			Serial.print(" text: \"");
			Serial.print(text);
			Serial.println("\"");
			sendMessage(users[i].id,text);
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