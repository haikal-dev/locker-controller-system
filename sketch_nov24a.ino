#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

const char master_password[] = "134679";
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

// buzzer
const int buzzerPin = 4;

// Servo
Servo servo;

void setup() {
  // Initialize buzzer
  pinMode(buzzerPin, OUTPUT);

  //Initialize the keypad
  Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

  // Initialize the LCD
  lcd.init();

  // Turn on the backlight (if available on your LCD module)
  lcd.backlight();

  // Print a welcome message to the LCD
  lcd.setCursor(0, 0); // Set the cursor to the first column and first row
  lcd.print("Welcome to Locker");
  lcd.setCursor(0, 1); // Set the cursor to the first column and second row
  lcd.print("Controlled System");
  lcd.setCursor(0, 2); // Set the cursor to the first column and second row
  lcd.print("(-_-)");
  lcd.setCursor(2, 3); // Set the cursor to the first column and second row
  lcd.print("*** Loading ***");

  // Wait for a few seconds
  delay(3000);

  // Clear the LCD
  lcd.clear();

  // Initialize servo
  servo.attach(14);
  servo.write(0);

  Serial.begin(9600);
}

void loop() {
  lcd_show_keypad();
  
  int nextColumn = 7;
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
        nextColumn = 7;
        keyIndex = 0;

        lcd_show_keypad();
      }

      else if(key == '#'){
        if (keyIndex == sizeof(master_password) - 1) {
          if (strncmp(passwords, master_password, keyIndex) == 0) {
            // Admin password entered successfully
            Serial.println("Admin access granted!");
            master();
          } else {
            // Incorrect password
            sound_error();
          }

          // Clear the enteredKeys array after processing
          keyIndex = 0;
        }
        // sound_error();
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
  lcd.setCursor(2, 0);
  lcd.print("Master Password:");
}

void sound_error(){
  digitalWrite(buzzerPin, HIGH);
  delay(100);
  digitalWrite(buzzerPin, LOW);
  delay(100);
  digitalWrite(buzzerPin, HIGH);
  delay(100);
  digitalWrite(buzzerPin, LOW);
}

void master(){
  Serial.println("Entering master mode...");
  lcd.clear();

  int lockerKeyIndex = 0;
  char lockerNumber[lockerKeyIndex];

  while(true){
    char key = keypad.getKey();

    lcd.setCursor(0, 0);
    lcd.print("Open locker?");
    lcd.setCursor(0, 1);
    lcd.print("[1 - 10]");

    if(key){
      if(key == 'D'){
        lcd.clear();
        loop();
      }

      else if(key == 'A') {
        close_locker();
      }

      else {
        if(key == '#'){
          lockerNumber[lockerKeyIndex] = '\0';  // Null-terminate the string
          int enteredNumber = atoi(lockerNumber);

          if (enteredNumber >= 1 && enteredNumber <= 10) {
            Serial.print("You pressed: ");
            Serial.println(enteredNumber);
            // Perform actions for valid number

            open_locker(enteredNumber);

          } else {
            sound_error();
          }

          // Reset for the next input
          lockerKeyIndex = 0;
          memset(lockerNumber, 0, sizeof(lockerNumber));
        }

        else {
          lockerNumber[lockerKeyIndex] = key;
          lockerKeyIndex++;
          lcd.setCursor(lockerKeyIndex - 1, 1);
          lcd.print(key);
        }
      }
    }
  }
}

void open_locker(int lockerNumber){
  if(lockerNumber == 1){
    servo.write(90);
    delay(1000);
  }
}

void close_locker(){
  int lockerKeyIndex = 0;
  char lockerNumber[lockerKeyIndex];

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Close locker?");
  lcd.setCursor(0, 1);
  lcd.print("[1 - 10]");

  while(true) {
    char key = keypad.getKey();

    if(key) {
      if(key == '#') {
        lockerNumber[lockerKeyIndex] = '\0';  // Null-terminate the string
        int enteredNumber = atoi(lockerNumber);

        if (enteredNumber >= 1 && enteredNumber <= 10) {
          Serial.print("You pressed: ");
          Serial.println(enteredNumber);
          // Perform actions for valid number

          close_locker_servo(enteredNumber);

        } else {
          sound_error();
        }

        // Reset for the next input
        lockerKeyIndex = 0;
        memset(lockerNumber, 0, sizeof(lockerNumber));
      }

      else if(key == 'D') {
        master();
      }

      else {
        lockerNumber[lockerKeyIndex] = key;
        lockerKeyIndex++;
        lcd.setCursor(lockerKeyIndex - 1, 1);
        lcd.print(key);
      }
    }
  }
}

void close_locker_servo(int lockerNumber) {
  servo.write(0);
  delay(1000);
}
