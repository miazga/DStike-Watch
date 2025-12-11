# Bootloader Mode Guide for DStike Bad Watch

## Understanding the Bootloader

The ATmega32U4 chip on your DStike Bad Watch has a bootloader that allows firmware uploads. However, it's only active for a **short window (~8 seconds)** after reset.

## Why Backup/Upload Fails

If you see errors like:
```
Error: butterfly_recv(pgm, &c, 1) failed
Error: initialization failed (rc = -1)
```

This means the device is **not in bootloader mode** or the bootloader window has expired.

## How to Enter Bootloader Mode

### Method 1: Single Reset (Quick Window)

1. **Run the backup/restore script first**
2. When prompted, **press the RESET button** on the watch
3. **Immediately press ENTER** in the terminal
4. The bootloader is active for ~8 seconds

**Signs of bootloader mode:**
- Watch screen may go blank
- LED may blink in a pattern
- USB device may reconnect

### Method 2: Double Reset (Leonardo-style)

Some Arduino Leonardo bootloaders support double-reset:

1. Press RESET button
2. Wait 0.5 seconds
3. Press RESET button again quickly
4. Bootloader stays active longer

### Method 3: Manual Timing

If scripts fail, try manual timing:

```bash
# 1. Press RESET button on watch
# 2. Within 8 seconds, run:
avrdude -C /Users/marcin/bin/avrdude_macOS_64bit/etc/avrdude.conf \
        -p atmega32u4 -c avr109 -P /dev/cu.usbmodemHIDPC1 \
        -B 115200 -U flash:r:backup.hex:i
```

Replace `/dev/cu.usbmodemHIDPC1` with your actual port.

## Troubleshooting

### Device Disappears After Reset

This is normal! The port may change:
- Before reset: `/dev/cu.usbmodemHIDPC1`
- After reset: `/dev/cu.usbmodemHIDPC2` (or similar)

**Solution**: Run `ls /dev/cu.usbmodem*` after pressing reset to find the new port.

### "Port Not Found" Error

The bootloader port might be different from the application port.

**Check for bootloader port:**
```bash
# Before reset
ls /dev/cu.usbmodem*

# Press RESET button

# After reset (quickly!)
ls /dev/cu.usbmodem*
```

Look for a new port that appears.

### Bootloader Window Too Short

If 8 seconds isn't enough:

1. Have the terminal ready with command typed
2. Press reset
3. Immediately hit ENTER
4. Or use Arduino IDE which handles timing automatically

### Connection Keeps Failing

Try slower baud rate:
```bash
avrdude ... -B 9600 ...  # Instead of -B 115200
```

Or remove the `-B` flag entirely:
```bash
avrdude ... (no -B flag) ...
```

## Arduino IDE Alternative

Arduino IDE automatically handles bootloader timing:

1. Open Arduino IDE
2. File → Open → (select .ino file)
3. Tools → Board → Arduino Leonardo
4. Tools → Port → (select your device)
5. Sketch → Upload

**Press RESET when IDE says "Uploading..."**

## Finding the Reset Button

On DStike Bad Watch:
- Look for a small button (usually on the side or back)
- It may be labeled "RST" or "RESET"
- It's typically a tactile push button

## Bootloader LED Patterns

When in bootloader mode, you may see:
- LED blinking slowly
- LED pulsing (breathing effect)
- Screen goes blank
- Different USB device name

## Script Retry Logic

The updated backup/restore scripts now:
- ✅ Prompt you to press RESET
- ✅ Wait for your confirmation
- ✅ Retry up to 3 times
- ✅ Use appropriate baud rate (-B 115200)
- ✅ Provide helpful error messages

## Best Practices

1. **Be quick**: Have everything ready before pressing RESET
2. **Use scripts**: They handle timing better than manual commands
3. **Watch for LED**: Visual confirmation of bootloader mode
4. **Check port**: Port may change after reset
5. **Try multiple times**: First attempt often fails, that's normal

## Emergency: Can't Enter Bootloader

If you absolutely cannot enter bootloader mode:

### Hardware Bootloader Trigger
Some boards support hardware bootloader trigger:
- Short the RESET pin to GND while connecting USB
- Check DStike documentation for specific method

### ISP Programmer
Use an external ISP programmer:
- AVRISP mkII
- USBasp
- Arduino as ISP

Connect to ICSP header (if available) and flash directly.

## Summary Commands

### Quick Backup with Correct Port
```bash
# 1. Find port first
ls /dev/cu.usbmodem*

# 2. Press RESET button

# 3. Run immediately (replace PORT)
cd /Users/marcin/dev/deauther/Backup
./backup.sh
# (Follow prompts)
```

### Quick Restore
```bash
cd /Users/marcin/dev/deauther/Backup
./restore.sh your_backup.hex
# (Follow prompts to press RESET)
```

---

**Remember**: The 8-second window is tight, but with practice you'll get the timing right!

