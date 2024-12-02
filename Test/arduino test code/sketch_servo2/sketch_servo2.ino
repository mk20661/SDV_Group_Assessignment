#include <ESP32Servo.h>

Servo servo1; 

int servo1Pin = 2; 

int pos1 = 0;     
int step1 = 1;    

void setup() {
  servo1.attach(servo1Pin); 
  pos1 = 0;                 
  servo1.write(pos1);       
  delay(500);              
}

void loop() {
  pos1 += step1;           
  if (pos1 >= 180 || pos1 <= 0) { 
    step1 = -step1;        
  }

  servo1.write(pos1);      
  delay(10);               
}
