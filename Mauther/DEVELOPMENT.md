# Development Guide for Mauther

Guide for developing and modifying Mauther firmware.

## Development Environment

### Required Tools
- Arduino IDE 1.8.x or higher
- Serial Monitor for debugging
- Text editor (or use Arduino IDE's built-in editor)
- Git (for version control)

### Required Libraries
- U8g2 (OLED display)
- VL53L0X (distance sensor)
- RTClib (RTC)
- Keyboard (built-in)
- Wire (built-in)

## Initial Setup

### 1. First-Time Upload

After backing up original firmware:

```bash
# Open project
cd /Users/marcin/dev/deauther/Mauther
open Mauther.ino  # Opens in Arduino IDE
```

**Arduino IDE Setup**:
1. Tools → Board → Arduino Leonardo
2. Tools → Port → (select your device)
3. Click Upload (→)
4. **Press RESET when upload hangs** (first time only)

### 2. Subsequent Uploads

After the first successful upload, Arduino IDE usually handles bootloader timing automatically. You typically won't need to press RESET for regular development uploads.

**If upload fails**:
- Press RESET button when IDE shows "Uploading..."
- Wait for bootloader to activate
- IDE continues automatically

## Development Workflow

### Quick Iteration Loop

For rapid development:

1. **Edit code** in Arduino IDE or your favorite editor
2. **Save** (Ctrl/Cmd + S)
3. **Upload** (Ctrl/Cmd + U)
   - Arduino IDE compiles and uploads automatically
   - No manual RESET needed (usually)
4. **Watch reboots** with new code
5. **Test** changes immediately

### Using Serial Monitor for Debugging

Enable debug mode in `config.h`:

```cpp
#define DEBUG_MODE  // Uncomment this line
```

**View debug output**:
1. Tools → Serial Monitor (or Ctrl/Cmd + Shift + M)
2. Set baud rate to **115200**
3. Upload firmware
4. Watch serial output:
   ```
   Mauther v1.0
   Initializing...
   Initializing VL53L0X...
   VL53L0X initialized
   Buttons initialized
   Initialization complete!
   ```

**Debug tips**:
- Add `Serial.println()` statements for debugging
- Use `Serial.print()` to output variable values
- Debug output doesn't slow down the watch significantly

### Project Structure

```
Mauther/
├── Mauther.ino      # Main entry point (setup/loop)
├── config.h         # Configuration & pin definitions
├── display.h        # OLED display functions
├── sensors.h        # VL53L0X distance sensor
├── actuators.h      # Buzzer, LED, Laser
├── buttons.h        # Button handling
├── rtc_module.h     # DS3231 RTC
├── menu.h           # Menu system
└── badusb.h         # Keyboard emulation
```

**Module philosophy**:
- Each `.h` file is a self-contained module
- Uses namespaces to avoid conflicts
- Include only what you need

## Common Development Tasks

### Adding a New Menu Item

**1. Update config.h**:
```cpp
#define MENU_ITEMS 7  // Increase count
```

**2. Edit menu.h**:
```cpp
// Add to menu items array
const char* mainMenuItems[] = {
    "Back to Main",
    // ... existing items
    "My New Feature"  // Add here
};

// Add menu state
enum MenuState {
    // ... existing states
    MENU_MY_FEATURE
};

// Add handler function
void handleMyFeature() {
    Display::drawCentered("My Feature Active!");
    
    Buttons::Button btn = Buttons::getLastPressed();
    if (btn == Buttons::BTN_SELECT) {
        currentMenu = MENU_MAIN_MENU;
        Actuators::beep(BUZZER_BEEP_FREQ, 50);
    }
    
    resetTimeout();
}

// Add to update() switch
void update() {
    checkTimeout();
    
    switch (currentMenu) {
        // ... existing cases
        case MENU_MY_FEATURE:
            handleMyFeature();
            break;
    }
}
```

**3. Upload and test**!

### Adding a New Sensor

**1. Create new module** `my_sensor.h`:
```cpp
#pragma once
#include <Arduino.h>
#include "config.h"

namespace MySensor {
    int sensorValue = 0;
    
    void begin() {
        pinMode(SENSOR_PIN, INPUT);
        #ifdef DEBUG_MODE
        Serial.println(F("MySensor initialized"));
        #endif
    }
    
    void update() {
        sensorValue = analogRead(SENSOR_PIN);
    }
    
    int getValue() {
        return sensorValue;
    }
}
```

**2. Include in main** `Mauther.ino`:
```cpp
#include "my_sensor.h"

void setup() {
    // ... existing code
    MySensor::begin();
}

void loop() {
    // ... existing code
    MySensor::update();
}
```

### Modifying Display Layout

Edit `display.h`:

```cpp
void drawMainScreen(const char* time, float temp, uint16_t distance, bool laserOn) {
    u8g2.clearBuffer();
    
    // Customize layout here
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(0, 20, time);
    
    // Add your elements
    
    u8g2.sendBuffer();
}
```

**Display tips**:
- Screen size: 128x64 pixels
- Fonts available: See U8g2 documentation
- Use `u8g2.getStrWidth()` to center text
- Call `u8g2.clearBuffer()` before drawing
- Call `u8g2.sendBuffer()` when done

### Adding BadUSB Scripts

Edit `badusb.h`:

```cpp
void runMyCustomScript() {
    #ifdef DEBUG_MODE
    Serial.println(F("Running custom script..."));
    #endif

    isRunning = true;
    
    cmdDelay(1000);           // Wait 1 second
    cmdGui('r');               // Windows + R
    cmdDelay(500);
    cmdString("notepad");      // Type "notepad"
    cmdEnter();                // Press Enter
    cmdDelay(1000);
    cmdString("Hello World!"); // Type text
    
    isRunning = false;
}
```

**Call from menu**:
```cpp
void handleBadUSB() {
    Display::drawCentered("BadUSB Ready");
    
    Buttons::Button btn = Buttons::getLastPressed();
    if (btn == Buttons::BTN_UP) {
        BadUSB::runMyCustomScript();
    }
    // ... rest of handler
}
```

## Performance Optimization

### Memory Management

ATmega32U4 has limited RAM (2.5KB):

**Check memory usage**:
```
Sketch uses 18234 bytes (63%) of program storage space.
Global variables use 723 bytes (28%) of dynamic memory.  <-- Keep this low!
```

**Memory tips**:
- Use `F()` macro for strings: `Serial.println(F("Text"))`
- Avoid large arrays
- Use `const` for constants
- Minimize global variables

### CPU Optimization

**Tips**:
- Avoid `delay()` in production code (use millis() instead)
- Update sensors at appropriate intervals (not every loop)
- Don't poll buttons too frequently
- Menu timeout prevents unnecessary updates

### Flash Memory

ATmega32U4 has 32KB flash:

**If you run out of space**:
- Remove unused locale files from badusb.h
- Simplify display graphics
- Remove debug code in production builds
- Use smaller fonts

## Testing

### Hardware Tests

**Distance Sensor**:
- Navigate to "Distance Sensor" menu
- Move hand 0-120cm from watch
- Verify readings change
- Test alarm at <100cm

**Buttons**:
- Test all three buttons
- Test long press (hold >1 second)
- Test in all menu screens

**Display**:
- Check all menu screens render correctly
- Verify text doesn't overflow
- Test contrast in different lighting

**RTC**:
- Verify time displays correctly
- Check temperature reading

### Software Tests

**Serial Monitor**:
```cpp
#define DEBUG_MODE
```
- Monitor for errors
- Check initialization sequence
- Verify sensor readings

**BadUSB**:
- Test on YOUR OWN computer only
- Start with simple scripts
- Verify keyboard emulation works
- Test different key combinations

## Troubleshooting Development Issues

### Compilation Errors

**Missing library**:
```
fatal error: U8g2lib.h: No such file or directory
```
- Install missing library via Library Manager

**Syntax error**:
- Check for missing semicolons
- Verify bracket matching
- Check namespace usage

### Upload Issues

**Port disappears**:
- Normal after pressing RESET
- Arduino IDE handles automatically
- If stuck, press RESET when "Uploading..." appears

**Verification failed**:
- Upload again (sometimes transient)
- Try different USB port/cable
- Check battery level

### Runtime Issues

**Display blank**:
- Check I2C connections
- Verify display address (0x3C)
- Enable debug mode to see serial output

**Buttons not responding**:
- Check debounce timing
- Verify INPUT_PULLUP mode
- Test with serial debug output

**Sensor not working**:
- Check I2C address conflicts
- Verify sensor initialized (serial output)
- Test sensor with example sketch

## Version Control

### Git Setup

```bash
cd /Users/marcin/dev/deauther
git init
git add Mauther/
git commit -m "Initial Mauther firmware"
```

### Branching for Features

```bash
git checkout -b feature/my-new-feature
# Make changes
git add -A
git commit -m "Add new feature"
git checkout main
git merge feature/my-new-feature
```

### Backup Development Versions

```bash
# Tag important versions
git tag -a v1.0.0 -m "First stable release"
git tag -a v1.1.0 -m "Added new sensor support"
```

## Best Practices

### Code Style

- Use descriptive variable names
- Comment complex logic
- Keep functions short and focused
- Use namespaces for modules

### Safety

- Always test on YOUR OWN devices first
- Don't deploy BadUSB scripts without testing
- Keep backup firmware safe
- Document changes

### Performance

- Update sensors only when needed
- Use appropriate delays
- Monitor memory usage
- Profile with serial output

## Resources

- [Arduino Leonardo Reference](https://www.arduino.cc/en/Reference/HomePage)
- [U8g2 Documentation](https://github.com/olikraus/u8g2/wiki)
- [VL53L0X Library](https://github.com/pololu/vl53l0x-arduino)
- [RTClib Documentation](https://adafruit.github.io/RTClib/html/index.html)

## Getting Help

- Check serial monitor for debug output
- Review existing module code for examples
- Test individual components separately
- Use Arduino IDE's example sketches for libraries

---

Happy developing! 🚀

