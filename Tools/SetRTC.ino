/*
 * RTC Time Setter for DStike Bad Watch
 * 
 * This sketch sets the DS3231 RTC to a specific date/time.
 * 
 * Instructions:
 * 1. Edit the date/time values below
 * 2. Upload this sketch to your watch
 * 3. The RTC will be set immediately
 * 4. Upload Mauther firmware back to restore functionality
 */

#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("RTC Time Setter");
  Serial.println("===============");
  
  if (!rtc.begin()) {
    Serial.println("ERROR: Couldn't find RTC!");
    while (1) delay(10);
  }
  
  Serial.println("RTC found!");
  
  // ===== SET YOUR TIME HERE =====
  // Format: Year, Month, Day, Hour, Minute, Second
  // Example: December 11, 2025, 18:30:00
  
  rtc.adjust(DateTime(2025, 12, 11, 18, 30, 0));
  //              Year  Mon  Day  Hr  Min  Sec
  
  // OR: Set to compile time (your computer's current time)
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
  Serial.println("\nTime has been set!");
  Serial.println("===================");
  
  // Read and display the new time
  DateTime now = rtc.now();
  
  Serial.print("Date: ");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.println(now.day(), DEC);
  
  Serial.print("Time: ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.println(now.second(), DEC);
  
  Serial.print("Temperature: ");
  Serial.print(rtc.getTemperature());
  Serial.println(" C");
  
  Serial.println("\n===================");
  Serial.println("Done! You can now upload Mauther firmware.");
  Serial.println("The time will persist (RTC has battery backup).");
}

void loop() {
  // Display current time every second
  static unsigned long lastPrint = 0;
  
  if (millis() - lastPrint > 1000) {
    DateTime now = rtc.now();
    
    char buf[20];
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d", 
             now.hour(), now.minute(), now.second());
    Serial.println(buf);
    
    lastPrint = millis();
  }
}

