#include <WiFi.h>
#include <ArduinoJson.h>
#include "password.h"

// WiFi credentials
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;

// Flask server details
const char* server = "192.168.31.54"; // Replace with the Raspberry Pi's IP address
const int port = 5000;

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    // Attempt to connect to WiFi
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("WiFi connected!");

    // Request the next holiday
    WiFiClient client;
    if (client.connect(server, port)) {
        // Send the HTTP GET request
        client.println("GET /next-holiday HTTP/1.1");
        client.println("Host: 192.168.31.54");
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

void loop() {
    
}
