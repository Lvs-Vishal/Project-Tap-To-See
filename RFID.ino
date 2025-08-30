#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- Pin Definitions ---
#define SS_PIN    5   // RFID SDA (SS) pin
#define RST_PIN   4   // RFID Reset pin
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// --- Object Initializations ---
MFRC522 mfrc522(SS_PIN, RST_PIN);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- Student Data Structure ---
struct Student {
  byte uid[4];
  const char* name;
  const char* notice;
};

// --- Mock Student Database ---
// Enter the 4 UIDs you provided
Student students[] = {
  {{0x34, 0xDB, 0x3B, 0x06}, "Vishal", "Lab session: Mon 2PM"},
  {{0x22, 0x63, 0x12, 0x05}, "Durai", "Exam on: 04/09/25"},
  {{0xD6, 0xFC, 0xF8, 0x05}, "Palanivel", "Fee payment due"},
  {{0x74, 0xF0, 0xE9, 0x00}, "venkat", "Project submission Fri"}
};
const int numStudents = sizeof(students) / sizeof(students[0]);

void setup() {
  Serial.begin(9600); // For debugging
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522

  // Initialize OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  showDefaultMessage();
}

void loop() {
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }

  // A card is present, find the matching student
  bool studentFound = false;
  for (int i = 0; i < numStudents; i++) {
    // Compare the scanned UID with the student's UID in our database
    if (memcmp(mfrc522.uid.uidByte, students[i].uid, 4) == 0) {
      displayStudentInfo(students[i]);
      studentFound = true;
      break; // Exit the loop once a match is found
    }
  }

  if (!studentFound) {
    showAccessDenied();
  }

  // Halt the card and wait before showing the default message again
  mfrc522.PICC_HaltA();
  delay(5000); // Show the message for 5 seconds
  showDefaultMessage();
}

// --- Display Functions ---

void showDefaultMessage() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.println("Scan ID");
  display.display();
}

void displayStudentInfo(const Student& student) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println(student.name);
  display.setTextSize(1);
  display.println("---------------------");
  display.setCursor(0, 35);
  display.println(student.notice);
  display.display();
}

void showAccessDenied() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(10, 20);
  display.println("ACCESS");
  display.setCursor(25, 40);
  display.println("DENIED");
  display.display();
}
