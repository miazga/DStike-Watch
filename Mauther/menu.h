/*
 * Menu module - Handles menu system and navigation
 */

#pragma once
#include <Arduino.h>
#include <U8g2lib.h>
#include "config.h"
#include "display.h"
#include "buttons.h"
#include "actuators.h"

#ifdef FEATURE_DISTANCE_SENSOR
#include "sensors.h"
#endif

#ifdef FEATURE_RTC
#include "rtc_module.h"
#endif

// Access to u8g2 for direct drawing in menu
extern U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2;

namespace Menu {
  enum MenuState {
    MENU_MAIN_SCREEN,
    MENU_MAIN_MENU,
    MENU_DISTANCE,
    MENU_LASER,
    MENU_LED_TEST,
    MENU_BADUSB,
    MENU_SETTINGS,
    MENU_SLEEP
  };

  MenuState currentMenu = MENU_MAIN_SCREEN;
  int menuSelection = 0;
  unsigned long lastActivity = 0;
  bool distanceAlarmActive = false;

  const char* mainMenuItems[] = {
    "Back",
    #ifdef FEATURE_DISTANCE_SENSOR
    "Distance",
    #endif
    "Laser",
    "LED",
    #ifdef FEATURE_BADUSB
    "BadUSB",
    #endif
    #ifdef FEATURE_RTC
    "Info",
    #endif
    "Sleep"
  };

  void begin() {
    currentMenu = MENU_MAIN_SCREEN;
    menuSelection = 0;
    lastActivity = millis();
    
    // Ensure LED is OFF when starting
    #ifdef FEATURE_LED
    Actuators::setLEDOff();
    #endif
  }

  void resetTimeout() {
    lastActivity = millis();
  }

  void checkTimeout() {
    if (currentMenu != MENU_MAIN_SCREEN) {
      if (millis() - lastActivity > MENU_TIMEOUT_MS) {
        currentMenu = MENU_MAIN_SCREEN;
        menuSelection = 0;
        // Turn off LED when returning to main screen
        #ifdef FEATURE_LED
        Actuators::setLEDOff();
        #endif
      }
    }
  }

  void handleMainScreen() {
    // Get current data
    char timeStr[16] = "00:00:00";
    float temp = 0.0;
    uint16_t distance = 9999;
    
    #ifdef FEATURE_RTC
    RTCModule::getTimeString(timeStr, sizeof(timeStr));
    temp = RTCModule::getTemperature();
    #endif
    
    #ifdef FEATURE_DISTANCE_SENSOR
    distance = Sensors::getDistance();
    #endif
    
    bool laserOn = Actuators::isLaserOn();

    // Check distance alarm - LED only (no buzzer)
    #ifdef FEATURE_DISTANCE_SENSOR
    if (Sensors::isAlarmTriggered()) {
      if (!distanceAlarmActive) {
        Actuators::setLEDRed();
        distanceAlarmActive = true;
      }
    } else {
      if (distanceAlarmActive) {
        Actuators::setLEDOff();
        distanceAlarmActive = false;
      }
    }
    #endif

    // Display main screen
    Display::drawMainScreen(timeStr, temp, distance, laserOn);

    // Check for button press to enter menu
    Buttons::Button btn = Buttons::getLastPressed();
    if (btn == Buttons::BTN_SELECT) {
      currentMenu = MENU_MAIN_MENU;
      menuSelection = 0;
      resetTimeout();
    } else if (btn == Buttons::BTN_DOWN) {
      Actuators::laserToggle();
    }
  }

  void handleMainMenu() {
    Display::drawMenu("MENU", mainMenuItems, MENU_ITEMS, menuSelection);

    Buttons::Button btn = Buttons::getLastPressed();
    if (btn == Buttons::BTN_UP) {
      menuSelection = (menuSelection - 1 + MENU_ITEMS) % MENU_ITEMS;
      resetTimeout();
    } else if (btn == Buttons::BTN_DOWN) {
      menuSelection = (menuSelection + 1) % MENU_ITEMS;
      resetTimeout();
    } else if (btn == Buttons::BTN_SELECT) {
      resetTimeout();
      
      // Map menu selection to actual menu state dynamically
      // This adjusts for disabled features automatically
      int itemIndex = 0;
      
      // Item 0: Always "Back"
      if (menuSelection == itemIndex++) {
        currentMenu = MENU_MAIN_SCREEN;
        return;
      }
      
      // Item 1: Distance (if enabled)
      #ifdef FEATURE_DISTANCE_SENSOR
      if (menuSelection == itemIndex++) {
        currentMenu = MENU_DISTANCE;
        return;
      }
      #endif
      
      // Item 2/3: Laser (always enabled)
      if (menuSelection == itemIndex++) {
        currentMenu = MENU_LASER;
        return;
      }
      
      // Item 3/4: LED (always enabled)
      if (menuSelection == itemIndex++) {
        currentMenu = MENU_LED_TEST;
        return;
      }
      
      // Item 4/5: BadUSB (if enabled)
      #ifdef FEATURE_BADUSB
      if (menuSelection == itemIndex++) {
        currentMenu = MENU_BADUSB;
        return;
      }
      #endif
      
      // Item 5/6: Info/Settings (if RTC enabled)
      #ifdef FEATURE_RTC
      if (menuSelection == itemIndex++) {
        currentMenu = MENU_SETTINGS;
        return;
      }
      #endif
      
      // Last item: Sleep (always enabled)
      if (menuSelection == itemIndex++) {
        currentMenu = MENU_SLEEP;
        return;
      }
      
      // Default fallback
      currentMenu = MENU_MAIN_SCREEN;
    }
  }

