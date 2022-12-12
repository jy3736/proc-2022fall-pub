#include <Wire.h>

# define I2C_SLAVE_ADDRESS 11

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

void loop(){}

int n = 0;

void requestEvents()
{
  Serial.println(F("---> recieved request"));
  Serial.print(F("sending value : "));
  Serial.println(n);
  Wire.write(n);
}

void receiveEvents(int numBytes)
{  
    if (Wire.available()) {
      word w = Wire.read();
    }
    while (Wire.available()) { 
      byte c = Wire.read();  
      Serial.println(c); 
    } 
}
