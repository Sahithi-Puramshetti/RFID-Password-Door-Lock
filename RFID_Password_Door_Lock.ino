// Include required libraries
#include <MFRC522.h>
#include <Keypad.h>
#include <SoftwareSerial.h>
#include <SPI.h>
// Create instances
MFRC522 mfrc522(53, 5); // MFRC522 mfrc522(SS_PIN, RST_PIN)
// include the library code:
#include <LiquidCrystal.h>

 

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = A8, en = A9, d4 = A10, d5 = A11, d6 = A12, d7 = A13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#define Doorrelay 49

 

char initial_password[4] = {'1', '2', '3', '4'};  // Variable to store initial password
String tagUID = "19 47 64 C1";  // String to store UID of tag. Change it with your tag's UID
char password[4];   // Variable to store users password
boolean RFIDMode = true; // boolean to change modes
boolean NormalMode = true; // boolean to change modes
char key_pressed = 0; // Variable to store incoming keys
uint8_t i = 0;  // Variable used for counter
// defining how many rows and columns our keypad have
const byte rows = 3;
const byte columns = 3;
// Keypad pin map
char hexaKeys[rows][columns] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'}
 
};
// Initializing pins for keypad
byte row_pins[rows] = {A0, A1, A2};
byte column_pins[columns] = {A3, A4, A5};
// Create instance for keypad
Keypad keypad_key = Keypad( makeKeymap(hexaKeys), row_pins, column_pins, rows, columns);
void setup() {
  Serial.begin(9600);
  pinMode(Doorrelay, OUTPUT);
  Serial.println("Scan your tag......... ");
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("WELCOME");
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan ur TAG");

}
void loop() {
  
 if (NormalMode == true) {
    // System will first look for mode
    if (RFIDMode == true) {
      // Function to receive message

 

      // Look for new cards
      if ( ! mfrc522.PICC_IsNewCardPresent()) {
        return;
      }
      // Select one of the cards
      if ( ! mfrc522.PICC_ReadCardSerial()) {
        return;
      }
      //Reading from the card
      String tag = "";
      for (byte j = 0; j < mfrc522.uid.size; j++)
      {
        tag.concat(String(mfrc522.uid.uidByte[j] < 0x10 ? " 0" : " "));
        tag.concat(String(mfrc522.uid.uidByte[j], HEX));
      }
      tag.toUpperCase();
      //Checking the card
      if (tag.substring(1) == tagUID)
      {
        Serial.println("Tag mached. ");
        Serial.println("Enter Password");
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("TAG matched");
        delay(2000);
        lcd.clear();
        lcd.print("Enter Password:");
        lcd.setCursor(0, 1);
        RFIDMode = false; // Make RFID mode false
      }
      else
      {
        Serial.print("Wrong Tag Shown");
        Serial.println("  Access Denied ");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Wrong Tag Shown");
        lcd.setCursor(0, 1);
        lcd.print("Access Denied");
        delay(3000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Scan Your Tag   ");        
        
      }
    }
    // If RFID mode is false, it will look for keys from keypad
    if (RFIDMode == false) {
      key_pressed = keypad_key.getKey(); // Storing keys
      if (key_pressed)
      {
        password[i++] = key_pressed; // Storing in password variable
        lcd.setCursor(0, 1);
        lcd.print("****");
      }
      if (i == 4) // If 4 keys are completed
      {
        delay(200);
        if (!(strncmp(password, initial_password, 4))) // If password is matched
        {
          Serial.print("Password Accepted");
          lcd.clear();
          lcd.print("Pass Accepted");
          digitalWrite(Doorrelay, LOW);
          
          
          lcd.clear();
          i = 0;
          RFIDMode = true; // Make RFID mode true
        }
        else    // If password is not matched
        {
          Serial.println("Wrong Password");
          lcd.clear();
          lcd.print("Wrong Password");        
          delay(5000);
          
          lcd.clear();
          i = 0;
          RFIDMode = true;  // Make RFID mode true
        }
         
          digitalWrite(Doorrelay, HIGH);
          lcd.setCursor(0, 0);
          lcd.print("Scan ur TAG");
      }
    }
  }
}