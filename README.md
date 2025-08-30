# Next-Gen Interactive Smart Notice Board 📢

An IoT-based smart notice board using an ESP32 and RFID to display personalized, real-time information for students.



---

## 🎯 Problem Statement

Traditional notice boards in schools and colleges are static and paper-based. Students often miss critical, personalized updates (exam schedules, lab slots, event notices) because the information is cluttered and not updated in real-time. This project aims to solve that problem.

---

## ✨ Key Features

-   ✅ **RFID-based Personalization**: Each student taps their ID card to see notices relevant only to them.
-   📡 **Real-Time Updates**: Can be extended with Wi-Fi to allow faculty to push notices remotely.
-   📺 **Clear OLED Display**: Information is displayed on a clean, crisp OLED screen.
-   ⚙️ **Scalable**: Built with the powerful ESP32, allowing for future expansion with more sensors or cloud integration.

---

## 🛠️ Tech Stack

### Hardware
* ESP32 DevKitC Board
* MFRC522 RFID Reader + RFID Cards/Fobs
* SSD1306 0.96" I2C OLED Display
* Breadboard and Jumper Wires

### Software
* Arduino IDE
* C++ (Arduino Framework)
* **Libraries:**
    * `MFRC522` by GitHubCommunity
    * `Adafruit GFX`
    * `Adafruit SSD1306`

---

## 🔌 Hardware Connections

| Device             | Pin        | ESP32 Pin   |
| ------------------ | ---------- | ----------- |
| **MFRC522 (RFID)** | **SDA (SS)** | **GPIO 5** |
|                    | **SCK** | **GPIO 18** |
|                    | **MOSI** | **GPIO 23** |
|                    | **MISO** | **GPIO 19** |
|                    | **RST** | **GPIO 4** |
|                    | **3.3V** | **3V3** |
|                    | **GND** | **GND** |
| **SSD1306 (OLED)** | **SCL** | **GPIO 22** |
|                    | **SDA** | **GPIO 21** |
|                    | **VCC** | **3V3** |
|                    | **GND** | **GND** |

---


## 💡 How It Works

The system boots up and displays a "Scan ID" message. When an RFID card is tapped on the reader:
1.  The ESP32 reads the card's unique ID (UID).
2.  It searches the pre-defined list of students to find a matching UID.
3.  If a match is found, the student's name and personalized notice are displayed on the OLED screen.
4.  If no match is found, an "ACCESS DENIED" message is shown.
5.  After 5 seconds, the screen resets to the default message, ready for the next scan.

---

## 🔮 Future Enhancements

-   **Cloud Integration:** Connect to Google Sheets or Firebase to manage student data remotely.
-   **IoT Sensors:** Add sensors like a DHT11 (temperature/humidity) or MQ135 (air quality) to display environmental data.
-   **Web Dashboard:** Create a web interface for faculty to easily update notices.

---

## 📄 License

This project is licensed under the MIT License. See the `LICENSE` file for details.
