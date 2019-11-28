void free(){
	if(servoState0 == servoOpen){
		//servo is already open
	}
	else{
		//Servo0.attach(servoPin0);
		Servo0.write(servoOpen);
		servoState0 = servoOpen;
	}
	
	if(servoState1 == servoOpen){
		
	}
	else{
		//Servo1.attach(servoPin1);
		Servo1.write(servoOpen);
		servoState1 = servoOpen;
	}
	
	if(servoState2 == servoOpen){
		
	}
	else{
		//Servo2.attach(servoPin2);
		Servo2.write(servoOpen);
		servoState2 = servoOpen;
	}
	
	//wait servos movment
	//noInterrupts();
	delay(2000);
	//interrupts();
	//detach if proceed
	//if(Servo0.attached()){Servo0.detach();}
	//if(Servo1.attached()){Servo1.detach();}
	//if(Servo2.attached()){Servo2.detach();}
}

void in(){
	if(servoState0 == servoOpen){
		//Servo0.attach(servoPin0);
		Servo0.write(servoClose180);
		servoState0 = servoClose180;
	}
	else{
		
	}
	
	if(servoState1 == servoOpen){
		
	}
	else{
		//Servo1.attach(servoPin1);
		Servo1.write(servoOpen);
		servoState1 = servoOpen;
	}
	
	if(servoState2 == servoOpen){
		
	}
	else{
		//Servo2.attach(servoPin2);
		Servo2.write(servoOpen);
		servoState2 = servoOpen;
	}
	
	//wait servos movment
	//noInterrupts();
	delay(2000);
	//interrupts();
	//detach if proceed
	//if(Servo0.attached()){Servo0.detach();}
	//if(Servo1.attached()){Servo1.detach();}
	//if(Servo2.attached()){Servo2.detach();}
}

void out(){
	if(servoState0 == servoOpen){
		//servo is already open
	}
	else{
		//Servo0.attach(servoPin0);
		Servo0.write(servoOpen);
		servoState0 = servoOpen;
	}
	
	if(servoState1 == servoOpen){
		//Servo1.attach(servoPin1);
		Servo1.write(servoClose0);
		servoState1 = servoClose0;
	}
	else{
		
	}
	
	if(servoState2 == servoOpen){
		//Servo2.attach(servoPin2);
		Servo2.write(servoClose180);
		servoState2 = servoClose180;
	}
	else{
		
	}
	
	//wait servos movment
	//noInterrupts();
	delay(2000);
	//interrupts();
	//detach if proceed
	//if(Servo0.attached()){Servo0.detach();}
	//if(Servo1.attached()){Servo1.detach();}
	//if(Servo2.attached()){Servo2.detach();}
}

void close(){
	if(servoState0 == servoOpen){
		//Servo0.attach(servoPin0);
		Servo0.write(servoClose180);
		servoState0 = servoClose180;
	}
	else{
		
	}
	
	if(servoState1 == servoOpen){
		//Servo1.attach(servoPin1);
		Servo1.write(servoClose0);
		servoState1 = servoClose0;
	}
	else{
		
	}
	
	if(servoState2 == servoOpen){
		//Servo2.attach(servoPin2);
		Servo2.write(servoClose180);
		servoState2 = servoClose180;
	}
	else{
		
	}
	
	//wait servos movment
	//noInterrupts();
	delay(2000);
	//interrupts();
	//detach if proceed
	//if(Servo0.attached()){Servo0.detach();}
	//if(Servo1.attached()){Servo1.detach();}
	//if(Servo2.attached()){Servo2.detach();}
}