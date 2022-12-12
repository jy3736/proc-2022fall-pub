#include <Wire.h>
# define I2C_ADDR 11
char sbuf[50];
bool pending = false;
byte msg[20];
byte cmd;
unsigned len;

// -----------------------------------------------------
void setup() {
  Wire.begin(I2C_ADDR);
  Serial.begin(9600); 
  sprintf(sbuf,"Arduino I2C Slave(%d)",I2C_ADDR);
  Serial.println(sbuf);
  Serial.println("===================================");
  delay(1000);    
  // register Request/Receive event services           
  Wire.onRequest(irsRequest);
  Wire.onReceive(irsReceive);
}

// -----------------------------------------------------
void loop() {
  if (pending) {
    Serial.print("received : ");
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
void irsRequest() {
  unsigned rn;
  Serial.print("irsRequest send => ");
  switch(cmd) {
    case 11 : len = 3;
              break;
    case 123: len = 5;
              break;
    default: len = 1;
  }
  for(int i=0; i<len; i++) {
    rn = random(1,6);
    Serial.print(rn);
    Serial.print(" ");
    Wire.write(rn);
  }
  Serial.println();
}

// -----------------------------------------------------
void irsReceive(int nb) {  
  Serial.print("irsReceive # of bytes = "); 
  Serial.println(nb);
  if (nb==1) {
    cmd = Wire.read();   
    Serial.print("  cmd = "); 
    Serial.println(cmd);
  } else {
    len = 0;
    while (Wire.available())
      msg[len++] = Wire.read();  
    pending = true;
  }
}
