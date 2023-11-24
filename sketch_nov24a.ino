#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address (you can find this using the I2C scanner)
int lcdAddress = 0x27;

// Define LCD columns and rows
int lcdColumns = 20;
int lcdRows = 4;

// Create an LCD object
LiquidCrystal_I2C lcd(lcdAddress, lcdColumns, lcdRows);

void setup() {
  // Initialize the LCD
  lcd.init();

  // Turn on the backlight (if available on your LCD module)
  lcd.backlight();

  // Print a welcome message to the LCD
  lcd.setCursor(0, 0); // Set the cursor to the first column and first row
  lcd.print("Welcome to");
  lcd.setCursor(0, 1); // Set the cursor to the first column and second row
  lcd.print("LCD Keypad");
  lcd.setCursor(0, 2); // Set the cursor to the first column and second row
  lcd.print("Locker System");
  lcd.setCursor(0, 3); // Set the cursor to the first column and second row
  lcd.print("(-_-)");

  // Wait for a few seconds
  // delay(3000);

  // Clear the LCD
  // lcd.clear();
}

void loop() {
  // Your main code can go here
}
