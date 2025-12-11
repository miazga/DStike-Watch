# Firmware Backup & Restore Guide

This folder contains tools and instructions for backing up and restoring firmware on the DStike Bad Watch.

## ⚠️ Important Notice

Before flashing custom firmware (like Mauther), it's highly recommended to backup your original firmware. However, **the ATmega32U4 bootloader cannot be backed up via Arduino IDE** - only the application firmware can be saved.

### 🔴 Critical: Bootloader Mode Required

The backup/restore process requires the device to be in **bootloader mode**. The bootloader is only active for ~8 seconds after pressing RESET. See [BOOTLOADER_GUIDE.md](BOOTLOADER_GUIDE.md) for detailed instructions.

**Quick Start:**
1. Run the backup script: `./backup.sh`
2. When prompted, press the RESET button on your watch
3. Immediately press ENTER in the terminal
4. The script will handle the rest (with retries)

## Backup Methods

### Method 1: Using avrdude (Recommended)

#### Prerequisites
- Install avrdude (Arduino IDE includes it)
- Find avrdude location:
  - **Windows**: `C:\Program Files (x86)\Arduino\hardware\tools\avr\bin\avrdude.exe`
  - **macOS**: `/Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/avrdude`
  - **Linux**: Usually in `/usr/bin/avrdude` or `/usr/share/arduino/hardware/tools/avr/bin/avrdude`

#### Backup Command

```bash
# macOS/Linux
avrdude -p atmega32u4 -c avr109 -P /dev/cu.usbmodem* -U flash:r:backup_firmware.hex:i

# Windows
avrdude -p atmega32u4 -c avr109 -P COM3 -U flash:r:backup_firmware.hex:i
```

**Note**: Replace `/dev/cu.usbmodem*` or `COM3` with your actual port.

#### Find Your Port
- **macOS**: `ls /dev/cu.*`
- **Linux**: `ls /dev/ttyACM*`
- **Windows**: Check Device Manager or Arduino IDE

### Method 2: Using Arduino IDE (Partial Backup)

Unfortunately, Arduino IDE doesn't have a built-in backup feature. You can only keep the original `.hex` file if you have it.

### Method 3: Using Original Firmware Files

If you have the original firmware source code or hex file from the manufacturer, keep a copy in this folder:

```
Backup/
├── original_firmware/
│   ├── firmware.hex
│   └── firmware.ino
└── README.md
```

## Restore Original Firmware

### Using avrdude

```bash
# macOS/Linux
avrdude -p atmega32u4 -c avr109 -P /dev/cu.usbmodem* -U flash:w:backup_firmware.hex:i

# Windows  
avrdude -p atmega32u4 -c avr109 -P COM3 -U flash:w:backup_firmware.hex:i
```

### Using Arduino IDE

1. Open the original firmware `.ino` file
2. Select **Tools > Board > Arduino Leonardo**
3. Select the correct COM port
4. Click **Upload**

## Creating a Backup Before First Flash

### Step-by-Step Process

1. **Connect the watch** via USB

2. **Find your port**:
```bash
# macOS
ls /dev/cu.*

# Linux
ls /dev/ttyACM*
```

3. **Create backup directory**:
```bash
mkdir -p ~/DStike_Backup
cd ~/DStike_Backup
```

4. **Run backup command**:
```bash
# Example for macOS
avrdude -p atmega32u4 -c avr109 -P /dev/cu.usbmodem14101 -U flash:r:original_firmware_$(date +%Y%m%d).hex:i
```

5. **Verify backup**:
```bash
ls -lh *.hex
# Should show a file around 28-32KB
```

6. **Save to this folder**:
```bash
cp *.hex /Users/marcin/dev/deauther/Backup/
```

## Troubleshooting

### "Permission Denied" Error
```bash
# Linux
sudo usermod -a -G dialout $USER
# Then logout and login

# macOS - Usually not needed
```

### "Device Not Found"
1. Press the reset button on the watch
2. The bootloader is active for ~8 seconds
3. Run the command immediately

### "Verification Error"
- Try uploading again
- Check USB cable quality
- Use a different USB port

## Alternative: Using AVRISP/USBasp Programmer

If you have an ISP programmer, you can backup both bootloader and application:

```bash
# Backup EVERYTHING (including bootloader)
avrdude -p atmega32u4 -c usbasp -U flash:r:complete_backup.hex:i -U eeprom:r:eeprom_backup.hex:i -U lfuse:r:lfuse.hex:i -U hfuse:r:hfuse.hex:i -U efuse:r:efuse.hex:i
```

## Restore Safety Checklist

✅ Backup file exists and is non-zero size  
✅ Correct board selected (Arduino Leonardo)  
✅ Correct port selected  
✅ USB cable is reliable  
✅ Battery is charged (>50%)  

## Additional Resources

- [Arduino Leonardo Documentation](https://www.arduino.cc/en/Main/Arduino_BoardLeonardo)
- [avrdude Documentation](https://www.nongnu.org/avrdude/user-manual/avrdude.html)
- [ATmega32U4 Datasheet](http://www.atmel.com/Images/Atmel-7766-8-bit-AVR-ATmega16U4-32U4_Datasheet.pdf)

## Support

If you brick your device and don't have a backup:
1. Try finding the original firmware from the manufacturer (DStike)
2. Contact DStike support
3. Look for community firmware repositories

---

**Remember**: Always backup before flashing custom firmware!

