
extern volatile char inOutFlag;
extern volatile char outInFlag;


void buttonInOutISR(){
	//detachInterrupt(digitalPinToInterrupt(buttonPin1));
	inOutFlag = 1;
}

void buttonOutInISR(){
	//detachInterrupt(digitalPinToInterrupt(buttonPin0));
	outInFlag=1;
}