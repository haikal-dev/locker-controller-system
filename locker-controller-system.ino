// Library untuk LCD-I2C, Keypad dan Servo
// Wajib import library ini
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

// Master Password
// Boleh adjust master password di sini
const char master_password[] = "134679";

// Keypad
// Untuk setting Rows dan Columns Keypad
const int ROWS = 4; //four rows
const int COLS = 4; //four columns

// Kedudukan nombor dan huruf dalam
// keypad - Matrix 2 dimensi
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Keypad pin ke arduino
// R1, R2, R3, R4, C1, C2, C3, C4
// Lihat gambar rajah Keypad Pinout 4x4
byte rowPins[ROWS] = {13, 12, 11, 10}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 8, 7, 6}; // connect to the column pinouts of the keypad

// Declare objek 'keypad' ke arah Library Keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Set alamat LCD (Boleh guna code I2C Scanner untuk mengimbas alamat LCD)
int lcdAddress = 0x27;

// Menentukan LCD Rows dan Columns
int lcdColumns = 20;
int lcdRows = 4;

// Declare objek LCD
LiquidCrystal_I2C lcd(lcdAddress, lcdColumns, lcdRows);

// buzzer pin -> Arduino
const int buzzerPin = 4;

// LED Pin -> Arduino
const int LED1_Pin = 5;
const int LED2_Pin = 3;
const int LED3_Pin = A2;

// Servo Objek
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

void setup() {
  // Beritahu arduino yang kita set
  // Pin untuk buzzer sebagai output
  pinMode(buzzerPin, OUTPUT);

  // Beritahu arduino yang kita set
  // Pin untuk LED sebagai output
  pinMode(LED1_Pin, OUTPUT);
  pinMode(LED2_Pin, OUTPUT);
  pinMode(LED3_Pin, OUTPUT);

  // Declare objek 'keypad' ke arah Library Keypad
  Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

  // Menghidupkan fungsi LCD
  lcd.init();

  // Mengaktifkan lampu LCD (backlight)
  lcd.backlight();

  // Set cursor LCD 0:0 dan print mesej
  lcd.setCursor(0, 0);
  lcd.print("Welcome to Locker");

  // Set cursor LCD 0:0 dan print mesej
  lcd.setCursor(0, 1);
  lcd.print("Controlled System");

  // Set cursor LCD 0:0 dan print mesej
  lcd.setCursor(0, 2);
  lcd.print("(-_-)");

  // Set cursor LCD 0:0 dan print mesej
  lcd.setCursor(2, 3);
  lcd.print("*** Loading ***");

  // Delay 3 saat
  delay(3000);

  // Kosongkan text pada LCD
  lcd.clear();

  // Mengaktifkan fungsi servo
  // mengikut pin yang ditetapkan
  servo1.attach(14); // pin 14
  servo1.write(0); // angle 0

  servo2.attach(15); // pin 15
  servo2.write(0); // angle 0

  servo3.attach(A3); // pin A3
  servo3.write(0); // angle 0

  servo4.attach(2); // pin A4
  servo4.write(0); // angle 0

  Serial.begin(9600); // Mulakan Serial Debugging pada Serial Monitor
}


