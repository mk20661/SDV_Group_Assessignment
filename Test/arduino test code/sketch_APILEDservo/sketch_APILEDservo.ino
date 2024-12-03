#include <ESP32Servo.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "password.h"

// WiFi credentials
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;

// Flask server details
const char* server = "10.129.119.99"; // Replace with the Raspberry Pi's IP address
const int port = 5000;
WiFiClient client;

// LED and Servo setup
#define LED_PIN 1          
#define LED_COUNT 15   

Servo servo1; 
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int servo1Pin = 2; 
int pos1 = 0;     

// Server data
int update_term_order = 0;
int update_left_weeks = 0;
int update_reaming_holidays = 0;

void setup() {
    Serial.begin(9600);
    WiFi.begin(ssid, password);

    // Attempt to connect to WiFi
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("WiFi connected!");

    // Initialize Servo
    servo1.attach(servo1Pin); 
    servo1.write(0);  // Reset to 0 degrees
    delay(500);

    // Initialize LED strip
    strip.begin();           
    strip.setBrightness(30); 
    strip.show();           
    strip.fill(strip.Color(0, 0, 0)); 

    // Get data from server
}

void loop() {
    getBankHoliday();
    getTermInfo();
    updateDisplay(); // Update LED and Servo based on server data
    delay(10000);    // Update every 10 seconds
}

void updateDisplay() {
    // Map `update_reaming_holidays` to Servo position with specific logic
    int servoPosition;
    if (update_reaming_holidays >= 31) {
        servoPosition = 180; // Cap at 180 degrees for holidays >= 31
    } else {
        servoPosition = (update_reaming_holidays - 1) * 6; // Calculate position
    }

    // Ensure servoPosition is within bounds (0 to 180 degrees)
    servoPosition = constrain(servoPosition, 0, 180);
    
    servo1.write(servoPosition);
    Serial.print("Servo Position: ");
    Serial.println(servoPosition);


    // Control the LED strip based on `update_term_order`
    if (update_term_order == 1) {
        lightPhase(32, 32, 32, update_left_weeks, 1, true); // Gray for term 1
    } else if (update_term_order == 2) {
        lightPhase(155, 5, 80, update_left_weeks, 1, true); // Purple for term 2
    } else {
        lightPhase(0, 91, 155, update_left_weeks, 1, false); // Blue for other terms
    }
    Serial.print("Active LEDs: ");
    Serial.println(update_left_weeks);
}

void lightPhase(int red, int green, int blue, int numLEDs, int timePerLED, bool isYellowSixth) {
    for (int i = 0; i < numLEDs; i++) {
        // If `isYellowSixth` is true and it's the sixth LED, light it yellow
        if (isYellowSixth && i == 5) { 
            strip.setPixelColor(i, strip.Color(155, 155, 0)); // Yellow
        } else {
            strip.setPixelColor(i, strip.Color(red, green, blue)); // Other LEDs in specified color
        }
    }
    strip.show(); // Update the LED strip
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

        StaticJsonDocument<128> doc;
        DeserializationError error = deserializeJson(doc, response);

        if (error) {
            Serial.print("JSON parsing failed: ");
            Serial.println(error.c_str());
            return;
        }

        const int days_until = doc["days_until"];
        update_reaming_holidays = constrain(days_until, 0, 31);

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
        update_left_weeks = constrain(left_weeks, 0, LED_COUNT);
        update_term_order = term;
        Serial.print("Left Weeks: ");
        Serial.println(update_left_weeks);
        Serial.print("Term Order: ");
        Serial.println(update_term_order);
    } else {
        Serial.println("Failed to connect to the server.");
    }
}
