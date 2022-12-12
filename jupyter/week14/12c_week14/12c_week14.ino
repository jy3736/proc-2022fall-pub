#include <Wire.h>
#define I2C_ADDR 11

byte buf[20];
byte cmd = 0;

// -----------------------------------------------------
void setup() {
  Wire.begin(I2C_ADDR);
  Serial.begin(9600); 
  Serial.print("I2C Slave(");
  Serial.print(I2C_ADDR);
  Serial.println(") is ready ...");
  Serial.println("=============================");
  delay(1000);    
  // register onReceive event services           
  Wire.onReceive(irsReceive);
  Wire.onRequest(irsRequest);
}

// -----------------------------------------------------
void dispmsg(int n) {
  for(int i=0; i<n; i++) {
    Serial.print(buf[i]);
    Serial.print(" ");
  }
  Serial.println();
}

// -----------------------------------------------------
void loop() {
  delay(100);
}

// -----------------------------------------------------
void irsReceive(int nb) {
  int i;
  for(i=0; Wire.available(); i++)
    buf[i] = Wire.read();
  cmd = buf[0];  
  Serial.print("Received data  : ");
  dispmsg(i);
}

// -----------------------------------------------------
void randgen(int n) {
  for(int i=0; i<n; i++) {
    buf[i] = random(1,100);
    Wire.write(buf[i]);
  }
  Serial.print("Send bytes : ");
  dispmsg(n);
}

// -----------------------------------------------------
void irsRequest() {
  switch(cmd) {
    case 77 : randgen(3); break;
    case 123: randgen(5); break;
    default : if (cmd>0 && cmd<=50)
                randgen(cmd);
              else
                randgen(1);
  }
  cmd = 0;
}
