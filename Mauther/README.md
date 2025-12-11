# Mauther - Custom Firmware for DStike Bad Watch

A feature-rich custom firmware for the DStike Bad Watch with full hardware support and BadUSB capabilities.

## Features

### Hardware Support
- ✅ **1.3" OLED Display (SH1106)** - Beautiful menu system and information display
- ✅ **VL53L0X Distance Sensor** - 0-1.2m range with proximity alarm
- ✅ **DS3231 RTC** - Accurate timekeeping with temperature sensor
- ✅ **Buzzer** - Boot sound only (silent operation)
- ✅ **RGB NeoPixel LED (Pin 6)** - Full RGB color control, 8 colors, status indicators
- ✅ **White LED** - Highlight/flashlight (hardware controlled via power button)
- ✅ **Laser Pointer** - Toggle on/off control
- ✅ **Three Buttons** - Full menu navigation (Up, Down, Select)
- ✅ **Power Button** - Device power and white LED control

### Software Features
- 🎯 Real-time clock display with date
- 📏 Distance measurement with visual and audio alarm (< 1m)
- 💡 Laser control from main screen and menu
- 🌈 LED color testing
- ⌨️ BadUSB keyboard emulation
- 🎨 Clean, intuitive menu system
- ⏰ Auto-return to main screen after inactivity
- 🔊 Audio feedback for all interactions

## Hardware Configuration

### Pin Mapping
```
Buzzer      → Digital Pin 12
RGB LED     → Digital Pin 6 (PWM)
Laser       → Digital Pin 13
OLED SDA    → Digital Pin 3 (Software I2C)
OLED SCL    → Digital Pin 2 (Software I2C)
Button Up   → Digital Pin 10 (INPUT_PULLUP)
Button Down → Digital Pin 8 (INPUT_PULLUP)
Button Sel  → Digital Pin 9 (INPUT_PULLUP)
```

### I2C Devices
```
OLED Display    → 0x3C
VL53L0X Sensor  → 0x29
DS3231 RTC      → 0x68
```

## Installation

### Prerequisites

1. **Arduino IDE** (1.8.x or higher)
2. **Board Support**: Arduino Leonardo
3. **Required Libraries**:
   - U8g2 (for OLED display)
   - VL53L0X (distance sensor)
   - RTClib (real-time clock)
   - Adafruit_NeoPixel (RGB LED) ✨ **Important!**
   - Keyboard (built-in)
   - Wire (built-in)

### Installing Libraries

Via Arduino Library Manager:
```
Sketch → Include Library → Manage Libraries
```

Search and install:
- `U8g2` by oliver
- `VL53L0X` by Pololu
- `RTClib` by Adafruit
- `Adafruit NeoPixel` by Adafruit ✨ **Required for RGB LED!**

### Flashing Firmware

1. **Backup Original Firmware** (IMPORTANT!)
   ```bash
   cd ../Backup
   ./backup.sh
   ```

2. **Open Mauther.ino** in Arduino IDE

3. **Configure Arduino IDE**:
   - Board: `Arduino Leonardo`
   - Port: Select your DStike Bad Watch port

4. **Compile & Upload**:
   - Click `Verify` to check for errors
   - Click `Upload` to flash

5. **Wait for upload** to complete (~30 seconds)

6. **Done!** The watch will reboot automatically

## Usage

### Main Screen

The main screen displays:
- Project name (MAUTHER)
- Current time (HH:MM:SS)
- Temperature from RTC (°C)
- Distance measurement (mm)
- Laser status indicator

### Button Controls

#### Main Screen
- **SELECT**: Enter main menu
- **DOWN**: Toggle laser on/off
- **UP**: (Reserved for future use)

#### In Menus
- **UP**: Navigate up / Previous option
- **DOWN**: Navigate down / Next option
- **SELECT**: Confirm selection / Go back

### Menu System

```
Main Menu
├── Back to Main (return to main screen)
├── Distance Sensor (view real-time distance)
├── Laser Control (toggle laser)
├── LED Test (cycle through LED colors)
├── BadUSB (BadUSB script execution)
└── Settings (view date and settings)
```

### Distance Sensor Mode

- Automatically activated on main screen
- Range: 0-1200mm (0-1.2m)
- Alarm triggers when object < 1000mm (1m)
- When alarm triggers:
  - Buzzer sounds continuous tone
  - RGB LED turns RED
  - Alarm stops when distance > 1m

### Laser Control

- **From Main Screen**: Press DOWN button
- **From Menu**: Navigate to "Laser Control"
- Toggle between ON/OFF states
- Visual confirmation on display

### LED Test Mode

The watch features a **NeoPixel WS2812** RGB LED with full color support!

Cycle through 8 colors:
- OFF
- RED
- GREEN
- BLUE
- YELLOW
- CYAN
- MAGENTA
- WHITE

