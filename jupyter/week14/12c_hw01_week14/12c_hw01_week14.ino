#include <Wire.h>

#define I2C_ADDR 11

unsigned len;
byte msg[20];
byte cmd = 0;
byte result = 0;

// -----------------------------------------------------
void setup() {
  Wire.begin(I2C_ADDR);
  Serial.begin(9600); 
  Serial.print("I2C Slave for Week14 Homework 01: ");
  Serial.println(I2C_ADDR);
  Serial.println("======================================");
  delay(1000);    
  // register onReceive event services           
  Wire.onReceive(irsReceive);
  Wire.onRequest(irsRequest);
}

void disp_msg() {
    Serial.print("Received data  : ");
    for(int i=0; i<len; i++) {
      Serial.print(msg[i]);
      Serial.print(" ");
    }
    Serial.println();  
    cmd = 0;
}

void fun_sum() {
  result = 0;
  for(int i=1; i<len; i++)
    result+=msg[i];  
  cmd = 255;
}

void fun_min() {
  // add your code here
  cmd = 255;
}

void fun_max() {
  // add your code here
  cmd = 255;
}

// -----------------------------------------------------
void loop() {
  switch(cmd) {
    case 0   : break;
    case 1   : fun_sum();  break;
    case 2   : fun_min();  break;
    case 3   : fun_max();  break;
    case 255 :
    default  : disp_msg();
  }
  delay(200);
}

// -----------------------------------------------------
void irsReceive(int nb) {
  len = 0;
  while (Wire.available())
    msg[len++] = Wire.read();
  cmd = msg[0];  
}

// -----------------------------------------------------
void irsRequest() {
  Wire.write(result);
}
