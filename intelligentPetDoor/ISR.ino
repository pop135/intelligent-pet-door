extern volatile char inOutFlag;
extern volatile char outInFlag;

void initISR(){
	attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_0), buttonInOutISR, FALLING);
	attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_1), buttonOutInISR, FALLING);
}


void buttonInOutISR(){
	//detachInterrupt(digitalPinToInterrupt(buttonPin1));
	inOutFlag = 1;
}

void buttonOutInISR(){
	//detachInterrupt(digitalPinToInterrupt(buttonPin0));
	outInFlag=1;
}