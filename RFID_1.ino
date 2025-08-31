#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "time.h"

// --- User Configuration ---
const char* WIFI_SSID = "[ID]";
const char* WIFI_PASS = "[PASSWORD]";
String GOOGLE_SCRIPT_URL = "[Your_Scrip_URL]";

// --- Time Configuration ---
const char* NTP_SERVER = "pool.ntp.org";
const long  GMT_OFFSET_SEC = 19800; // India Standard Time (IST) is UTC+5:30 (5.5 * 3600)
const int   DAYLIGHT_OFFSET_SEC = 0;

// --- Hardware Pins & Objects ---
#define SS_PIN    5
#define RST_PIN   4
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

MFRC522 mfrc522(SS_PIN, RST_PIN);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
struct tm timeinfo; // Structure to hold time data

void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  
  // --- Initialize Display ---
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Connecting to WiFi...");
  display.display();

  // --- Connect to WiFi ---
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");

  // --- Initialize Time from NTP Server ---
  configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER);
}

void loop() {
  showDefaultScreen(); // Continuously update the time on the default screen

  // --- RFID Card Detection ---
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }

  // A card was scanned, get its UID
  // --- CORRECTED UID FORMATTING ---
  String cardUID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    // Add a leading zero if the hex value is a single digit
    if (mfrc522.uid.uidByte[i] < 0x10) {
      cardUID += "0";
    }
    cardUID += String(mfrc522.uid.uidByte[i], HEX);
  }
  cardUID.toUpperCase();
  // The UID will now be a solid string like "22631205"
  
  Serial.println("Card Scanned (Formatted): " + cardUID);

  // --- API Call to Google Sheet ---
  updateAndDisplayInfo(cardUID);
  
  delay(10000); // Show student info for 10 seconds before reverting
}

void updateAndDisplayInfo(String uid) {
  display.clearDisplay();
  display.setCursor(10, 20);
  display.setTextSize(2);
  display.println("Processing...");
  display.display();
  
  HTTPClient http;
  String fullUrl = GOOGLE_SCRIPT_URL + "?uid=" + uid;
  http.begin(fullUrl);
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  
  int httpCode = http.GET();
  if (httpCode > 0) {
    String payload = http.getString();
    Serial.println("Response: " + payload);

    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);

    if (doc["found"] == true) {
      String name = doc["name"];
      String notice = doc["notice"];
      int attendance = doc["attendance"];
      int totalDays = doc["totalDays"];
      float percentage = ((float)attendance / totalDays) * 100.0;
      
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0, 0);
      display.println(name);
      display.setTextSize(1);
      display.setCursor(0, 20);
      display.println("Att: " + String(attendance) + "/" + String(totalDays) + " (" + String(percentage, 1) + "%)");
      display.setCursor(0, 35);
      display.println("Notice: " + notice);
      
    } else {
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(10, 20);
      display.println("ACCESS");
      display.setCursor(25, 40);
      display.println("DENIED");
    }
  } else {
    Serial.println("Error on HTTP request");
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,20);
    display.println("Error connecting to server.");
  }
  
  display.display();
  http.end();
}

void showDefaultScreen() {
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  
  char timeString[9]; // HH:MM:SS
  strftime(timeString, sizeof(timeString), "%H:%M:%S", &timeinfo);
  char dateString[11]; // YYYY-MM-DD
  strftime(dateString, sizeof(dateString), "%Y-%m-%d", &timeinfo);

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(20, 10);
  display.println(timeString);
  display.setTextSize(1);
  display.setCursor(25, 40);
  display.println(dateString);
  display.display();
}
