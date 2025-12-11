/*
 * Mauther - Custom Firmware for DStike Bad Watch
 * 
 * Hardware:
 * - ATmega32U4 (Arduino Leonardo)
 * - 1.3" OLED SH1106 (I2C: SDA=Pin3, SCL=Pin2)
 * - VL53L0X Distance Sensor (I2C)
 * - DS3231 RTC (I2C)
 * - Buzzer (Pin 12)
 * - RGB LED (Pin 6)
 * - Laser (Pin 13)
 * - Buttons: Up(Pin 10), Down(Pin 8), Select(Pin 9)
 * 
 * Features:
 * - Time display with RTC
 * - Distance measurement with alarm
 * - Laser pointer control
 * - BadUSB/DuckyScript execution
 * - Menu system
 */

#include "config.h"
#include "display.h"
#include "actuators.h"
#include "buttons.h"

#ifdef FEATURE_DISTANCE_SENSOR
#include "sensors.h"
#endif

#ifdef FEATURE_RTC
#include "rtc_module.h"
#endif

#ifdef FEATURE_BADUSB
#include "badusb.h"
#endif

#include "menu.h"

void setup() {
  #ifdef DEBUG_MODE
  Serial.begin(115200);
  #endif

  Display::begin();
  
  #ifdef FEATURE_DISTANCE_SENSOR
  Sensors::begin();
  #endif
  
  Actuators::begin();
  Buttons::begin();
  
  #ifdef FEATURE_RTC
  RTCModule::begin();
  #endif
  
  #ifdef FEATURE_BADUSB
  BadUSB::begin();
  #endif
  
  Menu::begin();

  Actuators::playBootSound();
  delay(500);
}

void loop() {
  Buttons::update();
  
  #ifdef FEATURE_DISTANCE_SENSOR
  Sensors::update();
  #endif
  
  Menu::update();
  
  delay(10);
}

