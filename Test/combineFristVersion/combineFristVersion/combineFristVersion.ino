#include <ESP32Servo.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "password.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// WiFi credentials
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;

// Flask server details
const char* server = SECRET_IP_ADRESS; // Replace with the Raspberry Pi's IP address
const int port = 5000;
WiFiClient client;
//OLED setting
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// LED and Servo setup
#define WEEK_LED 1          
#define WEEK_LED_COUNT 15
#define TERM_LED 5  
#define TERM_NUM_LED 8
#define servoPin 2

Servo servo; 
Adafruit_NeoPixel week_strip(WEEK_LED_COUNT, WEEK_LED, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel term_strip(TERM_NUM_LED, TERM_LED, NEO_GRB + NEO_KHZ800);

// Server data
int update_term_order = 0;
int update_left_weeks = 0;
int update_reaming_holidays = 0;

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    // Attempt to connect to WiFi
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("WiFi connected!");
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
      Serial.println(F("OLED fail"));
    while (true);
    }
    display.clearDisplay();
    display.display();

    // Initialize Servo
    servo.attach(servoPin); 
    servo.write(0);  // Reset to 0 degrees
    delay(500);

    // Initialize LED strip
     week_strip.begin();           
     week_strip.setBrightness(30); 
     week_strip.show();           
     week_strip.fill(week_strip.Color(0, 0, 0));

     term_strip.begin();           
     term_strip.setBrightness(30); 
     term_strip.show();           
     term_strip.fill(term_strip.Color(0, 0, 0)); 
     
}

void loop() {
    getBankHoliday();
    getTermInfo();
    updateDisplay(); // Update LED and Servo based on server data
    delay(10000);    // Update every 10 seconds
}

void updateDisplay() {
    // Map `update_reaming_holidays` to Servo position with specific logic
    int servoPosition = 0;
    if (update_reaming_holidays >= 31) {
        servoPosition = 180; // Cap at 180 degrees for holidays >= 31
    } else {
        servoPosition = (update_reaming_holidays - 1) * 6; // Calculate position
    }

    // Ensure servoPosition is within bounds (0 to 180 degrees)
    servoPosition = constrain(servoPosition, 0, 180);
    
    servo.write(servoPosition);
    Serial.print("Servo Position: ");
    Serial.println(servoPosition);


    // Control the LED strip based on `update_term_order`
    if (update_term_order == 1) {
        lightPhase(32, 32, 32, update_left_weeks, 1, true); // Gray for term 1
        term_LED(32, 32, 32,0);
        term_LED(155, 5, 32,2);
        term_LED(0, 91, 155,4);
    } else if (update_term_order == 2) {
        lightPhase(155, 5, 80, update_left_weeks, 1, true); // Purple for term 2
        term_LED(0, 0, 0,0);
    } else {
        lightPhase(0, 91, 155, update_left_weeks, 1, false); // Blue for other terms
        term_LED(0, 0, 0,2);
    }
    Serial.print("Active LEDs: ");
    Serial.println(update_left_weeks);
}

void lightPhase(int red, int green, int blue, int numLEDs, int timePerLED, bool isYellowSixth) {
    for (int i = 0; i < numLEDs; i++) {
        // If `isYellowSixth` is true and it's the sixth LED, light it yellow
        if (isYellowSixth && i == 8) { 
             week_strip.setPixelColor(i,  week_strip.Color(155, 155, 0)); // Yellow
        } else {
             week_strip.setPixelColor(i,  week_strip.Color(red, green, blue)); // Other LEDs in specified color
        }
    }
     week_strip.show(); // Update the LED strip
}

void term_LED(int red, int green, int blue, int order_LED){
  term_strip.setPixelColor(order_LED,red,green,blue);
  term_strip.show();
}

void getBankHoliday() {
    if (client.connect(server, port)) {
        client.println("GET /next-holiday HTTP/1.1");
        client.println("Host: ");
        client.println(server);
        client.println("Connection: close");
        client.println();

        String response = "";
        bool isBody = false;
        while (client.connected() || client.available()) {
            String line = client.readStringUntil('\n');
            if (line == "\r") {
                isBody = true;
                continue;
            }
            if (isBody) {
                response += line;
            }
        }
        client.stop();

        DynamicJsonDocument doc(512);;
        DeserializationError error = deserializeJson(doc, response);

        if (error) {
            Serial.print("JSON parsing failed: ");
            Serial.println(error.c_str());
            return;
        }

        const int days_until = doc["days_until"];
        const char* holiday_name = doc["name"];
        update_reaming_holidays = constrain(days_until, 0, 31);
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);
        display.println(holiday_name);
        display.display();
        Serial.print("Remaining Holidays: ");
        Serial.println(update_reaming_holidays);
    } else {
        Serial.println("Failed to connect to the server.");
    }
}

void getTermInfo() {
    if (client.connect(server, port)) {
        client.println("GET /term-week HTTP/1.1");
        client.println("Host: " + String(server));
        client.println("Connection: close");
        client.println();

        String response = "";
        bool isBody = false;
        while (client.connected() || client.available()) {
            String line = client.readStringUntil('\n');
            if (line == "\r") {
                isBody = true;
                continue;
            }
            if (isBody) {
                response += line;
            }
        }
        client.stop();

        StaticJsonDocument<256> doc;
        DeserializationError error = deserializeJson(doc, response);

        if (error) {
            Serial.print("JSON parsing failed: ");
            Serial.println(error.c_str());
            return;
        }

        const int left_weeks = doc["left_weeks"];
        const int term = doc["term"];
        update_left_weeks = constrain(left_weeks, 0, WEEK_LED_COUNT);
        update_term_order = term;
        Serial.print("Left Weeks: ");
        Serial.println(update_left_weeks);
        Serial.print("Term Order: ");
        Serial.println(update_term_order);
    } else {
        Serial.println("Failed to connect to the server.");
    }
}
