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

/* The PCB pins that are atached to sensors */
#define BUTTON_PIN_0 	( 2 )
#define BUTTON_PIN_1 	( 3 )

/*----------TYPEDEFS------------------------------------------------------------*/

/*----------VARIABLES-----------------------------------------------------------*/

/* Flags that allow checkButtonISR know if some sensor has detected movement */
extern volatile char inOutFlag;
extern volatile char outInFlag;

/*----------FUNCTIONS-----------------------------------------------------------*/

void initISR(){
	
	/* Attach given interrupts to given PCB pins */
	attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_0), buttonInOutISR, FALLING);
	attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_1), buttonOutInISR, FALLING);
	
}

/*----------INTERRUPTS----------------------------------------------------------*/

/* IN->OUT sensor ISR */
void buttonInOutISR(){
	
	/* Just set the flag to allow further treatment */
	inOutFlag = 1;
	
}

/* OUT->IN sensor ISR */
void buttonOutInISR(){
	
	/* Just set the flag to allow further treatment */
	outInFlag=1;
	
}