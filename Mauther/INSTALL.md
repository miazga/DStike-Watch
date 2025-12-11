# Installation Guide for Mauther Firmware

Quick start guide to get Mauther running on your DStike Bad Watch.

## 📋 Before You Begin

### Required Hardware
- ✅ DStike Bad Watch
- ✅ USB cable (data capable, not charge-only)
- ✅ Computer (Windows, macOS, or Linux)

### Required Software
- Arduino IDE 1.8.x or higher
- USB drivers (usually auto-installed)

## 🔧 Step-by-Step Installation

### Step 1: Backup Original Firmware ⚠️

**THIS IS CRUCIAL!** Always backup before flashing custom firmware.

```bash
cd ../Backup
./backup.sh
```

**What happens**:
1. Script checks device is connected
2. Prompts you to **press RESET button** on the watch
3. You press RESET, then press ENTER in terminal
4. Script detects bootloader port and backs up firmware
5. Backup saved as `dstike_backup_YYYYMMDD_HHMMSS.hex`

**Note**: The bootloader is only active for ~8 seconds after pressing RESET. The script handles this automatically, but you must press the physical RESET button on your watch when prompted.

**Troubleshooting**: If backup fails, see `../Backup/BOOTLOADER_GUIDE.md` for detailed help.

**Windows users**: See `../Backup/README.md` for detailed instructions.

---

### Step 2: Install Arduino IDE

Download from: https://www.arduino.cc/en/software

- **Windows**: Run installer
- **macOS**: Drag to Applications
- **Linux**: Extract and run `./install.sh`

---

### Step 3: Install Required Libraries

Open Arduino IDE and install these libraries via Library Manager:

**How to open Library Manager**:
```
Sketch → Include Library → Manage Libraries
```

**Install these libraries**:

1. **U8g2** by oliver
   - Search: "U8g2"
   - Click "Install"

2. **VL53L0X** by Pololu
   - Search: "VL53L0X"
   - Click "Install"

3. **RTClib** by Adafruit
   - Search: "RTClib"
   - Click "Install"

4. **Adafruit NeoPixel** by Adafruit ✨ **IMPORTANT!**
   - Search: "Adafruit NeoPixel"
   - Click "Install"
   - **Required for RGB LED to work!**

---

### Step 4: Configure Arduino IDE

1. **Select Board**:
   ```
   Tools → Board → Arduino AVR Boards → Arduino Leonardo
   ```

2. **Connect Watch** via USB

3. **Select Port**:
   ```
   Tools → Port → (select your device)
   ```
   
   - **macOS**: `/dev/cu.usbmodem...`
   - **Linux**: `/dev/ttyACM0`
   - **Windows**: `COM3` (or similar)

---

### Step 5: Open Mauther Project

1. Navigate to the Mauther folder
2. Open `Mauther.ino` in Arduino IDE

---

### Step 6: Compile (Verify)

Click the **✓ Verify** button to check for errors.

Expected output:
```
Sketch uses XXXXX bytes (XX%) of program storage space.
Global variables use XXX bytes (XX%) of dynamic memory.
```

If you see errors, check that all libraries are installed.

---

### Step 7: Upload Firmware

1. Click the **→ Upload** button

2. **Arduino IDE will handle bootloader timing**:
   - Most of the time, Arduino IDE automatically triggers bootloader mode
   - If upload hangs at "Uploading...", **press RESET button** on the watch
   - The IDE will detect the bootloader and continue

3. **Wait for completion**:
   - Compiling: ~10-30 seconds
   - Uploading: ~10-20 seconds
   - Total: ~30-60 seconds

4. **Progress indicators**:
   ```
   Compiling sketch...
   Uploading...
   Writing...
   Verifying...
   Done!
   ```

5. **Watch will reboot automatically**

**💡 Tip for Development**: After the first upload, subsequent uploads are usually faster and don't require manual RESET. Arduino IDE learns the device timing.

---

### Step 8: First Boot

After upload completes:

1. **Splash screen** appears (2 seconds):
   ```
   Mauther v1.0
   DStike Bad Watch
   ```

2. **Boot sound** plays (beep-beep)

3. **Main screen** loads:
   ```
   MAUTHER
   00:00:00
   Temp: XX.XC
   Dist: XXXmm
   ```