  void handleDistanceMenu() {
    #ifdef FEATURE_DISTANCE_SENSOR
    char buf[12];
    uint16_t d = Sensors::getDistance();
    if (d > DISTANCE_MAX_RANGE) {
      Display::drawCentered("---");
    } else {
      snprintf(buf, sizeof(buf), "%dmm", d);
      Display::drawCentered(buf);
    }
    #else
    Display::drawCentered("N/A");
    #endif

    if (Buttons::getLastPressed() == Buttons::BTN_SELECT) {
      currentMenu = MENU_MAIN_MENU;
    }
  }

  void handleLaserMenu() {
    Display::drawCentered(Actuators::isLaserOn() ? "ON" : "OFF");
    
    Buttons::Button btn = Buttons::getLastPressed();
    if (btn == Buttons::BTN_DOWN || btn == Buttons::BTN_UP) {
      Actuators::laserToggle();
      resetTimeout();
    } else if (btn == Buttons::BTN_SELECT) {
      currentMenu = MENU_MAIN_MENU;
    }
  }

  void handleLEDTest() {
    static int colorIndex = 0;
    const char* colors[] = {"OFF", "RED", "GRN", "BLU", "YEL"};
    
    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_6x10_tf);
      u8g2.drawStr(0, 0, colors[colorIndex]);
      u8g2.drawStr(0, 20, "UP/DN");
      u8g2.drawStr(0, 35, "SEL:Back");
    } while (u8g2.nextPage());

    Buttons::Button btn = Buttons::getLastPressed();
    if (btn == Buttons::BTN_UP || btn == Buttons::BTN_DOWN) {
      colorIndex = (colorIndex + 1) % 5;
      resetTimeout();
      
      #ifdef FEATURE_LED
      switch (colorIndex) {
        case 0: Actuators::setLEDOff(); break;
        case 1: Actuators::setLEDRed(); break;
        case 2: Actuators::setLEDGreen(); break;
        case 3: Actuators::setLEDBlue(); break;
        case 4: Actuators::setLEDYellow(); break;
      }
      #endif
    } else if (btn == Buttons::BTN_SELECT) {
      currentMenu = MENU_MAIN_MENU;
      Actuators::setLEDOff();
    }
  }

  void handleBadUSB() {
    #ifdef FEATURE_BADUSB
    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_6x10_tf);
      u8g2.drawStr(0, 0, "BadUSB");
      u8g2.drawStr(0, 20, "UP/DN:Run");
      u8g2.drawStr(0, 35, "SEL:Back");
    } while (u8g2.nextPage());
    
    Buttons::Button btn = Buttons::getLastPressed();
    if (btn == Buttons::BTN_UP || btn == Buttons::BTN_DOWN) {
      Display::drawCentered("Run...");
      BadUSB::runBrowserScript();
      resetTimeout();
      delay(500);
    } else if (btn == Buttons::BTN_SELECT) {
      currentMenu = MENU_MAIN_MENU;
    }
    #else
    Display::drawCentered("N/A");
    if (Buttons::getLastPressed() == Buttons::BTN_SELECT) {
      currentMenu = MENU_MAIN_MENU;
    }
    #endif
  }

  void handleSettings() {
    #ifdef FEATURE_RTC
    char buf[12];
    RTCModule::getDateString(buf, sizeof(buf));
    Display::drawCentered(buf);
    #else
    Display::drawCentered("v1.0");
    #endif
    
    if (Buttons::getLastPressed() == Buttons::BTN_SELECT) {
      currentMenu = MENU_MAIN_MENU;
    }
  }

  void handleSleep() {
    static bool sleeping = false;
    
    if (!sleeping) {
      // Going to sleep
      sleeping = true;
      
      // Show sleep message
      u8g2.firstPage();
      do {
        u8g2.setFont(u8g2_font_6x10_tf);
        u8g2.drawStr(20, 25, "Sleep Mode");
        u8g2.drawStr(10, 40, "Press any btn");
      } while (u8g2.nextPage());
      
      delay(1000);
      
      // Turn off all components
      #ifdef FEATURE_LED
      Actuators::setLEDOff();
      #endif
      
      #ifdef FEATURE_LASER
      Actuators::laserOff();
      #endif
      
      // Turn off display
      Display::turnOff();
    }
    
    // Wait for any button to wake
    if (Buttons::getLastPressed() != Buttons::BTN_NONE) {
      // Wake up
      Display::turnOn();
      sleeping = false;
      currentMenu = MENU_MAIN_SCREEN;
      delay(100); // Debounce
    }
  }

  void update() {
    checkTimeout();

    switch (currentMenu) {
      case MENU_MAIN_SCREEN:
        handleMainScreen();
        break;
      case MENU_MAIN_MENU:
        handleMainMenu();
        break;
      case MENU_DISTANCE:
        handleDistanceMenu();
        break;
      case MENU_LASER:
        handleLaserMenu();
        break;
      case MENU_LED_TEST:
        handleLEDTest();
        break;
      case MENU_BADUSB:
        handleBadUSB();
        break;
      case MENU_SETTINGS:
        handleSettings();
        break;
      case MENU_SLEEP:
        handleSleep();
        break;
    }
  }
}

