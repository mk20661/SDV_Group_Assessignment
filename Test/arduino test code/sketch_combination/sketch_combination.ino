#include <ESP32Servo.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 1          
#define LED_COUNT 15   

Servo servo1; 
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int servo1Pin = 2; 
int pos1 = 0;     
int step1 = 1;    

void setup() {
  
  servo1.attach(servo1Pin); 
  pos1 = 0;                 
  servo1.write(pos1);       
  delay(500);               


  strip.begin();           
  strip.setBrightness(30); 
  strip.show();           
  strip.fill(strip.Color(0, 0, 0)); 
}

void loop() {
  countdown(3);  // 启动倒计时和灯光控制
}

void countdown(int minutes) {
  int totalSeconds = minutes * 60;
  int activeLEDs = 11;           
  int timePerLED = 1;           
  lightPhase(32, 32, 32, activeLEDs, timePerLED, true);
  lightPhase(155, 5, 80, activeLEDs, timePerLED, true);
  lightPhase(0, 91, 155, activeLEDs, timePerLED, false);
  strip.clear();
  strip.show();
}

void lightPhase(int red, int green, int blue, int numLEDs, int timePerLED, bool isYellowSixth) {
  for (int i = 0; i < numLEDs; i++) {
    if (isYellowSixth && i == 5) { 
      strip.setPixelColor(i, strip.Color(155, 155, 0));
    } else {
      strip.setPixelColor(i, strip.Color(red, green, blue)); 
    }
  }
  strip.show();
  
  for (int i = 0; i < numLEDs; i++) {
    delay(timePerLED * 1000); 

    // 根据当前 LED 的序号计算舵机角度
    pos1 = map(i, 0, numLEDs - 1, 0, 180); 
    servo1.write(pos1);                    
    delay(10);                             

    strip.setPixelColor(i, 0);  
    strip.show();
  }
}
