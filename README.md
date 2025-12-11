# DStike Bad Watch

Custom software for the DStike Bad Watch - A versatile Arduino development watch with BadUSB capabilities.

## Overview

The DStike Bad Watch is an ATmega32U4-based development watch designed for Arduino beginners and enthusiasts. It can be recognized as a keyboard by computers, enabling BadUSB functionality for automation and testing purposes.

## Hardware Features

### Integrated Components

- **Microcontroller**: ATmega32U4 (Arduino Leonardo bootloader)
- **Display**: 1.3" OLED SH1106 (I2C interface)
- **Distance Sensor**: VL53L0X (0-1.2m range, I2C)
- **Real-Time Clock**: DS3231 (high accuracy, I2C)
- **Buzzer**: Digital audio output (boot sound only in Mauther)
- **NeoPixel RGB LED**: WS2812 addressable LED (full RGB color, 16.7M colors)
- **White LED**: Hardware-controlled flashlight (via power button long press)
- **Laser**: Built-in laser pointer
- **Battery**: 800mAh Li-ion with charging indicator (25%, 50%, 75%, 100%)

### Power Consumption

- **Operating Current**: 40-70mA (varies with code execution)
- **Battery Life**: Depends on usage and code optimization

### Power Switch Functions

- **Single Click**: Turn on
- **Double Click**: Turn off
- **Long Press**: Enable highlight LED

## Pin Configuration

| Component | Pin | Type |
|-----------|-----|------|
| Buzzer | Digital Pin 12 | Output |
| NeoPixel RGB LED | Digital Pin 6 | Output (WS2812 Data) |
| Laser | Digital Pin 13 | Output |
| OLED Display (SDA) | Digital Pin 3 | I2C |
| OLED Display (SCL) | Digital Pin 2 | I2C |
| Button Up | Digital Pin 10 | Input |
| Button Down | Digital Pin 8 | Input |
| Button Select | Digital Pin 9 | Input |

### Breakout GPIO Pins

The watch provides the following breakout pins for expansion:
- 5V
- 3V
- BAT
- GND
- GPIO5
- SDA
- SCL
- TXD
- RXD
- AD0

## Default Functionality

### Pre-flashed Demo Features

The watch comes pre-flashed with a demo program that displays:
- Current time
- Internal temperature (from DS3231 RTC sensor)
- Distance measurements

### Switch Operations

**Switch Up (Distance Sensor Mode)**
- Push the switch upwards to activate distance detection
- If distance < 1m:
  - Buzzer sounds "B~"
  - RGB LED turns RED
- Detection range: 0-1.2m

**Switch Down (Laser Mode)**
- Push the switch downwards to enable laser
- Push down again to disable laser

## Getting Started

### Prerequisites

- Arduino IDE (latest version recommended)
- Arduino Leonardo board support
- Required Libraries:
  - U8g2 (for OLED display)
  - VL53L0X (for distance sensor)
  - RTClib (for DS3231 RTC)
  - Adafruit NeoPixel (for RGB LED) ✨ **Important!**

### Installation

1. Clone this repository
```bash
git clone https://github.com/miazga/DStike-Watch.git
cd DStike-Watch
```

2. Install required Arduino libraries via Library Manager

3. Select **Arduino Leonardo** as the board in Arduino IDE

4. Connect the watch via USB and select the appropriate port

5. Upload your custom firmware

## Development

### BadUSB Capabilities

The ATmega32U4 can emulate a USB keyboard, enabling BadUSB functionality:

**Mauther Firmware Features:**
- 🖥️ **Cross-Platform Support**: Works on both macOS and Windows
- 🌐 **Browser Script**: Opens URLs in default browser automatically
- ⚙️ **OS-Specific Mode**: Configure target OS in `config.h`
  - macOS: Uses CMD+Space (Spotlight)
  - Windows: Uses WIN+R (Run dialog)
- 🎯 **Single Button Trigger**: Press UP/DOWN in BadUSB menu to execute

**Example Use Cases:**
- Automated testing scripts
- Computer automation tasks
- Security research and penetration testing
- Quick URL launching

**⚠️ Important**: Only use BadUSB features on systems you own or have explicit permission to test. Unauthorized use may be illegal in your jurisdiction.

**Configuration:**
```cpp
// In Mauther/config.h
#define BADUSB_TARGET_MAC        // For macOS
// #define BADUSB_TARGET_WINDOWS // For Windows
```

See [Mauther/BADUSB_OS_CONFIG.md](Mauther/BADUSB_OS_CONFIG.md) for detailed configuration.

### Example Code Structure

```cpp
// Basic structure for custom firmware
#include <U8g2lib.h>
#include <Wire.h>
#include <Keyboard.h>

void setup() {
  // Initialize components
  pinMode(12, OUTPUT); // Buzzer
  pinMode(6, OUTPUT);  // RGB LED
  pinMode(13, OUTPUT); // Laser
  pinMode(10, INPUT_PULLUP); // Button Up
  pinMode(8, INPUT_PULLUP);  // Button Down
  pinMode(9, INPUT_PULLUP);  // Button Select
  
  // Initialize I2C devices
  Wire.begin();
  // Your initialization code here
}

void loop() {
  // Your main code here
}
```

## Safety & Legal Notice

⚠️ **IMPORTANT**: This device can be used as a BadUSB attack tool. Please observe the following:

- Only use on systems you own or have written authorization to test
- Be aware of local laws regarding computer security tools
- This project is for educational purposes only
- The developers are not responsible for misuse of this device

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

## License

[Add your license here]

## Project Structure

