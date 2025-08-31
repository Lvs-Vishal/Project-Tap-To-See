# Next-Gen Interactive Smart Notice Board

An IoT-based smart notice board using an ESP32 and RFID to display personalized, real-time information for students, managed via a cloud-based web dashboard.



---

## Problem Statement

Traditional notice boards in schools and colleges are static and paper-based. Students often miss critical, personalized updates (exam schedules, lab slots, event notices) because the information is cluttered and not updated in real-time. This project solves that problem with a modern, full-stack IoT solution.

---

## Key Features

-   **RFID-based Personalization**: Each student taps their ID card to see their own notices and attendance.
-   **Cloud-Based Management**: All student data is stored and managed in a central Google Sheet, acting as a lightweight database.
-   **Dynamic Web Dashboard**: A user-friendly web interface for faculty to remotely add, update, and view student records, including photos.
-   **Real-Time Attendance Tracking**: The system automatically logs attendance, preventing duplicate scans on the same day.
-   **Live Data Display**: The ESP32 fetches and displays live data, including a real-time clock synced via an NTP server.
-   **Modern UI**: The web dashboard features a dynamic, animated interface with dark mode, search, and professional pop-up notifications.

---

## System Architecture

This project uses a client-server model where the Google Sheet acts as the central database.

1.  **The Client (ESP32)**: The physical device reads an RFID card, connects to the internet, and makes an HTTP request to the API to fetch and update data.
2.  **The Admin Dashboard (Web App)**: A web-based front-end that allows administrators to manage student data through a user-friendly interface.
3.  **The Backend (Google Apps Script)**: A serverless API that acts as the secure bridge between the clients and the Google Sheet, handling all data logic.

---

## Tech Stack

### Hardware
* ESP32 DevKitC Board
* MFRC522 RFID Reader + RFID Cards/Fobs
* SSD1306 0.96" I2C OLED Display
* Breadboard and Jumper Wires

### Cloud & Backend
* **Database**: Google Sheets
* **API**: Google Apps Script (JavaScript)

### Device Software
* **Framework**: Arduino (C++)
* **Libraries**: `MFRC522`, `Adafruit GFX`, `Adafruit SSD1306`, `ArduinoJson`, `HTTPClient`

### Web Dashboard
* **Languages**: HTML, CSS, JavaScript (Vanilla JS)
* **Libraries**: Font Awesome (icons), SweetAlert2 (pop-ups)

---

## Hardware Connections

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

## How It Works

### On the ESP32 Device
1.  The system boots, connects to Wi-Fi, and displays a real-time clock.
2.  When a student taps their card, the ESP32 reads the UID and sends an HTTP GET request to the Google Script API.
3.  The API checks if the student has already scanned today.
    * If **yes**, it returns a "Marked for today!" message.
    * If **no**, it increments the `AttendanceDays` count in the Google Sheet and logs the timestamp.
4.  The API returns a JSON object with the student's updated details.
5.  The ESP32 parses the JSON and displays the personalized info on the OLED screen.

### On the Web Dashboard
1.  Visiting the Web app URL loads the admin dashboard.
2.  The dashboard fetches all student records from the Google Sheet and displays them in an interactive grid.
3.  The administrator can add a new student or click on an existing student to update their details (name, notice, photo URL) via a pop-up form.
4.  Submitting the form triggers a function in the Apps Script that updates the Google Sheet in real-time.

---

## Future Enhancements

-   **Migrate to Firebase**: Upgrade from Google Sheets to a more robust Firebase Realtime Database for faster performance and better scalability.
-   **Add IoT Sensors**: Integrate sensors like a DHT11 (temperature/humidity) to display live environmental data on the device.
-   **Mobile Notifications**: Implement a system to push critical notices directly to a student's phone.

---

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.
