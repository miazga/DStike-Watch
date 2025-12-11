# Tools for DStike Bad Watch

This folder contains utility sketches for managing your watch.

## SetRTC.ino - RTC Time Setter

### Purpose
Sets the DS3231 Real-Time Clock to a specific date and time.

### When to Use
- When you want to set a precise time
- When your RTC time is incorrect
- When setting up the watch for the first time

### Instructions

1. **Open the sketch**
   ```
   Arduino IDE → File → Open → Tools/SetRTC.ino
   ```

2. **Edit the time** (around line 30)
   ```cpp
   rtc.adjust(DateTime(2025, 12, 11, 18, 30, 0));
   //              Year  Mon  Day  Hr  Min  Sec
   ```
   
   Change to your desired date/time:
   - Year: 2025
   - Month: 1-12 (1=January, 12=December)
   - Day: 1-31
   - Hour: 0-23 (24-hour format)
   - Minute: 0-59
   - Second: 0-59

3. **Select board and port**
   - Board: Arduino Leonardo
   - Port: Your watch's USB port

4. **Upload the sketch**
   - Click **Upload** (→)
   - Wait for upload to complete

5. **Open Serial Monitor** (optional)
   - Tools → Serial Monitor (115200 baud)
   - You'll see the time being set and current time displayed

6. **Upload Mauther firmware back**
   - Open `Mauther/Mauther.ino`
   - Click **Upload**
   - Your watch is back to normal with correct time!

### Alternative: Set to Computer Time

Instead of manually entering the time, you can set it to your computer's current time:

**Uncomment this line:**
```cpp
rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
```

**Comment out the manual line:**
```cpp
// rtc.adjust(DateTime(2025, 12, 11, 18, 30, 0));
```

Then upload - the RTC will be set to your computer's time at the moment of compilation.

### How It Works

The DS3231 RTC has a **coin cell battery backup** (CR2032), so once you set the time:
- ✅ Time persists even when USB is unplugged
- ✅ Time persists when uploading new firmware
- ✅ Time stays accurate (±2 minutes per year)
- ✅ No need to set it again unless battery dies

### Troubleshooting

**"Couldn't find RTC!"**
- Check I2C connections
- RTC might be faulty
- Try different I2C address (0x68)

**Time not persisting**
- RTC battery (CR2032) might be dead
- Replace battery and set time again

**Time incorrect after setting**
- Make sure you used 24-hour format
- Check month is 1-12, not 0-11

**Serial Monitor shows nothing**
- Check baud rate is 115200
- Try different USB cable
- Wait 3-5 seconds after upload

### Example Times

**Set to noon on New Year's Day 2026:**
```cpp
rtc.adjust(DateTime(2026, 1, 1, 12, 0, 0));
```

**Set to midnight:**
```cpp
rtc.adjust(DateTime(2025, 12, 11, 0, 0, 0));
```

**Set to 11:59 PM:**
```cpp
rtc.adjust(DateTime(2025, 12, 11, 23, 59, 0));
```

### Notes

- The RTC uses **24-hour format** (0-23 hours)
- Mauther firmware displays time in 24-hour format
- Temperature sensor is built into the DS3231 (±3°C accuracy)
- Setting time does NOT affect other watch settings

---

## Future Tools

More utility sketches will be added here:
- EEPROM configuration backup
- LED calibration
- Distance sensor calibration
- BadUSB script uploader

