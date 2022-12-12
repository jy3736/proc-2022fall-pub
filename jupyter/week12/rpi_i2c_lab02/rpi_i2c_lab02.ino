#include <Wire.h>

# define I2C_SLAVE_ADDRESS 11

struct i2c_msg {
  byte     cmd;   
  byte     size;
  char     c;   
  float    f;     // 4-byte
  int      i;     // 2-byte 
} msg;


void setup()
{
  Wire.begin(I2C_SLAVE_ADDRESS);
  Serial.begin(9600); 
  Serial.print("Slave @ ");
  Serial.println(I2C_SLAVE_ADDRESS);
  delay(1000);               
  Wire.onRequest(requestEvents);
  Wire.onReceive(receiveEvents);
}

bool pending = false;

void loop(){
  if (pending) {
    Serial.print("cmd  : "); Serial.println(msg.cmd);
    Serial.print("size : "); Serial.println(msg.size);
    Serial.print("c    : "); Serial.println(msg.c);
    Serial.print("f    : "); Serial.println(msg.f);
    Serial.print("i    : "); Serial.println(msg.i);
    pending = false;
  }
  delay(0);
}

int n = 0;

void requestEvents()
{
  Serial.println(F("---> recieved request"));
  Serial.print(F("sending value : "));
  Serial.println(n);
  Wire.write(n);
}

void receiveEvents(int nb)
{  
    char* p;
    Serial.print("# of bytes received : ");
    Serial.println(nb);
    p = (char*)&msg;
    while (Wire.available()) { 
      *p = Wire.read();
      Serial.println((byte)*p);
      p++;
    }
    pending = true;  
}
