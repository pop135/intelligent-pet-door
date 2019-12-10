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

/* Secret stuff to allow connections (Router/Telegram bot) */
#include "arduino_secrets.h"

/* Telegram bot library */
#include "CTBot.h"

/*----------DEFINES--------------------------------------------------------------*/

/* Uncoment to allow debug */
//#define DEBUG 1

/* Message max length */
#define MESSAGE_TEXT_LENGTH	( 200 )

/*----------TYPEDEFS------------------------------------------------------------*/

/* To store message info */
typedef struct simpleTBMessage {
  uint32_t id;
  char text[MESSAGE_TEXT_LENGTH];
} simpleTBMessage;

/*----------VARIABLES-----------------------------------------------------------*/

/* Telegram bot object */
CTBot intelligentPetDoorBot;

/* Secrets pulled from arduino_secrets.h */
String ssid  = SECRET_SSID; 
String pass  = SECRET_PASS; 
String token = SECRET_BOT_TOKEN; 

/* To store telegram message data */
TBMessage msg;



/*----------FUNCTIONS-----------------------------------------------------------*/

void setup() {
	/* Initialize serial to allow arduino comunication*/
	Serial.begin(9600);
	Serial.setTimeout(100);
	
	#ifdef DEBUG
		Serial.println("Starting TelegramBot...");
	#endif
	
	/* connect the ESP8266 to the desired access point */
	intelligentPetDoorBot.wifiConnect(ssid, pass);

	/* set the telegram bot token */
	intelligentPetDoorBot.setTelegramToken(token);
	
	/* Debug code */
	#ifdef DEBUG
		if (intelligentPetDoorBot.testConnection()) Serial.println("testConnection OK");
		else Serial.println("testConnection NOK");
	#endif
	
}

void loop() {

	/* Check if there is an incoming message from telegram */
	if (intelligentPetDoorBot.getNewMessage(msg)){
		
		/* New var to store the sending information */
		simpleTBMessage sendingSmsg;
		
		/* Getting the message id */
		sendingSmsg.id = msg.sender.id;
		
		/* Convert String to char array */
		msg.text.toCharArray(sendingSmsg.text,MESSAGE_TEXT_LENGTH);
		
		/* Send the struct through UART */
		Serial.write((uint8_t *) &sendingSmsg, (uint16_t) sizeof(sendingSmsg));
		
		/* Debug code */
		#ifdef DEBUG
			Serial.print("Recieved from TG-> id:");
			Serial.print(sendingSmsg.id);
			Serial.print(" text: ");
			Serial.print(sendingSmsg.text);
			Serial.println("");
		#endif
		
		/* Eco the message to telegram user */
		#ifdef DEBUG
			intelligentPetDoorBot.sendMessage(msg.sender.id, msg.text);
		#endif
	}
  
	/* Check if there is an incoming message from serial. Usually from arduino. */
    if(Serial.available()>0){
		
		/* New struct to store received information */
		simpleTBMessage recievingSmsg;
		
		/* Read the struct from serial */
		Serial.readBytes((uint8_t *) &recievingSmsg, (uint16_t) sizeof(recievingSmsg));
		
		/* Forward message to telegram chat id */
		intelligentPetDoorBot.sendMessage(recievingSmsg.id, recievingSmsg.text);
		
		/* Debug code */
		#ifdef DEBUG
			Serial.print("Recieved from Serial-> id: ");
			Serial.print(recievingSmsg.id);
			Serial.print(" text: ");
			Serial.println(recievingSmsg.text);
		#endif

	}
}

/*----------INTERRUPTS----------------------------------------------------------*/
