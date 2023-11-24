#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

const int ROWS = 4; //four rows
const int COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {13, 12, 11, 10}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 8, 7, 6}; // connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Set the LCD address (you can find this using the I2C scanner)
int lcdAddress = 0x27;

// Define LCD columns and rows
int lcdColumns = 20;
int lcdRows = 4;

// Create an LCD object
LiquidCrystal_I2C lcd(lcdAddress, lcdColumns, lcdRows);

void setup() {
  //Initialize the keypad
  Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

  // Initialize the LCD
  lcd.init();

  // Turn on the backlight (if available on your LCD module)
  lcd.backlight();

  // Print a welcome message to the LCD
  lcd.setCursor(0, 0); // Set the cursor to the first column and first row
  lcd.print("Welcome to LCD");
  lcd.setCursor(0, 1); // Set the cursor to the first column and second row
  lcd.print("Keypad Locker System");
  lcd.setCursor(0, 2); // Set the cursor to the first column and second row
  lcd.print("(-_-)");
  lcd.setCursor(2, 3); // Set the cursor to the first column and second row
  lcd.print("*** Loading ***");

  // Wait for a few seconds
  delay(3000);

  // Clear the LCD
  lcd.clear();

  Serial.begin(9600);
}

void loop() {
  lcd_show_keypad();
  
  int nextColumn = 0;
  const int maxKeys = 7;
  char passwords[maxKeys];
  int keyIndex = 0;

  // always focus on keypad input
  while (true) {
    char key = keypad.getKey();
    
    if(key) {

      // if user press 'C', then clear the password
      if(key == 'C') {
        lcd.clear();
        nextColumn = 0;
        keyIndex = 0;

        lcd_show_keypad();
      }

      else {
        // display output on LCD
        if (keyIndex < maxKeys - 1) {
          // Store the key in the array
          passwords[keyIndex] = key;
          keyIndex++;

          // Print the current state of the array to the Serial Monitor
          for (int i = 0; i < keyIndex; i++) {
            Serial.print(passwords[i]);
          }

          lcd.setCursor(nextColumn, 2);
          lcd.print("*");
          nextColumn++;
        }
      }
      
      Serial.println();
    }
  }
}

void lcd_show_keypad() {
  lcd.setCursor(0, 0);
  lcd.print("Please enter any");
  lcd.setCursor(0, 1);
  lcd.print("key:");
}