```
DStike-Watch/
├── README.md           # Main project documentation
├── Mauther/           # Custom firmware for DStike Bad Watch
│   ├── Mauther.ino    # Main Arduino sketch
│   ├── config.h       # Configuration file
│   ├── display.h      # OLED display module
│   ├── sensors.h      # Distance sensor module
│   ├── actuators.h    # Buzzer, LED, Laser control
│   ├── buttons.h      # Button handling
│   ├── rtc_module.h   # Real-time clock module
│   ├── menu.h         # Menu system
│   ├── badusb.h       # BadUSB/Keyboard emulation
│   ├── README.md      # Mauther documentation
│   └── INSTALL.md     # Installation guide
├── Backup/            # Firmware backup tools
│   ├── README.md      # Backup/restore guide
│   ├── backup.sh      # Backup script
│   └── restore.sh     # Restore script
└── WiFiDuck/          # Reference WiFiDuck project
```

## Mauther Firmware

The **Mauther** directory contains custom firmware that provides:

### Features
- 📺 **OLED Display**: 1.3" SH1106 with clean UI, scrolling menus
- 📏 **Distance Sensor**: VL53L0X (0-1200mm range)
- ⏰ **Real-Time Clock**: DS3231 with temperature sensor
- 🔊 **Buzzer**: Boot sound only (silent operation)
- 🌈 **NeoPixel RGB LED**: WS2812 addressable LED
  - Boot test sequence (Red→Green→Blue→OFF)
  - Distance alarm indicator (turns red when object < 1m)
  - Manual color test mode (5 colors)
  - Automatic turn-off when returning to main screen
- 🔦 **Laser Pointer**: Toggle on/off control
- 🎮 **Three Buttons**: Up, Down, Select with debouncing
- 📋 **Menu System**: Dynamic, scrolling, timeout auto-return
- ⌨️ **BadUSB**: Cross-platform keyboard emulation (Mac/Windows)
  - Browser script opens URLs automatically
  - OS-specific configuration
  - Single button trigger
- 🎯 **Proximity Alarm**: Visual (LED) feedback for distance < 1m
- ⚡ **Optimized**: Fits in 28KB flash (99% usage)

### Quick Start

1. **Backup your current firmware**:
```bash
cd Backup
./backup.sh
```

2. **Install required Arduino libraries**:
   - U8g2 (OLED display)
   - VL53L0X (distance sensor)
   - RTClib (real-time clock)
   - Adafruit NeoPixel (RGB LED) ✨ **Don't forget this!**

3. **Flash Mauther firmware**:
   - Open `Mauther/Mauther.ino` in Arduino IDE
   - Select Board: Arduino Leonardo
   - Select your device port
   - Click Upload

4. **Done!** The watch will reboot with Mauther firmware

### Key Features Explained

#### NeoPixel RGB LED
- **Boot Test**: Red→Green→Blue flash on startup, then turns OFF
- **Distance Alarm**: Turns red when object detected < 1m
- **LED Test Menu**: Cycle through 5 colors (OFF, RED, GREEN, BLUE, YELLOW)
- **Smart Auto-Off**: LED turns off when exiting menus or returning to main screen
- **Full Color Support**: WS2812 addressable LED (16.7M colors possible)

#### BadUSB Functionality
- **Cross-Platform**: Configure for macOS or Windows in `config.h`
- **Browser Launch**: Opens URLs in default browser
- **Easy Trigger**: Navigate to BadUSB menu, press UP or DOWN button
- **macOS Mode**: Uses CMD+Space (Spotlight) to open URLs
- **Windows Mode**: Uses WIN+R (Run dialog) to open URLs
- **Customizable**: Edit `badusb.h` to change URL or behavior

### Documentation

- **Quick Start**: [Mauther/QUICK_START.md](Mauther/QUICK_START.md) - Fix compilation errors
- **Installation**: [Mauther/INSTALL.md](Mauther/INSTALL.md) - Complete setup guide
- **Development**: [Mauther/DEVELOPMENT.md](Mauther/DEVELOPMENT.md) - Development workflow
- **Optimization**: [Mauther/OPTIMIZATION.md](Mauther/OPTIMIZATION.md) - Code size optimization
- **BadUSB Config**: [Mauther/BADUSB_OS_CONFIG.md](Mauther/BADUSB_OS_CONFIG.md) - OS configuration guide
- **Full Docs**: [Mauther/README.md](Mauther/README.md) - Complete documentation

## Firmware Backup & Restore

Before flashing custom firmware, **always backup** your original firmware!

The `Backup` folder contains:
- Detailed backup/restore instructions
- Automated backup script (`backup.sh`)
- Automated restore script (`restore.sh`)
- Storage for firmware `.hex` files

See [Backup/README.md](Backup/README.md) for complete guide.

## Tools

The `Tools` folder contains utility sketches:

### SetRTC.ino - RTC Time Setter
Set the DS3231 real-time clock to a specific date and time:
1. Open `Tools/SetRTC.ino`
2. Edit the time (line 30)
3. Upload to watch
4. Upload Mauther firmware back

Time persists even after uploading new firmware (RTC has battery backup).

See [Tools/README.md](Tools/README.md) for detailed instructions.

## Credits

- DStike for the original hardware design
- Spacehuhn Technologies for WiFiDuck (BadUSB inspiration)
- Arduino community for libraries and support
- Mauther firmware by Marcin

## Resources

- [Arduino Leonardo Documentation](https://www.arduino.cc/en/Main/Arduino_BoardLeonardo)
- [ATmega32U4 Datasheet](http://www.atmel.com/Images/Atmel-7766-8-bit-AVR-ATmega16U4-32U4_Datasheet.pdf)

---

**Disclaimer**: This is an independent software project for the DStike Bad Watch hardware. Use responsibly and ethically.

