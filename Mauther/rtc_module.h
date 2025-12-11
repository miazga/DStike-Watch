/*
 * RTC module - Handles DS3231 Real-Time Clock
 * Only include if FEATURE_RTC is defined
 */

#pragma once

#ifdef FEATURE_RTC

#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include "config.h"

namespace RTCModule {
  RTC_DS3231 rtc;
  bool rtcAvailable = false;
  DateTime lastTime;

  void begin() {
    if (rtc.begin()) {
      rtcAvailable = true;
      
      // FORCE TIME UPDATE - Remove this after first upload!
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      
      // Normal code (uncomment after time is set):
      // if (rtc.lostPower()) {
      //   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      // }
      
      lastTime = rtc.now();
    }
  }

  DateTime getTime() {
    if (rtcAvailable) {
      lastTime = rtc.now();
    }
    return lastTime;
  }

  float getTemperature() {
    if (rtcAvailable) {
      return rtc.getTemperature();
    }
    return 0.0;
  }

  void getTimeString(char* buffer, size_t bufferSize) {
    DateTime now = getTime();
    snprintf(buffer, bufferSize, "%02d:%02d:%02d", 
             now.hour(), now.minute(), now.second());
  }

  void getDateString(char* buffer, size_t bufferSize) {
    DateTime now = getTime();
    snprintf(buffer, bufferSize, "%04d-%02d-%02d", 
             now.year(), now.month(), now.day());
  }

  void setTime(uint16_t year, uint8_t month, uint8_t day, 
               uint8_t hour, uint8_t minute, uint8_t second) {
    if (rtcAvailable) {
      rtc.adjust(DateTime(year, month, day, hour, minute, second));
    }
  }

  bool isAvailable() {
    return rtcAvailable;
  }
}

#endif // FEATURE_RTC

