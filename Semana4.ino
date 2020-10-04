#include <SoftwareSerial.h>

int incomingByte = 0; // for incoming serial data
int te = 7;		// Transmitter enable
int re = 6; 	// Receiver enable
int tx = 3;		// Serial transmitter
int rx = 4;		// Serial recever
bool check = true;			// Input text control
bool thisArduino = false;	// Message control
int arduino = 3;			// Arduino number
int byteCount = 0;

SoftwareSerial mySerial(rx, tx); //RX,TX para comunicacao serial

void setup() {
  Serial.begin(900); 		// Starter serial
  mySerial.begin(900);		// Starter mySerial
  pinMode(te, OUTPUT);
  pinMode(re, OUTPUT);
  pinMode(rx, INPUT);
  digitalWrite(re, HIGH);
}

void loop() {
  // Reading
  if(mySerial.available()>0){
    incomingByte = mySerial.read();	// Reading
    if(incomingByte != 0){
      if(incomingByte == arduino+48 || incomingByte == 48){
        thisArduino = true;
      }else if(check && thisArduino){
      	Serial.print("arduino ");
        Serial.print(char(incomingByte));
        Serial.print(": ");
      	check = false;
        thisArduino = true;
	  }else if(incomingByte == 10 && thisArduino){
        Serial.print(char(incomingByte));
        check = true;
      	thisArduino = false;
      }else if(thisArduino){
      	Serial.print(char(incomingByte));	// Print signal sending
      }
    }
  }
  
  // Sending  
  if(Serial.available()>0){
    Serial.print("Me: ");
    while(Serial.available()>0){
      incomingByte = Serial.read();
      Serial.print(char(incomingByte));
      digitalWrite(re, LOW);		// Low receiver signal
      delay(50);
      digitalWrite(te, HIGH);		// High Transmiter signal
      mySerial.write(incomingByte);
      digitalWrite(te, LOW);		// Low Transmiter signal
      delay(50);
      digitalWrite(re, HIGH);		// High receiver signal
      byteCount = byteCount + 1;
      if(byteCount == 1){
      	digitalWrite(re, LOW);		// Low receiver signal
        delay(50);
        digitalWrite(te, HIGH);		// High Transmiter signal
        mySerial.write(arduino+48);
        digitalWrite(te, LOW);		// Low Transmiter signal
        delay(50);
        digitalWrite(re, HIGH);		// High receiver signal
      }
    }
    digitalWrite(re, LOW);		// Low receiver signal
    delay(50);
    digitalWrite(te, HIGH);		// High Transmiter signal
    mySerial.write(10);			// Send line feed (pular linha)
    Serial.println();
    digitalWrite(te, LOW);		// Low Transmiter signal
    delay(50);
    digitalWrite(re, HIGH);		// High receiver signal
    byteCount = 0;
  }
}
