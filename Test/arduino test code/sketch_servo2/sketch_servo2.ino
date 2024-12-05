#include <ESP32Servo.h>

Servo servo1; 

int servo1Pin = 2; // MKR WiFi 1010的引脚2

int pos1 = 0;     
int step1 = 1;    

void setup() {
  servo1.attach(servo1Pin); // 附加舵机到引脚
  pos1 = 0;                 
  servo1.write(pos1);        // 将舵机移动到初始位置
  delay(500);               
}

void loop() {
  servo1.write(pos1);        // 让舵机保持在当前位置（实际可修改逻辑）
  delay(10);                
}
