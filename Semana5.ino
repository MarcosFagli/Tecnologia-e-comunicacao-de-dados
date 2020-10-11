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

int sumToChecksum = 0;
bool lastByte = false;
String text = String("");


SoftwareSerial mySerial(rx, tx); //RX,TX para comunicacao serial

int checkSumCalc(int checksumValue){
  return checksumValue%256;
}

void sending(int incomingByte,bool print){
  digitalWrite(re, LOW);		// Low receiver signal
  delay(50);
  digitalWrite(te, HIGH);		// High Transmiter signal
  mySerial.write(incomingByte);
  digitalWrite(te, LOW);		// Low Transmiter signal
  delay(50);
  digitalWrite(re, HIGH);		// High receiver signal
}

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
      if(!lastByte && (incomingByte == arduino+48 || incomingByte == 48)){
        thisArduino = true;
		sumToChecksum = sumToChecksum + incomingByte;
      }else if(!lastByte && check && thisArduino){
        text.concat("arduino ");
        text.concat(char(incomingByte));
        text.concat(": ");
        sumToChecksum = sumToChecksum + incomingByte;
        check = false;
        thisArduino = true;
	  }else if(!lastByte && incomingByte == 10 && thisArduino){
        sumToChecksum = sumToChecksum + incomingByte;
        check = true;
        lastByte = true;
      }else if(!lastByte && thisArduino){
        text.concat(char(incomingByte));
        sumToChecksum = sumToChecksum + incomingByte;
      }else if(lastByte && thisArduino){
        if(incomingByte == checkSumCalc(sumToChecksum))
          Serial.println(text);
        else
          Serial.println("Checksum error");
      	thisArduino = false;
        lastByte = false;
        sumToChecksum = 0;
        text = "";
      }
    }
  }
  
  // Sending  
  if(Serial.available()>0){
    Serial.print("Me: ");
    while(Serial.available()>0){
      incomingByte = Serial.read();
      if(byteCount != 0)
      	Serial.print(char(incomingByte));
      sending(incomingByte, true);
      byteCount = byteCount + 1;
      sumToChecksum = sumToChecksum + incomingByte;
      if(byteCount == 1){
        sending(arduino+48, false);
        sumToChecksum = sumToChecksum + (arduino+48);
      }
    }
    sending(10, false);		// Send line feed (pular linha)
    sumToChecksum = sumToChecksum + 10;
    sending(checkSumCalc(sumToChecksum), false);
    Serial.println();
	sumToChecksum = 0;
    byteCount = 0;
  }
}
