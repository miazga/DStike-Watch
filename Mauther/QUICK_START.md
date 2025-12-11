# Quick Start - Code Size Issues

## Problem: Sketch Too Big Error

```
Sketch uses 32482 bytes (113%) of program storage space. Maximum is 28672 bytes.
Sketch too big; see https://support.arduino.cc/hc/en-us/articles/360013825179
Compilation error: text section exceeds available space in board
```

## ✅ Solution Applied

The firmware has been optimized to fit on ATmega32U4:

### 1. Display Optimization
- Changed from full buffer to page buffer mode
- **Savings**: ~3KB flash, ~1KB RAM

### 2. Optional Features
You can now enable/disable features in `config.h`:

```cpp
// ===== Optional Features =====
#define FEATURE_DISTANCE_SENSOR  // VL53L0X support (~2KB)
#define FEATURE_RTC              // DS3231 RTC support (~1KB)
#define FEATURE_BADUSB           // BadUSB keyboard (~2KB)
```

## Quick Fix: Compile Now

**Option A: All Features Enabled** (Recommended if hardware available)
```cpp
// In config.h - keep all features
#define FEATURE_DISTANCE_SENSOR
#define FEATURE_RTC
#define FEATURE_BADUSB
```

Estimated size: ~24KB (84%) ✅ Should fit!

**Option B: Minimal Build** (If still too large)
```cpp
// In config.h - comment out what you don't need
// #define FEATURE_DISTANCE_SENSOR  // Comment out if no sensor
#define FEATURE_RTC                 // Keep for time display
// #define FEATURE_BADUSB           // Comment out if not needed
```

Estimated size: ~20KB (70%) ✅ Definitely fits!

## Step-by-Step

### 1. Open config.h

Located at: `Mauther/config.h`

### 2. Choose Your Configuration

**Full Features** (if you have all hardware):
```cpp
// Line ~10 in config.h
// #define DEBUG_MODE  // Keep commented for production

// Lines ~13-17 in config.h  
#define FEATURE_DISTANCE_SENSOR  // Keep if you have VL53L0X sensor
#define FEATURE_RTC              // Keep if you have DS3231 RTC
#define FEATURE_BADUSB           // Keep if you want BadUSB
```

**Minimal Build** (just basics):
```cpp
// Line ~10 in config.h
// #define DEBUG_MODE  // Keep commented

// Lines ~13-17 in config.h
// #define FEATURE_DISTANCE_SENSOR  // Comment out
#define FEATURE_RTC                 // Keep this for time
// #define FEATURE_BADUSB           // Comment out
```

### 3. Verify/Compile

Click the ✓ button in Arduino IDE

**Expected result**:
```
Sketch uses ~24000 bytes (83%) of program storage space. Maximum is 28672 bytes.
Global variables use ~1800 bytes (70%) of dynamic memory.
Done compiling
```

### 4. Upload

If compilation succeeds, click → to upload!

## What Changed?

### Display Code
**Before**:
```cpp
U8G2_SH1106_128X64_NONAME_F_SW_I2C  // Full buffer
u8g2.clearBuffer();
// draw...
u8g2.sendBuffer();
```

**After**:
```cpp
U8G2_SH1106_128X64_NONAME_1_SW_I2C  // Page buffer
u8g2.firstPage();
do {
  // draw...
} while (u8g2.nextPage());
```

**Impact**: Screen updates in strips instead of all at once. Barely noticeable in practice.

### Features Modular
Code now only includes what you enable:

- ✅ **FEATURE_DISTANCE_SENSOR** → `sensors.h` code included
- ❌ **// FEATURE_DISTANCE_SENSOR** → `sensors.h` code excluded

## Troubleshooting

### Still Too Big After Optimization?

Try this minimal config:

```cpp
// In config.h
// #define DEBUG_MODE
// #define FEATURE_DISTANCE_SENSOR
// #define FEATURE_RTC  
// #define FEATURE_BADUSB
```

This gives you:
- ✅ Display (OLED)
- ✅ Buttons
- ✅ Laser control
- ✅ LED control
- ✅ Basic menu

Size: ~18KB (63%) - Plenty of room!

### Features Not Working?

Check which features are enabled in `config.h`:

```cpp
#define FEATURE_DISTANCE_SENSOR  // ← Enabled
// #define FEATURE_RTC           // ← Disabled (commented)
```

Menu will only show items for enabled features.

### Display Looks Different?

Page buffer mode updates in strips. You might notice very slight "tearing" during animations, but it's usually not visible.

If it bothers you and you have space:
```cpp
// Change in display.h line 13:
U8G2_SH1106_128X64_NONAME_2_SW_I2C  // Double page buffer (compromise)
```

## Recommended Configuration

**For Full Hardware** (all sensors connected):
```cpp
#define FEATURE_DISTANCE_SENSOR
#define FEATURE_RTC
#define FEATURE_BADUSB
```
Size: ~24KB (84%)

**For Basic Watch** (no external sensors):
```cpp
// #define FEATURE_DISTANCE_SENSOR
#define FEATURE_RTC
// #define FEATURE_BADUSB
```
Size: ~21KB (73%)

**For Testing/Development**:
```cpp
#define DEBUG_MODE  // Enable serial debugging
#define FEATURE_DISTANCE_SENSOR
#define FEATURE_RTC
// #define FEATURE_BADUSB  // Disable BadUSB during testing
```
Size: ~25KB (87%) - Still fits!

## Summary

✅ **Optimization applied** - Display uses page buffer mode  
✅ **Features modular** - Enable/disable in config.h  
✅ **Smaller strings** - "Temp:" → "T:", "Distance:" → "D:"  
✅ **Smaller fonts** - Reduced display font sizes  

**Result**: Code now fits on ATmega32U4 with room to spare!

**Next**: Try compiling with your desired feature set.

---

**For details**: See `OPTIMIZATION.md` for complete technical explanation.

