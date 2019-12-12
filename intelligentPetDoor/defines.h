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

/*----------DEFINES--------------------------------------------------------------*/

/*---PCB pin related defines-----------------------------------------------------*/

/* The PCB pins that are attached to sensors */
#define BUTTON_PIN_0 				( 2 )
#define BUTTON_PIN_1 				( 3 )

/* The PCB pins that are remaped RX and TX from software serial lib */
#define SOFTWARE_RX					( 5 )
#define SOFTWARE_TX					( 4 )

/* The PCB pins that are attached to servos  */
#define SERVO_PIN_0					( 9 )
#define SERVO_PIN_1					( 10 )
#define SERVO_PIN_2					( 11 )

/*---Servo related defines-------------------------------------------------------*/

/* Degrees for servo positions */
#define SERVO_OPEN					( 90 )
#define SERVO_CLOSE_180				( 180 )
#define SERVO_CLOSE_0				( 0 )

/*---User input/output related defines-------------------------------------------*/

/* Number of maxium words that an issued command could have */
#define MAX_WORDS 					( 6 )

/* Maxium message length */
#define MESSAGE_TEXT_LENGTH			( 200 )

/*---EEPROM related defines------------------------------------------------------*/

/* Numer of pets pos */
#define NUMBER_OF_PETS_POS 			( 0 )

/* Schedule position */
#define SCHEDULE_POS 				( 2 )

/* Size in bytes that an scheduled command  */
#define SCHEDULE_SIZE 				( 2 )

/* The maxium of scheduled commands that can be stored in a given day */
#define MAX_SCHEDULE_A_DAY 			( 4 )

/* The maxium of scheduled commands that can be stored */
#define MAX_SCHEDULE 				( MAX_SCHEDULE_A_DAY * 7 )

/* Users position */
#define USERS_POS 					( 64 )

/* Size in bytes that an user reg ocuppies from EEPROM */
#define USER_SIZE					( 5 )

/* The maxium of scheduled commands that can be stored */
#define MAX_USERS 					( 4 )

/*---Time related defines-------------------------------------------------------*/

/* One second time in ms */
#define ONE_SECOND_MS				( 1000 )

/* The time that has to be waited between door event and next action in MS */
#define TIME_BETWEN_DE_AND_NA		( 2000 )

/* Time to consider a door event ended (Closing door time debounce) in MS */
#define DOOR_EVENT_END_TIME_MS		( 2000 )

/* Opening time debounce. It is the minium time that the door has to be opened to 
 * consider that a pet passed through it. Times under that limit are considered 
 * not a trespassing pet. 
 */
#define DEBOUNCE_OPENING_TIME_MS 	( 800 )


