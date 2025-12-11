/*
 * Sensors module - Handles VL53L0X distance sensor
 * Only include if FEATURE_DISTANCE_SENSOR is defined
 */

#pragma once

#ifdef FEATURE_DISTANCE_SENSOR

#include <Arduino.h>
#include <Wire.h>
#include <VL53L0X.h>
#include "config.h"

namespace Sensors {
  VL53L0X distanceSensor;
  uint16_t lastDistance = 0;
  bool distanceSensorAvailable = false;
  unsigned long lastUpdate = 0;

  void begin() {
    Wire.begin();
    distanceSensor.setTimeout(500);
    if (distanceSensor.init()) {
      distanceSensorAvailable = true;
      distanceSensor.startContinuous();
    }
  }

  uint16_t getDistance() {
    return lastDistance;
  }

  bool isDistanceSensorAvailable() {
    return distanceSensorAvailable;
  }

  bool isAlarmTriggered() {
    // Use hysteresis to prevent rapid on/off toggling
    // Trigger at DISTANCE_ALARM_THRESHOLD, clear at DISTANCE_ALARM_CLEAR
    static bool alarmState = false;
    
    if (!alarmState && lastDistance > 0 && lastDistance < DISTANCE_ALARM_THRESHOLD) {
      // Enter alarm zone
      alarmState = true;
    } else if (alarmState && lastDistance > DISTANCE_ALARM_CLEAR) {
      // Exit alarm zone (with hysteresis)
      alarmState = false;
    }
    
    return alarmState;
  }

  void update() {
    if (!distanceSensorAvailable) return;
    
    if (millis() - lastUpdate > 100) {
      lastDistance = distanceSensor.readRangeContinuousMillimeters();
      if (distanceSensor.timeoutOccurred()) {
        lastDistance = DISTANCE_MAX_RANGE + 1;
      }
      lastUpdate = millis();
    }
  }
}

#endif // FEATURE_DISTANCE_SENSOR

