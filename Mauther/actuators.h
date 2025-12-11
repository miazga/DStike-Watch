/*
 * Actuators module - Handles Buzzer, RGB LED, and Laser
 * 
 * Note: DStike Bad Watch has TWO LEDs:
 * 1. White LED - Hardware controlled by power button
 *    - Long press (>2s): Turn on white LED
 *    - Double click: Power off device
 *    - Not controllable from software
 * 
 * 2. RGB LED (Pin 6) - NeoPixel WS2812 addressable LED
 *    - Requires Adafruit_NeoPixel library
 *    - True RGB color control
 *    - Used for status indicators
 */

#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "config.h"

namespace Actuators {
  bool laserEnabled = false;
  uint8_t ledR = 0, ledG = 0, ledB = 0;
  
  #ifdef FEATURE_LED
  // NeoPixel configuration
  #define NEOPIXEL_COUNT 1
  Adafruit_NeoPixel strip(NEOPIXEL_COUNT, PIN_RGB_LED, NEO_GRB + NEO_KHZ800);
  #endif

  void begin() {
    #ifdef FEATURE_BUZZER
    pinMode(PIN_BUZZER, OUTPUT);
    digitalWrite(PIN_BUZZER, LOW);
    #endif
    
    #ifdef FEATURE_LED
    strip.begin();
    strip.setBrightness(50);
    strip.clear();
    strip.show();
    
    // Boot LED test - blink and turn off
    strip.setPixelColor(0, strip.Color(255, 0, 0)); // Red
    strip.show();
    delay(200);
    strip.setPixelColor(0, strip.Color(0, 255, 0)); // Green
    strip.show();
    delay(200);
    strip.setPixelColor(0, strip.Color(0, 0, 255)); // Blue
    strip.show();
    delay(200);
    strip.clear(); // Turn OFF
    strip.show();
    #endif
    
    #ifdef FEATURE_LASER
    pinMode(PIN_LASER, OUTPUT);
    digitalWrite(PIN_LASER, LOW);
    #endif
  }


  void playBootSound() {
    #ifdef FEATURE_BUZZER
    tone(PIN_BUZZER, BUZZER_BOOT_FREQ, 100);
    delay(150);
    noTone(PIN_BUZZER);
    #endif
  }


  // ===== RGB LED Functions (NeoPixel) =====
  
  void setLED(uint8_t r, uint8_t g, uint8_t b) {
    #ifdef FEATURE_LED
    strip.setPixelColor(0, strip.Color(r, g, b));
    strip.show();
    #endif
  }

  void setLEDRed() { setLED(255, 0, 0); }
  void setLEDGreen() { setLED(0, 255, 0); }
  void setLEDBlue() { setLED(0, 0, 255); }
  void setLEDYellow() { setLED(255, 255, 0); }
  void setLEDOff() { setLED(0, 0, 0); }

  // ===== Laser Functions =====
  void laserOn() {
    #ifdef FEATURE_LASER
    digitalWrite(PIN_LASER, HIGH);
    laserEnabled = true;
    #endif
  }

  void laserOff() {
    #ifdef FEATURE_LASER
    digitalWrite(PIN_LASER, LOW);
    laserEnabled = false;
    #endif
  }

  void laserToggle() {
    #ifdef FEATURE_LASER
    if (laserEnabled) {
      laserOff();
    } else {
      laserOn();
    }
    #endif
  }

  bool isLaserOn() {
    return laserEnabled;
  }
}

