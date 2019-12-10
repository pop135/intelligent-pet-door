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

/* The EEPROM default arduino library */
#include <EEPROM.h>

/*----------DEFINES--------------------------------------------------------------*/

/* Numer of pets pos */
#define NUMBER_OF_PETS_POS ( 0 )

/* Schedule pos */
#define SCHEDULE_POS ( 2 )

/* The maxium of scheduled commands that can be stored */
#define MAX_SCHEDULE 	( 100 )

/* Schedule pos */
#define USERS_POS ( 203 )

/* The maxium of scheduled commands that can be stored */
#define MAX_USERS 	( 4 )

/*----------TYPEDEFS------------------------------------------------------------*/

/*----------VARIABLES-----------------------------------------------------------*/



/*----------FUNCTIONS-----------------------------------------------------------*/

uint8_t readEEPROM(int p){
	
	return EEPROM.read(p);
	
	
}

void writeEEPROM(int p, uint8_t d){
	
	EEPROM.write(p,d);
	
	
}

/*----------INTERRUPTS----------------------------------------------------------*/

