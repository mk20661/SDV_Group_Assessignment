#include <Servo.h>

Servo servo1; 
Servo servo2;

int servo1Pin = 2;
int servo2Pin = 3;

int pos1 = 0;
int pos2 = 0;

int step1 = 1;
int step2 = 1;

void setup() {
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo1.write(pos1);
  servo2.write(pos2);
  delay(500);
}

void loop() {
  pos1 += step1;
  if (pos1 >= 180 || pos1 <= 0) {
    step1 = -step1;
  }

  pos2 += step2;
  if (pos2 >= 180 || pos2 <= 0) {
    step2 = -step2;
  }

  servo1.write(pos1);
  servo2.write(pos2);

  delay(10); 
}
