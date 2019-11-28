//in->out button
void buttonInOutISR(){
	if(npetsIn > 0) {
		npetsIn--;
		npetsOut++;
		//buttonState0 = 1;
	}
	Serial.print("Button IN->OUT pressed. Pets IN: ");
	Serial.print(npetsIn);
	Serial.print(". Pets OUT: ");
	Serial.print(npetsOut);
	Serial.println(".");



}

//out->in button
void buttonOutInISR(){
	if(npetsOut > 0) {
		npetsOut--;
		npetsIn++;
		//buttonState1 = 1;
	}
	Serial.print("Button OUT->IN pressed. Pets IN: ");
	Serial.print(npetsIn);
	Serial.print(". Pets OUT: ");
	Serial.print(npetsOut);
	Serial.println(".");

}

/*void timerOneISR(){
	
	timerFlag=true;
  
}
*/