// SYSTEM START
void loop() {

  // Set cursor 2:0 dan printkan
  // teks 'Master Password:'
  // lihat function - void lcd_show_keypad()
  lcd_show_keypad();
  
  // hadkan nombor pin sehingga 6 aksara
  // 0, 1, 2, 3, 4, 5 dan 6
  int nextColumn = 7;
  const int maxKeys = 7;
  char passwords[maxKeys];

  // mulakan urutan password dengan 0
  int keyIndex = 0;

  // while - true:
  // sentiasa loop code bahagian ini
  while (true) {

    // jika pengguna tekan sebarang kombinasi pada keypad
    // maka, arduino akan menghantar dan mencetak teks
    // pada LCD
    char key = keypad.getKey();
    
    if(key) {

      // jika pengguna menekan huruf 'C',
      // maka, kosongkan teks pada LCD
      // set semula urutan password kepada 0
      if(key == 'C') {
        lcd.clear();
        nextColumn = 7;
        keyIndex = 0;

        // Set semula cursor 2:0 dan cetak
        // teks 'Master Password:' pada LCD
        // lihat function - void lcd_show_keypad()
        lcd_show_keypad();
      }

      // jika pengguna menekan simbol '#',
      // maka, arduino akan mengesahkan master password
      // dengan kombinasi yang diberikan oleh
      // pengguna. Jika pengguna salah masukkan password,
      // maka, buzzer akan berbunyi dan sistem akan reset 
      // semula memori supaya arduino tidak menyimpan sebarang
      // kombinasi yang ditekan oleh pengguna.
      // Jika pengesahan berjaya, maka sistem akan masuk
      // ke mod akses MASTER
      else if(key == '#'){
        
        if (keyIndex == sizeof(master_password) - 1) {
          
          if (strncmp(passwords, master_password, keyIndex) == 0) {

            Serial.println("Admin access granted!");
            master();

          } else {
            
            keyIndex = 0;
            password_error();
          }

          keyIndex = 0;
        }

        // jika pengguna tidak masukkan sebarang kombinasi,
        // lalu mereka menekan '#'.
        // bunyikan buzzer dan reset memori arduino supaya
        // LCD dan keypad kosong
        else {
          keyIndex = 0;
          password_error();
        }
      }

      else {

        // Setiap kali pengguna menekan kombinasi
        // selain dari 'C' dan '#', paparkan
        // kombinasi tersebut ke LCD dan
        // simpan kombinasi tersebut ke memori arduino
        if (keyIndex < maxKeys - 1) {

          passwords[keyIndex] = key;
          keyIndex++;

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

// Fungsi untuk PASSWORD ERROR
// Reset LCD
// Bunyikan buzzer
// Kembali ke halaman utama
void password_error() {
  lcd.clear();
  sound_error();
  loop();
}

// FUNGSI UNTUK HALAMAN UTAMA
// Set cursor 2:0 dan printkan
// teks 'Master Password:'
void lcd_show_keypad() {
  lcd.setCursor(2, 0);
  lcd.print("Master Password:");
}

// FUNGSI UNTUK BUNYI BUZZER
// Boleh diolahkan cara bunyi mengikut
// kesesuaian
void sound_error(){
  digitalWrite(buzzerPin, HIGH);
  delay(100);
  digitalWrite(buzzerPin, LOW);
  delay(100);
  digitalWrite(buzzerPin, HIGH);
  delay(100);
  digitalWrite(buzzerPin, LOW);
}

// MOD / ZON MASTER
// ----------------------
void master(){
  Serial.println("Entering master mode...");
  lcd.clear(); // kosongkan LCD

  // mulakan urutan bacaan locker
  int lockerKeyIndex = 0;
  char lockerNumber[lockerKeyIndex];

  // sentiasa jalankan proses dibawah
  while(true){
    char key = keypad.getKey();

    // paparkan 'Open Locker?' di LCD
    // [1 - 10]
    lcd.setCursor(0, 0);
    lcd.print("Open locker?");
    lcd.setCursor(0, 1);
    lcd.print("[1 - 10]");

    if(key){

      // UNTUK LOGOUT DARI MASTER DAN
      // KEMBALI KE SKRIN UTAMA
      // Jika pengguna menekan 'D',
      // maka kosongkan LCD
      // dan kembali ke skrin utama
      if(key == 'D'){
        lcd.clear();
        loop();
      }

      // UNTUK BERALIH KE MOD CLOSE LOCKER
      // Jika pengguna menekan 'A',
      // set mod ke CLOSE LOCKER
      else if(key == 'A') {
        close_locker();
      }

      else {

        // Jika pengguna menekan '#',
        // bermaksud pengguna telah mengesahkan nombor locker
        // dan mula membuka locker
        if(key == '#'){
          lockerNumber[lockerKeyIndex] = '\0';
          int enteredNumber = atoi(lockerNumber);

          // jika pengguna salah memasukkan nombor locker,
          // bunyikan buzzer. Jika betul, mula bukakan locker
          if (enteredNumber >= 1 && enteredNumber <= 10) {
            Serial.print("You pressed: ");
            Serial.println(enteredNumber);
            open_locker(enteredNumber);

          } else {
            sound_error();
          }

          // Reset memori ke 0 untuk locker seterusnya
          lockerKeyIndex = 0;
          memset(lockerNumber, 0, sizeof(lockerNumber));
        }

        else {
          // Paparkan nombor locker yang 
          // pengguna masukkan
          lockerNumber[lockerKeyIndex] = key;
          lockerKeyIndex++;
          lcd.setCursor(lockerKeyIndex - 1, 2);
          lcd.print(key);
        }
      }
    }
  }
}

// OPEN LOCKER
// ------------
// 1. Arahkan servo ke 90 darjah
// 2. Nyalakan LED
// 3. Delay 1 saat
void open_locker(int lockerNumber){
  if(lockerNumber == 1){
    servo1.write(90);
    digitalWrite(LED1_Pin, HIGH);
    delay(1000);
  }

  else if(lockerNumber == 2) {
    servo2.write(90);
    digitalWrite(LED2_Pin, HIGH);
    delay(1000);
  }

  else if(lockerNumber == 3) {
    servo3.write(90);
    digitalWrite(LED3_Pin, 255);
    delay(1000);
  }

  else if(lockerNumber == 4) {
    servo4.write(90);
    // digitalWrite(LED3_Pin, 255);
    delay(1000);
  }
}

// MOD CLOSE LOCKER
// -----------------------
void close_locker(){

  // Set urutan awal nombor locker
  int lockerKeyIndex = 0;
  char lockerNumber[lockerKeyIndex];

  // Kosongkan LCD, reset cursor LCD dan
  // paparkan 'Close Locker?' dan
  // nombor locker 1-10
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Close locker?");
  lcd.setCursor(0, 1);
  lcd.print("[1 - 10]");

  while(true) {
    char key = keypad.getKey();

    if(key) {

      // Jika pengguna menekan '#',
      // bermaksud pengguna telah mengesahkan nombor locker
      // dan mula menutup locker
      if(key == '#') {
        lockerNumber[lockerKeyIndex] = '\0';
        int enteredNumber = atoi(lockerNumber);

        // jika pengguna salah memasukkan nombor locker,
        // bunyikan buzzer. Jika betul, mula tutup locker
        if (enteredNumber >= 1 && enteredNumber <= 10) {
          Serial.print("You pressed: ");
          Serial.println(enteredNumber);
          // Perform actions for valid number

          close_locker_servo(enteredNumber);

        } else {
          sound_error();
        }

        // Reset memori ke 0 untuk locker seterusnya
        lockerKeyIndex = 0;
        memset(lockerNumber, 0, sizeof(lockerNumber));
      }

      // Di mod CLOSE LOCKER, jika pengguna masukkan 'D',
      // pengguna akan keluar dari mod CLOSE LOCKER
      // dan paparkan mod OPEN LOCKER
      else if(key == 'D') {
        master();
      }

      else {
        // Paparkan nombor locker yang 
        // pengguna masukkan
        lockerNumber[lockerKeyIndex] = key;
        lockerKeyIndex++;
        lcd.setCursor(lockerKeyIndex - 1, 2);
        lcd.print(key);
      }
    }
  }
}


// CLOSE LOCKER
// ------------
// 1. Arahkan servo ke 0 darjah
// 2. Padamkan LED
// 3. Delay 1 saat
void close_locker_servo(int lockerNumber) {
  if(lockerNumber == 1) {
    servo1.write(0);
    digitalWrite(LED1_Pin, LOW);
    delay(1000);
  }

  else if(lockerNumber == 2) {
    servo2.write(0);
    digitalWrite(LED2_Pin, LOW);
    delay(1000);
  }

  else if(lockerNumber == 3) {
    servo3.write(0);
    digitalWrite(LED3_Pin, 0);
    delay(1000);
  }

  else if(lockerNumber == 4) {
    servo4.write(0);
    // digitalWrite(LED3_Pin, 0);
    delay(1000);
  }
}