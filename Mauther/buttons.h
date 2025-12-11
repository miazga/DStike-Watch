/*
 * Buttons module - Handles button inputs with debouncing
 */

#pragma once
#include <Arduino.h>
#include "config.h"

namespace Buttons {
  enum Button {
    BTN_NONE,
    BTN_UP,
    BTN_DOWN,
    BTN_SELECT
  };

  enum ButtonEvent {
    EVT_NONE,
    EVT_PRESS,
    EVT_RELEASE,
    EVT_LONG_PRESS
  };

  struct ButtonState {
    uint8_t pin;
    bool lastState;
    bool currentState;
    unsigned long pressTime;
    bool longPressTriggered;
  };

  ButtonState buttons[3] = {
    {PIN_BUTTON_UP, HIGH, HIGH, 0, false},
    {PIN_BUTTON_DOWN, HIGH, HIGH, 0, false},
    {PIN_BUTTON_SEL, HIGH, HIGH, 0, false}
  };

  Button lastPressed = BTN_NONE;
  ButtonEvent lastEvent = EVT_NONE;

  void begin() {
    pinMode(PIN_BUTTON_UP, INPUT_PULLUP);
    pinMode(PIN_BUTTON_DOWN, INPUT_PULLUP);
    pinMode(PIN_BUTTON_SEL, INPUT_PULLUP);
  }

  void update() {
    unsigned long now = millis();
    
    for (int i = 0; i < 3; i++) {
      bool reading = digitalRead(buttons[i].pin);
      
      // Only update if state is stable (debouncing)
      if (reading != buttons[i].lastState) {
        delay(BUTTON_DEBOUNCE_MS);  // Simple debounce delay
        reading = digitalRead(buttons[i].pin);
      }
      
      if (reading == LOW && buttons[i].lastState == HIGH) {
        buttons[i].currentState = LOW;
        buttons[i].pressTime = now;
        buttons[i].longPressTriggered = false;
      }
      else if (reading == HIGH && buttons[i].lastState == LOW) {
        buttons[i].currentState = HIGH;
        unsigned long pressDuration = now - buttons[i].pressTime;
        
        if (pressDuration > BUTTON_LONG_PRESS_MS) {
          lastPressed = (Button)(i + 1);
          lastEvent = EVT_LONG_PRESS;
        } else if (pressDuration > BUTTON_DEBOUNCE_MS) {
          lastPressed = (Button)(i + 1);
          lastEvent = EVT_RELEASE;
        }
      }
      
      buttons[i].lastState = reading;
    }
  }

  Button getLastPressed() {
    Button btn = lastPressed;
    lastPressed = BTN_NONE;
    return btn;
  }

  ButtonEvent getLastEvent() {
    ButtonEvent evt = lastEvent;
    lastEvent = EVT_NONE;
    return evt;
  }

  bool isPressed(Button btn) {
    if (btn == BTN_NONE || btn > BTN_SELECT) return false;
    return buttons[btn - 1].currentState == LOW;
  }
}

