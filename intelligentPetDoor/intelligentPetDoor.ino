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

/* To allow debug */
#define DEBUG 1

/*----------TYPEDEFS------------------------------------------------------------*/

/*----------VARIABLES-----------------------------------------------------------*/

int npets = 1; //hardcoded!!!

/*----------FUNCTIONS-----------------------------------------------------------*/

/* Common setup function in all arduino projects. Initialize all things here. */
void setup() {
	
	/* Initialize servos */
	initServo();

	/* Initialitze sensors interrupts */
	initISR();

	/* Initialize Serial interface */
	initSerial();

	/* Initialize EEPROM */
	initEEPROM();

	/* Initialize Real Time Clock */
	initRTC();
	
	/* To set the date & time of DS3231 module */
	//adjustRTC();
	
	/* Initialize some time related variables to allow checkSchedule() function to run properly */
	initCheckSchedule();

	/* Initialize wireless module */
	
	
	
	/* Debug code */
	/*#ifdef DEBUG
		Serial.print(F("buttonPin0 (IN->OUT): "));
		Serial.print(digitalRead(BUTTON_PIN_0));
		Serial.print(F(" buttonPin1 (OUT->IN): "));
		Serial.println(digitalRead(BUTTON_PIN_1));
	#endif*/

}

/* Common loop function in all arduino projects. Main program goes here and iterates forever. Super-loop. */
void loop() {
	
	/* Checks if user has issued a command */
	checkCommand();
	
	/* Checks if programmed next action has to be done */
	checkNextAction();
	
	/* Checks if something programmed has to be done */
	checkSchedule();
	
	/* Checks if sensors have detected some movement */
	checkButtonsISR();
  
}

/*----------INTERRUPTS----------------------------------------------------------*/
