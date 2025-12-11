/*
 * Display module - Handles OLED SH1106 display
 */

#pragma once
#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include "config.h"

// Make u8g2 globally accessible
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

namespace Display {
  // Option 1: Hardware I2C (recommended - uses Arduino Wire library)
  // U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
  
  // Option 2: Software I2C with current pins (if Option 1 doesn't work)
  // U8G2_SH1106_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, PIN_OLED_SCL, PIN_OLED_SDA, U8X8_PIN_NONE);
  
  // Option 3: Try SSD1306 instead of SH1106 (some boards mislabeled)
  // U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

  void begin() {
    Wire.begin();
    Wire.setClock(400000);
    u8g2.begin();
    u8g2.setContrast(255);
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.setFontPosTop();
  }

  // Removed clear() and show() - not needed with page buffer mode

  void showSplash(const char* line1, const char* line2) {
    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_6x10_tf);
      u8g2.drawStr(30, 25, line1);
      u8g2.drawStr(40, 40, line2);
    } while (u8g2.nextPage());
  }

  void drawMainScreen(const char* time, float temp, uint16_t distance, bool laserOn) {
    char buf[12];
    
    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_6x10_tf);
      // Time
      u8g2.drawStr(0, 0, time);
      
      // Temperature
      snprintf(buf, sizeof(buf), "%.1fC", temp);
      u8g2.drawStr(0, 20, buf);
      
      // Distance
      if (distance > DISTANCE_MAX_RANGE) {
        u8g2.drawStr(0, 35, "---");
      } else {
        snprintf(buf, sizeof(buf), "%dmm", distance);
        u8g2.drawStr(0, 35, buf);
      }
      
      // Laser
      if (laserOn) u8g2.drawStr(0, 50, "LSR");
    } while (u8g2.nextPage());
  }

  void drawMenu(const char* title, const char* items[], int itemCount, int selected) {
    int maxVis = 5;
    int offset = (itemCount > maxVis && selected >= maxVis) ? min(selected - maxVis + 1, itemCount - maxVis) : 0;
    
    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_6x10_tf);
      u8g2.drawStr(0, 0, title);
      
      int y = 12;
      for (int i = offset; i < min(offset + maxVis, itemCount); i++) {
        if (i == selected) u8g2.drawStr(0, y, ">");
        u8g2.drawStr(8, y, items[i]);
        y += 10;
      }
    } while (u8g2.nextPage());
  }

  void drawText(const char* text) {
    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_6x10_tf);
      u8g2.drawStr(0, 0, text);
    } while (u8g2.nextPage());
  }

  void drawCentered(const char* text) {
    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_6x10_tf);
      u8g2.drawStr(30, 32, text);
    } while (u8g2.nextPage());
  }

  void update() {
    // Placeholder for any display animations or updates
  }
}

