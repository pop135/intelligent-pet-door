void free(){
	if(servoState0 == servoOpen){
		//servo is already open
	}
	else{
		Servo0.attach(servoPin0);
		Servo0.write(servoOpen);
		servoState0 = servoOpen;
	}
	if(servoState1 == servoOpen){
		
	}
	else{
		Servo1.attach(servoPin1);
		Servo1.write(servoOpen);
		servoState1 = servoOpen;
	}
	//wait servos movment
	delay(2000);
	//detach if proceed
	if(Servo0.attached()){Servo0.detach();}
	if(Servo1.attached()){Servo1.detach();}
}

void in(){
	if(servoState0 == servoOpen){
		//servo is already open
	}
	else{
		Servo0.attach(servoPin0);
		Servo0.write(servoOpen);
		servoState0 = servoOpen;
	}
	if(servoState1 == servoOpen){
		Servo1.attach(servoPin1);
		Servo1.write(servoClose0);
		servoState1 = servoClose0;
	}
	else{
		
	}
	//wait servos movment
	delay(2000);
	//detach if proceed
	if(Servo0.attached()){Servo0.detach();}
	if(Servo1.attached()){Servo1.detach();}
}

void out(){
	if(servoState0 == servoOpen){
		Servo0.attach(servoPin0);
		Servo0.write(servoClose180);
		servoState0 = servoClose180;
	}
	else{
		
	}
	if(servoState1 == servoOpen){
		
	}
	else{
		Servo1.attach(servoPin1);
		Servo1.write(servoOpen);
		servoState1 = servoOpen;
	}
	//wait servos movment
	delay(2000);
	//detach if proceed
	if(Servo0.attached()){Servo0.detach();}
	if(Servo1.attached()){Servo1.detach();}
}

void close(){
	if(servoState0 == servoOpen){
		Servo0.attach(servoPin0);
		Servo0.write(servoClose180);
		servoState0 = servoClose180;
	}
	else{
		
	}
	if(servoState1 == servoOpen){
		Servo1.attach(servoPin1);
		Servo1.write(servoClose0);
		servoState1 = servoClose0;
	}
	else{
		
	}
	//wait servos movment
	delay(2000);
	//detach if proceed
	if(Servo0.attached()){Servo0.detach();}
	if(Servo1.attached()){Servo1.detach();}
}