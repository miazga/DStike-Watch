/*
 * BadUSB module - Handles keyboard emulation and DuckyScript execution
 * Based on WiFiDuck keyboard implementation
 * 
 * NOTE: This module adds ~2KB to flash size due to Keyboard library
 * Only include if FEATURE_BADUSB is defined in config.h
 */

#pragma once

#ifdef FEATURE_BADUSB

#include <Arduino.h>
#include <Keyboard.h>
#include "config.h"

namespace BadUSB {
  bool isRunning = false;

  void begin() {
    Keyboard.begin();
  }


  void runBrowserScript() {
    if (isRunning) return;
    isRunning = true;
    
    delay(500);
    
    #ifdef BADUSB_TARGET_MAC
    // macOS: Open Spotlight (CMD+Space)
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(' ');
    delay(100);
    Keyboard.releaseAll();
    delay(1000);  // Wait for Spotlight to open
    
    #elif defined(BADUSB_TARGET_WINDOWS)
    // Windows: Open Run dialog (WIN+R)
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('r');
    delay(100);
    Keyboard.releaseAll();
    delay(700);  // Wait for Run dialog
    
    #else
    // Default: Try Windows method
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('r');
    delay(100);
    Keyboard.releaseAll();
    delay(700);
    #endif
    
    // Type the URL (works on both)
    Keyboard.print("https://www.youtube.com/watch?v=e-xoYTHebs8&autoplay=1&mute=1");
    delay(600);  // Longer delay after typing
    
    // Press Enter to open (explicit press and release)
    Keyboard.press(KEY_RETURN);
    delay(200);
    Keyboard.release(KEY_RETURN);
    
    isRunning = false;
  }
}

#endif // FEATURE_BADUSB

