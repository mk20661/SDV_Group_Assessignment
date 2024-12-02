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

    getBankHoldiay();
    getTermInfo();

}

void loop() {
    
}

void getBankHoldiay(){
  
    // Request the next holiday
    if (client.connect(server, port)) {
        // Send the HTTP GET request
        client.println("GET /next-holiday HTTP/1.1");
        client.println("Host: ");
        client.println(server);
        client.println("Connection: close");
        client.println();

        String response = "";
        bool isBody = false; // Flag to indicate when the HTTP body starts
        while (client.connected() || client.available()) {
            String line = client.readStringUntil('\n');
            if (line == "\r") {
                // HTTP headers end, body starts next
                isBody = true;
                continue;
            }
            if (isBody) {
                response += line; // Append only the body content
            }
        }
        client.stop();

        Serial.println("Extracted JSON data:");
        Serial.println(response);

        // Parse the JSON response
        StaticJsonDocument<128> doc;
        DeserializationError error = deserializeJson(doc, response);

        if (error) {
            Serial.print("JSON parsing failed: ");
            Serial.println(error.c_str());
            return;
        }

        // Print holiday details
        const char* date = doc["date"];
        const char* name = doc["name"];
        const int days_until = doc["days_until"];
        update_reaming_holidays = days_until;

        Serial.println("Next holiday:");
        Serial.print("Name: ");
        Serial.println(name);
        Serial.print("Date: ");
        Serial.println(date);
        Serial.print("Days remaining: ");
        Serial.println(days_until);
    } else {
        Serial.println("Failed to connect to the server.");
    }
}


void getTermInfo(){
  // Request the term week data
  if (client.connect(server, port)) {
      // Send the HTTP GET request
      client.println("GET /term-week HTTP/1.1");
      client.println("Host: " + String(server));
      client.println("Connection: close");
      client.println();

      String response = "";
      bool isBody = false; // Flag to indicate when the HTTP body starts
      while (client.connected() || client.available()) {
          String line = client.readStringUntil('\n');
          if (line == "\r") {
              // HTTP headers end, body starts next
              isBody = true;
              continue;
          }
          if (isBody) {
              response += line; // Append only the body content
          }
      }
      client.stop();

      Serial.println("Extracted JSON data:");
      Serial.println(response);

      // Parse the JSON response
      StaticJsonDocument<256> doc; // Adjust size based on response
      DeserializationError error = deserializeJson(doc, response);

      if (error) {
          Serial.print("JSON parsing failed: ");
          Serial.println(error.c_str());
          return;
      }

      // Extract term week details
      const char* term_start = doc["term_start"];
      const char* current_date = doc["current_date"];
      const int week_number = doc["week_number"];
      const int term = doc["term"];
      const int left_weeks = doc["left_weeks"];
      update_left_weeks = left_weeks;

      // Print term week details
      Serial.println("Current Term Week Data:");
      Serial.print("Term Start: ");
      Serial.println(term_start);
      Serial.print("Current Date: ");
      Serial.println(current_date);
      Serial.print("Week Number: ");
      Serial.println(week_number);
      Serial.print("Term: ");
      Serial.println(term);
      Serial.print("left weeks: ");
      Serial.println(left_weeks);
  } else {
      Serial.println("Failed to connect to the server.");
  }
}
