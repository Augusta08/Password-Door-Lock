
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>

const int rs = A0, en = A1, d4 = A2, d5 = A3, d6 = A4, d7 = A5;  // LCD Pins
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Servo myservo;
const int ROWS = 4;
const int COLS = 4;
char keys[ROWS][COLS] = {
  { '1', '2', '3', '*' },
  { '4', '5', '6', '*' },
  { '7', '8', '9', '/' },
  { 'C', '0', '=', '#' }
};
byte rowPins[ROWS] = { 9, 8, 7, 6 };  //connect to the row pinouts of the keypad
byte colPins[COLS] = { 5, 4, 3, 2 };  //connect to the column pinouts of the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
char correctPin[5] = "1234";  // Pin for (un)locking the door

enum DoorState {DOOR_CLOSED, DOOR_OPEN};
DoorState doorState = DOOR_CLOSED;

bool messagePrinted = false; // Close the door message

void displayEnterPinMessage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter The Pin");
  lcd.setCursor(0, 1);
}

void displayLCD(const char* line1, const char* line2 = "") {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  if (line2[0] != '\0') {
    lcd.setCursor(0, 1);
    lcd.print(line2);
  }

}

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  myservo.attach(11);
  displayEnterPinMessage();
  myservo.write(0);
}


void loop() {
  static String userInput = "";
  char key = keypad.getKey();
  if (doorState == DOOR_CLOSED) {
    if (key) {  // Check if a key is pressed
      if (key == 'C') { // Clear button
        userInput = "";
        displayEnterPinMessage();
      } else if (key == '=') { 
        if (userInput == correctPin) { // Check if pin entered by user is correct
          myservo.write(90);
          doorState = DOOR_OPEN;
          userInput = "";
        } else {
          userInput = "";
          displayLCD("Incorrect Pin");
          delay(1000);
          displayEnterPinMessage();
        }

      } else {
        lcd.print(key);    // Display the key on the LCD
        userInput += key;  // Append key to userInput string if needed
      }
    }
  } else {

    if (!messagePrinted) {
      displayLCD("Enter 'C' to", "close the door");
      messagePrinted = true;
    }
    if (key == '/') {
      myservo.write(0);
      doorState = DOOR_CLOSED;
      messagePrinted = false;
      displayEnterPinMessage();
      delay(100);
    }
  }
}