**Features:**
- True RGB color mixing (16.7 million colors possible)
- Brightness control (default 20% for battery life)
- Boot test sequence (Red → Green → Blue on startup)
- Used for distance alarm (turns red when object < 1m)

### BadUSB Mode

Execute keyboard emulation scripts for automation:

**Example Scripts Included**:
- Test Script: Opens Notepad and types text
- Calculator Script: Opens Windows calculator

**Safety**: Use only on systems you own!

## Configuration

Edit `config.h` to customize:

```cpp
// Debug mode
#define DEBUG_MODE  // Uncomment for serial debugging

// Distance alarm threshold
#define DISTANCE_ALARM_THRESHOLD 1000  // mm

// Menu timeout
#define MENU_TIMEOUT_MS 30000  // 30 seconds

// Buzzer frequencies
#define BUZZER_ALARM_FREQ 2000  // Hz
#define BUZZER_BEEP_FREQ  1000  // Hz
```

## Development

### Project Structure

```
Mauther/
├── Mauther.ino      # Main program
├── config.h         # Configuration & pin definitions
├── display.h        # OLED display functions
├── sensors.h        # VL53L0X distance sensor
├── actuators.h      # Buzzer, LED, Laser control
├── buttons.h        # Button handling & debouncing
├── rtc_module.h     # DS3231 RTC functions
├── menu.h           # Menu system & navigation
├── badusb.h         # Keyboard emulation & scripts
└── README.md        # This file
```

### Adding Custom Functions

#### Example: Add New Menu Item

1. Edit `config.h`:
```cpp
#define MENU_ITEMS 7  // Increase count
```

2. Edit `menu.h`:
```cpp
const char* mainMenuItems[] = {
    "Back to Main",
    // ... existing items
    "My New Feature"  // Add new item
};

// Add new menu state
enum MenuState {
    // ... existing states
    MENU_MY_FEATURE
};

// Add handler function
void handleMyFeature() {
    Display::drawCentered("My Feature!");
    // Your code here
}

// Add to update() switch statement
case MENU_MY_FEATURE:
    handleMyFeature();
    break;
```

### Adding BadUSB Scripts

Edit `badusb.h` to add new scripts:

```cpp
void runMyScript() {
    isRunning = true;
    
    cmdDelay(1000);
    cmdGui('r');  // Windows + R
    cmdDelay(500);
    cmdString("cmd");
    cmdEnter();
    // Add your commands here
    
    isRunning = false;
}
```

### Debug Mode

Enable debug output via Serial:

1. Uncomment in `config.h`:
```cpp
#define DEBUG_MODE
```

2. Open Serial Monitor (115200 baud)

3. View debug messages during operation

## Troubleshooting

### Display Not Working
- Check I2C address (default 0x3C)
- Verify SDA/SCL pins (3, 2)
- Test with I2C scanner sketch

### Distance Sensor Not Responding
- Check I2C connection
- Verify VL53L0X library installed
- Check for I2C address conflicts

### RTC Time Lost
- Replace CR2032 battery in DS3231 module
- Time will reset to compile time on first boot

### Buttons Not Responding
- Check INPUT_PULLUP configuration
- Verify pin connections (10, 8, 9)
- Adjust BUTTON_DEBOUNCE_MS if needed

### Laser Not Working
- Verify Pin 13 connection
- Check power supply (laser needs sufficient current)

### Upload Failed
- Press reset button on watch
- Select correct port in Arduino IDE
- Check USB cable quality

## Safety & Legal

⚠️ **BadUSB Functionality**: This device can emulate a keyboard. Use responsibly:
- Only on systems you own or have permission to test
- Be aware of local laws regarding security tools
- This project is for educational purposes
- Developers are not responsible for misuse

⚠️ **Laser Safety**: 
- Do not point at eyes or reflective surfaces
- Keep away from children
- Comply with local laser safety regulations

## Performance

- **Power Consumption**: 40-70mA (depends on active features)
- **Battery Life**: ~11-20 hours (800mAh battery)
- **Display Update**: ~10 FPS
- **Distance Update**: 10Hz

## Credits

- **Original Hardware**: DStike
- **BadUSB Inspiration**: WiFiDuck project by Spacehuhn Technologies
- **Libraries**: U8g2 (oliver), VL53L0X (Pololu), RTClib (Adafruit)
- **Firmware**: Mauther (Custom development)

## License

This project is provided as-is for educational purposes. Use responsibly.

Original hardware by DStike. This is an independent firmware project.

## Support

For issues, questions, or contributions:
- Check the main project README
- Review WiFiDuck documentation for BadUSB features
- Consult Arduino Leonardo documentation for hardware details

---

**Made with ❤️ for the DStike Bad Watch community**

Version 1.0.0

