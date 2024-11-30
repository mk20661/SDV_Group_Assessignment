#include <Adafruit_NeoPixel.h>
#define LED_PIN    1          
#define LED_COUNT  50   
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
void setup() {
  strip.begin();           
  strip.setBrightness(30); 
  strip.show();           
  strip.fill(strip.Color(0, 0, 0)); 
}

void loop() {
 countdown(3); 
//delay(10000);        
}

void countdown(int minutes) {
//  if (minutes < 3) {
//    Serial.println("Error: Countdown requires 3 minutes.");
//    return;
//  }
  int totalSeconds = minutes * 60;
  int phaseTime = 60;            
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
    strip.setPixelColor(i, 0);
    strip.show();
  }
}