---

## ✅ Verify Installation

Check that everything works:

### Test Display
- Main screen should show time, temp, distance

### Test Buttons
- **SELECT**: Opens menu → Should show "MAIN MENU"
- **UP/DOWN**: Navigate menu → Selection should move
- **SELECT**: Confirm → Should enter sub-menu

### Test Distance Sensor
- Move hand in front of watch
- Distance value should change
- If < 1m: Buzzer sounds, LED turns red

### Test Laser
- From main screen: Press **DOWN**
- Laser should turn on
- Press **DOWN** again to turn off

### Test LED
- Enter menu: **SELECT**
- Navigate to "LED Test": **DOWN** → **SELECT**
- Cycle colors: **UP** or **DOWN**

---

## 🐛 Troubleshooting

### "Board not found" or Upload Hangs

**Solution 1**: Press Reset During Upload
- Click Upload in Arduino IDE
- When it says "Uploading..." and hangs, **press RESET button** on watch
- IDE will detect bootloader and continue
- This is normal for first upload or after long idle time

**Solution 2**: Port Changes After Reset
- The port may change after pressing RESET
- Before reset: `/dev/cu.usbmodemHIDPC1`
- After reset: `/dev/cu.usbmodemHIDPC2` (or similar)
- Arduino IDE usually handles this automatically

**Solution 3**: Check USB Cable
- Must be a data cable, not charge-only
- Try a different USB port
- Try a different cable

**Solution 4**: Reinstall Drivers
- Windows: Install Arduino Leonardo drivers manually
- macOS/Linux: Usually automatic

---

### "Verification Failed" Error

**Solution**:
- Upload again (sometimes first attempt fails)
- Check USB connection
- Try different USB port
- Ensure battery is charged (>50%)

---

### Libraries Not Found

**Error**: `fatal error: U8g2lib.h: No such file or directory`

**Solution**:
1. Open Library Manager
2. Search for the missing library
3. Click "Install"
4. Restart Arduino IDE
5. Try compiling again

---

### Display Shows Nothing

**Possible causes**:
- Wrong I2C address
- Loose connection
- Code not uploaded properly

**Solution**:
1. Re-upload firmware
2. Check display connections
3. Try I2C scanner sketch to verify address

---

### Serial Port Not Showing

**macOS**:
```bash
ls /dev/cu.*
# Look for /dev/cu.usbmodem*
```

**Linux**:
```bash
ls /dev/ttyACM*
# May need: sudo usermod -a -G dialout $USER
```

**Windows**:
- Open Device Manager
- Check "Ports (COM & LPT)"
- Look for "Arduino Leonardo"

---

## 🔄 Restoring Original Firmware

If you want to go back to the original firmware:

### Using Backup Script (Recommended)
```bash
cd ../Backup
./restore.sh dstike_backup_YYYYMMDD_HHMMSS.hex
```

**What happens**:
1. Script confirms you want to restore
2. Prompts you to **press RESET button** on the watch
3. You press RESET, then press ENTER in terminal
4. Script detects bootloader port and restores firmware
5. Watch reboots with original firmware

### Using Arduino IDE
1. Open original firmware `.ino` file (if you have it)
2. Select Arduino Leonardo board
3. Select correct port
4. Click Upload
5. Press RESET if upload hangs at "Uploading..."

---

## 📝 Post-Installation

### Set the Time

Time is set to compile time on first boot. To update:

**Option 1**: Re-upload firmware (time updates to current compile time)

**Option 2**: Modify code and add time-setting function

**Option 3**: Add RTC battery (CR2032) to DS3231 module to maintain time

### Configure Settings

Edit `config.h` to customize:
- Alarm distance threshold
- Menu timeout duration
- Buzzer frequencies
- Debug mode

Then re-upload firmware.

---

## 🎉 You're Ready!

Your DStike Bad Watch is now running Mauther firmware with full hardware support and BadUSB capabilities.

**Next Steps**:
- Explore the menu system
- Try the distance sensor
- Test BadUSB scripts (safely!)
- Customize the firmware

**Need Help?**
- Check `README.md` for full documentation
- Review `Backup/README.md` for backup/restore info
- Check Arduino Leonardo documentation

---

**Happy Hacking! 🚀**

