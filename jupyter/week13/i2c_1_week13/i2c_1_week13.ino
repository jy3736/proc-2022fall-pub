#include <Wire.h>

#define I2C_ADDR 11

bool pending = false;
unsigned len;
byte msg[20];

// -----------------------------------------------------
void setup() {
  Wire.begin(I2C_ADDR);
  Serial.begin(9600); 
  Serial.print("Arduino I2C Slave : ");
  Serial.println(I2C_ADDR);
  Serial.println("=============================");
  delay(1000);    
  // register onReceive event services           
  Wire.onReceive(irsReceive);
}

// -----------------------------------------------------
void loop() {
  if (pending) {
    Serial.print("Received data  : ");
    for(int i=0; i<len; i++) {
      Serial.print(msg[i]);
      Serial.print(" ");
    }
    Serial.println();
    pending = false;
  }
  delay(200);
}

// -----------------------------------------------------
void irsReceive(int nb) {  
  Serial.print("Received bytes : "); 
  Serial.println(nb);
  len = 0;
  while (Wire.available())
    msg[len++] = Wire.read();  
  pending = true;
}
