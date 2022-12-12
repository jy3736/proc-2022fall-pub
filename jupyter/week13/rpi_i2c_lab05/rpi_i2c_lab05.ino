#include <Wire.h>

#define I2C_ADDR 11

// a template struct for receiving I2C package from RPi
struct i2c_msg {
  byte     cmd;   // don't touch this field!
  // you can add any field as you need below
  char     s[10]; // 10-byte
  float    f;     // 4-byte
  int      i;     // 2-byte 
} msg;

// for verify incoming I2C package size
unsigned SIZE = sizeof(i2c_msg);

void setup() {
  Wire.begin(I2C_ADDR);
  Serial.begin(9600); 
  Serial.print("Slave @ ");
  Serial.println(I2C_ADDR);
  Serial.println("==========================");
  delay(1000);               
  Wire.onRequest(irsRequest);
  Wire.onReceive(irsReceive);
}

bool pending = false;

void loop(){
  if (pending) {
    Serial.print("receive("); 
    Serial.print(msg.cmd); Serial.print(", "); 
    Serial.print(msg.s);   Serial.print(", "); 
    Serial.print(msg.f);   Serial.print(", "); 
    Serial.print(msg.i);   Serial.println(")");
    pending = false;
  }
  delay(200);
}

void irsRequest()
{
  #ifdef __DEBUG__
  Serial.println("<irsRequest>");
  #endif
  Serial.print("Receive REQ => ");
  Serial.println("Echo received struct!");
  msg.cmd = 66;
  Wire.write((char*)&msg,sizeof(msg));
}

// -----------------------------------------------------
// Event handling when receiving data from I2C channel
// -----------------------------------------------------
void irsReceive(int nb)
{  
  #ifdef __DEBUG__
  Serial.print("<irsReceive> : ");
  Serial.println(nb);
  #endif
  char* p;
  // Drop message when receiving an I2C package with 
  // a mismatched size
  if (nb!=SIZE) {
    if (Wire.read()==0) return; // RPi's read_block_data
    Serial.println("Error: message size does not match!");
    Serial.println(nb);
    Serial.println(SIZE);
    while (Wire.available()) Wire.read();
    return;
  }
  p = (char*)&msg;
  while (Wire.available()) 
    *p++ = Wire.read();
  pending = true;  
}
