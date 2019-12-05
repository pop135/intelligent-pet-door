#include <SoftwareSerial.h>

SoftwareSerial ESP8266Serial(5, 4); //RX, TX

void setup() {

Serial.begin(9600);
ESP8266Serial.begin(115200);

Serial.println("Program started.");

}

void loop() {

  if(ESP8266Serial.available()){

    Serial.println(ESP8266Serial.readString());
    
  }

  if (Serial.available()>0){
    String a = Serial.readString();
    a.concat("😻");
    ESP8266Serial.print(a);
    Serial.println(a);
    }

}
