/**
 * Handles requests from both the ESP32 device and the web browser dashboard.
 */
function doGet(e) {
  // Check if the 'uid' parameter exists in the URL, indicating a request from the ESP32
  if (e.parameter.uid) {
    return handleEsp32Request(e.parameter.uid);
  } else {
    // If no 'uid' parameter, serve the HTML file for the web dashboard
    return HtmlService.createHtmlOutputFromFile('Index')
        .setTitle("Smart Notice Board Dashboard");
  }
}

/**
 * Processes requests specifically from the ESP32 device.
 * @param {string} uid - The UID of the card scanned.
 * @returns {ContentService} A JSON object with the student's data.
 */
function handleEsp32Request(uid) {
  const sheet = SpreadsheetApp.getActiveSpreadsheet().getSheetByName("Sheet1");
  const data = sheet.getDataRange().getValues();
  let result = { "found": false };

  const TIMEZONE = "Asia/Kolkata"; 
  const TOTAL_DAYS = 100;
  const now = new Date();
  const todayStr = Utilities.formatDate(now, TIMEZONE, "yyyy-MM-dd");

  for (let i = 1; i < data.length; i++) { // Start from row 1 to skip header
    if (data[i][0].toString().trim().toUpperCase() === uid.toString().trim().toUpperCase()) {
      
      const lastUpdatedTimestamp = data[i][4]; // Timestamp is in Column E
      let lastUpdatedDateStr = "";
      if (lastUpdatedTimestamp && lastUpdatedTimestamp instanceof Date) {
        lastUpdatedDateStr = Utilities.formatDate(lastUpdatedTimestamp, TIMEZONE, "yyyy-MM-dd");
      }

      let currentAttendance = parseInt(data[i][3]) || 0;
      let notice = data[i][2];
      let status = "ok";

      // --- FIXED: Attendance Logic ---
      if (lastUpdatedDateStr === todayStr) {
        notice = "Already Marked Today!"; // Custom message for repeat scan
        status = "already_marked";
      } else {
        // It's a new day, so increment attendance
        currentAttendance++;
        const formattedTimestamp = Utilities.formatDate(now, TIMEZONE, "yyyy-MM-dd HH:mm:ss");
        sheet.getRange(i + 1, 4).setValue(currentAttendance); // Update Column D
        sheet.getRange(i + 1, 5).setValue(formattedTimestamp); // Update Column E
        status = "updated";
      }
      
      result = {
        "found": true,
        "name": data[i][1],
        "notice": notice,
        "attendance": currentAttendance,
        "totalDays": TOTAL_DAYS,
        "status": status
      };
      break; 
    }
  }
  return ContentService.createTextOutput(JSON.stringify(result)).setMimeType(ContentService.MimeType.JSON);
}


// --- FUNCTIONS FOR THE WEB APP ---

/**
 * Gets all student data for the web dashboard.
 */
function getStudents() {
  const sheet = SpreadsheetApp.getActiveSpreadsheet().getSheetByName("Sheet1");
  const data = sheet.getRange(2, 1, sheet.getLastRow() - 1, 6).getValues(); // Get 6 columns now
  
  return data.map(row => ({
    uid: row[0],
    name: row[1],
    notice: row[2],
    attendance: row[3],
    lastUpdated: row[4] ? new Date(row[4]).toLocaleString() : "N/A",
    photoUrl: row[5] || "" // Get photo URL from Column F
  }));
}

/**
 * Adds or updates a student from the web dashboard.
 */
function addOrUpdateStudent(formData) {
  const sheet = SpreadsheetApp.getActiveSpreadsheet().getSheetByName("Sheet1");
  const data = sheet.getDataRange().getValues();
  const uidToFind = formData.uid.toString().trim().toUpperCase();
  let recordFound = false;

  for (let i = 1; i < data.length; i++) {
    if (data[i][0].toString().trim().toUpperCase() === uidToFind) {
      // Update existing record
      sheet.getRange(i + 1, 2).setValue(formData.name);
      sheet.getRange(i + 1, 3).setValue(formData.notice);
      sheet.getRange(i + 1, 6).setValue(formData.photoUrl); // Update photo URL
      recordFound = true;
      return "Student record updated successfully!";
    }
  }

  if (!recordFound) {
    // Add a new record
    sheet.appendRow([uidToFind, formData.name, formData.notice, 0, "", formData.photoUrl]);
    return "New student added successfully!";
  }
}
