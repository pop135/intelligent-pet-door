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

/*----------TYPEDEFS------------------------------------------------------------*/

/* Data type to store a command. */
typedef struct commandIssued {
	
	/* 0 = /now
	 * 1 = /program
	 * 2 = /where
	 */
	uint8_t commandName;
	
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	
	/* 0 = open
	 * 1 = in
	 * 2 = out
	 * 3 = close 
	 */
	uint8_t movement; 
	
	/* 0 = schedule command
 	 * 1 = delete scheduled command
 	 * 2 = show scheduled commands
	 * 3 = delete all scheduled commands 
	 */
	uint8_t modifierFlag;
	
} commandIssued; 

typedef struct EEPROMStoredCommand {
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t movement;
	
} EEPROMStoredCommand;

/* Data type to store a next action command */
typedef struct nextAction {
	
	/* 0 = open
	 * 1 = in
	 * 2 = out
	 * 3 = close 
	 */
	uint8_t movement;
	
	uint8_t npetsIn;
	uint8_t npetsOut;
	
} nextAction;

/* To store messages sent to ESP8266 */
typedef struct simpleTBMessage {
	uint32_t id;
	char text[MESSAGE_TEXT_LENGTH];
} simpleTBMessage;

/* To store user related info */
typedef struct User {
	uint32_t id;
	uint8_t errorcount;
	uint8_t notificationsEnabled;
} User;
