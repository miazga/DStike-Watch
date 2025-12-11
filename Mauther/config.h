/*
 * Configuration file for Mauther
 */

#pragma once

// ===== Version =====
#define VERSION "1.0.0"
#define PROJECT_NAME "Mauther"

// ===== Debug Mode =====
// #define DEBUG_MODE  // Uncomment ONLY for development (costs ~1KB)
// Keep disabled for production to save space!

// ===== Optional Features (comment out to save space) =====
// Enable only what you need to fit in 28KB flash:
#define FEATURE_DISTANCE_SENSOR  // VL53L0X support (~2KB)
#define FEATURE_RTC              // DS3231 RTC support (~1KB)
#define FEATURE_BADUSB           // BadUSB keyboard emulation (~2KB)
#define FEATURE_BUZZER           // Buzzer - ONLY for boot sound
#define FEATURE_LED              // RGB LED control
#define FEATURE_LASER            // Laser pointer control

// Note: Buzzer only plays on device startup, all other sounds disabled

// ===== BadUSB Target OS =====
// Choose target OS for BadUSB scripts (only one should be defined)
#define BADUSB_TARGET_MAC        // macOS (CMD+Space → Spotlight)
// #define BADUSB_TARGET_WINDOWS // Windows (WIN+R → Run dialog)

// ===== Pin Definitions =====
#define PIN_BUZZER      12
#define PIN_RGB_LED     6    // Multicolor LED (software controlled)
// Note: White LED is hardware controlled by power button:
//   - Long press (>2s): Turn on white LED
//   - Double click: Turn off device
#define PIN_LASER       13
#define PIN_OLED_SDA    3
#define PIN_OLED_SCL    2
#define PIN_BUTTON_UP   10
#define PIN_BUTTON_DOWN 8
#define PIN_BUTTON_SEL  9

// ===== I2C Addresses =====
#define OLED_I2C_ADDR   0x3C
#define VL53L0X_ADDR    0x29
#define DS3231_ADDR     0x68

// ===== Display Settings =====
#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64
#define DISPLAY_UPDATE_MS 100

// ===== Distance Sensor Settings =====
#define DISTANCE_ALARM_THRESHOLD 1000  // mm (1 meter) - trigger alarm
#define DISTANCE_ALARM_CLEAR     1100  // mm - clear alarm (hysteresis to prevent buzzing)
#define DISTANCE_MAX_RANGE       1200  // mm (1.2 meters)

// ===== Button Settings =====
#define BUTTON_DEBOUNCE_MS 20   // Reduced for faster response
#define BUTTON_LONG_PRESS_MS 800  // Reduced from 1000ms

// ===== Buzzer Settings =====
#define BUZZER_ALARM_FREQ 1500  // Hz (reduced from 2000 - less annoying)
#define BUZZER_BEEP_FREQ  800   // Hz (reduced from 1000 - softer)
#define BUZZER_BOOT_FREQ  400   // Hz (reduced from 500 - quieter)
#define BUZZER_ALARM_DURATION 200  // ms - short beep duration
#define BUZZER_ALARM_INTERVAL 1000 // ms - time between alarm beeps

// ===== RGB LED Colors =====
#define LED_OFF         0, 0, 0
#define LED_RED         255, 0, 0
#define LED_GREEN       0, 255, 0
#define LED_BLUE        0, 0, 255
#define LED_YELLOW      255, 255, 0
#define LED_CYAN        0, 255, 255
#define LED_MAGENTA     255, 0, 255
#define LED_WHITE       255, 255, 255

// ===== Menu Settings =====
// Calculate menu items based on enabled features
#define MENU_ITEMS_BASE 3  // Back, Laser, LED
#ifdef FEATURE_DISTANCE_SENSOR
  #define MENU_ITEMS_DIST 1
#else
  #define MENU_ITEMS_DIST 0
#endif
#ifdef FEATURE_BADUSB
  #define MENU_ITEMS_BADUSB 1
#else
  #define MENU_ITEMS_BADUSB 0
#endif
#ifdef FEATURE_RTC
  #define MENU_ITEMS_RTC 1
#else
  #define MENU_ITEMS_RTC 0
#endif
#define MENU_ITEMS (MENU_ITEMS_BASE + MENU_ITEMS_DIST + MENU_ITEMS_BADUSB + MENU_ITEMS_RTC + 1)  // +1 for Sleep

#define MENU_TIMEOUT_MS 30000  // Return to main screen after 30s

// ===== BadUSB Settings =====
#define MAX_SCRIPT_SIZE 2048
#define DEFAULT_DELAY_MS 5

