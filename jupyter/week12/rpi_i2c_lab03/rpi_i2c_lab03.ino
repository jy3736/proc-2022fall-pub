#include <Wire.h>

#define I2C_ADDR 11

// a template struct for receiving I2C package from RPi
struct i2c_msg {
  byte     cmd;   // don't touch this field!
  // you can add any field as you need below
  char     c;     // 1-byte
  float    f;     // 4-byte
  int      i;     // 2-byte 
} msg;

// for verify incoming I2C package size
unsigned SIZE = sizeof(i2c_msg)+1;

void setup()
{
  Wire.begin(I2C_ADDR);
  Serial.begin(9600); 
  Serial.print("I2C Slave @ ");
  Serial.println(I2C_ADDR);
  Serial.println("-------------------------");
  delay(1000);               
  Wire.onReceive(irsReceive);
}

bool pending = false;

void loop(){
  if (pending) {
    Serial.print("receive("); 
    Serial.print(msg.cmd); Serial.print(", "); 
    Serial.print(msg.c);   Serial.print(", "); 
    Serial.print(msg.f);   Serial.print(", "); 
    Serial.print(msg.i);   Serial.println(")");
    pending = false;
  }
  delay(0);
}

// -----------------------------------------------------
// Event handling when receiving data from I2C channel
// -----------------------------------------------------
void irsReceive(int nb)
{  
    char* p;
    // Drop message when receiving an I2C package with 
    // a mismatched size
    if (nb>SIZE) {
      Serial.println("Error: message size does not match!");
      Serial.println(nb);
      Serial.println(SIZE);
      while (Wire.available())
        Wire.read();
      return;
    }
    p = (char*)&msg;
    *p++ = Wire.read(); // Read 'cmd'
    Wire.read();        // Drop 'size' data(1-byte) added 
                        // by RPi's write_block_data(...)
    // Read and fill in the rest of the message structure
    while (Wire.available()) 
      *p++ = Wire.read();
    pending = true;  
}
