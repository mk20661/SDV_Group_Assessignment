#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Rotary encoder pins
const int A_pin = 2;
const int B_pin = 3;
const int Button_pin = 4;

int menuIndex = 0;                // Current index of the menu
int lastA = LOW;                  // Last state of the A pin
int menuItems = 3;                // Number of menu items
const char* menu[3] = {"USD", "EUR", "CNY"}; // Menu options for currencies

unsigned long lastDebounceTime = 0;          // Debounce timer for rotation
const unsigned long debounceDelay = 10;      // Debounce delay for rotation
unsigned long buttonLastDebounceTime = 0;    // Debounce timer for button
const unsigned long buttonDebounceDelay = 50; // Debounce delay for button
int lastButtonState = HIGH;                  // Last button state

void setup() {
  // Initialize OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("OLED init failed"));
    while(true);
  }
  display.clearDisplay();
  display.display();

  // Set up pin modes for rotary encoder
  pinMode(A_pin, INPUT);
  pinMode(B_pin, INPUT);
  pinMode(Button_pin, INPUT_PULLUP);
  Serial.begin(9600);

  displayMenu();
}

void loop() {
  checkRotation();  // Check if the rotary encoder was rotated
  checkButton();    // Check if the button was pressed
}

// Check rotary encoder rotation
void checkRotation() {
  int currentA = digitalRead(A_pin);
  if (currentA != lastA && (millis() - lastDebounceTime) > debounceDelay) {
    lastDebounceTime = millis();
    int currentB = digitalRead(B_pin);
    // Update menu index based on rotation direction
    menuIndex += (currentA == HIGH) ? (currentB == LOW ? 1 : -1) : 0;
    if (menuIndex < 0) menuIndex = menuItems - 1; // Loop back to last item
    if (menuIndex >= menuItems) menuIndex = 0;    // Loop back to first item
    displayMenu();  // Update the displayed menu
  }
  lastA = currentA;
}

// Check if the button is pressed
void checkButton() {
  int buttonState = digitalRead(Button_pin);
  if (buttonState != lastButtonState && (millis() - buttonLastDebounceTime) > buttonDebounceDelay) {
    buttonLastDebounceTime = millis();
    if (buttonState == LOW) {
      Serial.print("Selected: ");
      Serial.println(menu[menuIndex]);
      displaySelection(menu[menuIndex]);
      // Stay on the selected menu item after selection
    }
  }
  lastButtonState = buttonState;
}

// Display menu items on the OLED screen
void displayMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Select Currency");
  // Print each menu item with a cursor ">" next to the selected item
  for (int i = 0; i < menuItems; i++) {
    display.print(i == menuIndex ? "> " : "  ");
    display.println(menu[i]);
  }
  display.display();
}

// Display the selected menu item
void displaySelection(const char* selection) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Selected: ");
  display.println(selection);
  display.display();
}
