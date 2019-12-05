/*
 Name:		    echoBot.ino
 Created:	    12/21/2017
 Author:	    Stefano Ledda <shurillu@tiscalinet.it>
 Description: a simple example that check for incoming messages
              and reply the sender with the received message
*/
#include "arduino_secrets.h"
#include "CTBot.h"
CTBot myBot;

String ssid  = SECRET_SSID    ; // REPLACE mySSID WITH YOUR WIFI SSID
String pass  = SECRET_PASS; // REPLACE myPassword YOUR WIFI PASSWORD, IF ANY
String token = SECRET_BOT_TOKEN   ; // REPLACE myToken WITH YOUR TELEGRAM BOT TOKEN

void setup() {
	// initialize the Serial
	Serial.begin(115200);
	Serial.println("Starting TelegramBot...");

	// connect the ESP8266 to the desired access point
	myBot.wifiConnect(ssid, pass);

	// set the telegram bot token
	myBot.setTelegramToken(token);
	
	// check if all things are ok
	if (myBot.testConnection())
		Serial.println("\ntestConnection OK");
	else
		Serial.println("\ntestConnection NOK");
}

void loop() {
	// a variable to store telegram message data
	TBMessage msg;

	// if there is an incoming message...
	if (myBot.getNewMessage(msg)){
	  Serial.print(msg.text);
	}

  if(Serial.available()>0){
    String read="";
    while(Serial.available()){
      read.concat((char)Serial.read());
    }
    myBot.sendMessage(msg.sender.id, read);
  }
 

	 
	// wait 500 milliseconds
	delay(500);
}
