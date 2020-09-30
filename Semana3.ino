// Percentual de envio de sinal em 0.07% - Este percentual foi obtido experimentalmente para evitar colisão e manter envios em tempos perceptiveis

#include <SoftwareSerial.h>
int incomingByte = 0; // for incoming serial data
int n = 0;
int te = 7;   // Transmitter enable
int re = 6;   // Receiver enable
int tx = 3;   // Serial transmitter
int rx = 4;   // Serial recever
long randNumber;

SoftwareSerial mySerial(rx, tx); //RX,TX para comunicacao serial

void setup() {
  Serial.begin(900);    // Starter serial
  mySerial.begin(900);    // Starter mySerial
  pinMode(te, OUTPUT);
  pinMode(re, OUTPUT);
  pinMode(rx, INPUT);
  digitalWrite(re, HIGH);
  delay(503);  
}

void loop() {
  // Reading
  if(mySerial.available()>0){
    incomingByte = mySerial.read(); // Reading
    if(incomingByte != 0)     // 
      Serial.println(incomingByte); // Print signal sending
  }
  
  // Sending
  randNumber = random(100000);
  
  if(randNumber > 99930){
    digitalWrite(re, LOW);    // Low receiver signal
    delay(50);
    digitalWrite(te, HIGH);   // High Transmiter signal
    mySerial.write('O');    // Sending 79
    digitalWrite(te, LOW);    // Low Transmiter signal
    delay(50);
    digitalWrite(re, HIGH);   // High receiver signal
  }
}
