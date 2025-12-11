#!/bin/bash
# Restore script for DStike Bad Watch firmware

echo "========================================="
echo "DStike Bad Watch Firmware Restore Tool"
echo "========================================="
echo ""

# Check if hex file is provided
if [ $# -eq 0 ]; then
    echo "Usage: $0 <firmware.hex>"
    echo ""
    echo "Available backups:"
    ls -1 *.hex 2>/dev/null
    exit 1
fi

FIRMWARE_FILE="$1"

# Check if file exists
if [ ! -f "$FIRMWARE_FILE" ]; then
    echo "ERROR: File not found: $FIRMWARE_FILE"
    exit 1
fi

echo "Firmware file: $FIRMWARE_FILE"
ls -lh "$FIRMWARE_FILE"
echo ""

# Find avrdude
AVRDUDE=""
if [ -f "/Users/marcin/bin/avrdude_macOS_64bit/bin/avrdude" ]; then
    AVRDUDE="/Users/marcin/bin/avrdude_macOS_64bit/bin/avrdude"
    AVRDUDE_CONF="/Users/marcin/bin/avrdude_macOS_64bit/etc/avrdude.conf"
elif command -v avrdude &> /dev/null; then
    AVRDUDE="avrdude"
    AVRDUDE_CONF=""
else
    echo "ERROR: avrdude not found!"
    echo "Please install Arduino IDE or avrdude"
    exit 1
fi

echo "Using avrdude: $AVRDUDE"
echo ""

# Check if device is connected
echo "Checking for connected device..."
if [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS
    INITIAL_PORT=$(ls /dev/cu.usbmodem* 2>/dev/null | head -n 1)
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    # Linux
    INITIAL_PORT=$(ls /dev/ttyACM* 2>/dev/null | head -n 1)
else
    echo "ERROR: Unsupported OS"
    exit 1
fi

if [ -z "$INITIAL_PORT" ]; then
    echo "ERROR: Device not found!"
    echo "Please connect the DStike Bad Watch via USB"
    exit 1
fi

echo "Device detected: $INITIAL_PORT"
echo ""

# Confirmation
echo "⚠️  WARNING: This will overwrite the current firmware!"
echo ""
read -p "Are you sure you want to continue? (yes/no): " CONFIRM

if [ "$CONFIRM" != "yes" ]; then
    echo "Restore cancelled"
    exit 0
fi

echo ""
echo "⚠️  IMPORTANT: You need to enter BOOTLOADER mode!"
echo ""
echo "Steps:"
echo "1. Press the RESET button on your watch"
echo "2. Wait 1 second for the bootloader to activate"
echo "3. Press ENTER here (within 8 seconds of reset)"
echo ""
read -p "Press ENTER after you've pressed RESET: "

echo ""
echo "Detecting bootloader port..."

# Wait a moment for port to appear
sleep 1

# Detect the NEW port (bootloader port)
PORT=""
if [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS - look for the bootloader port
    PORT=$(ls /dev/cu.usbmodem* 2>/dev/null | head -n 1)
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    # Linux
    PORT=$(ls /dev/ttyACM* 2>/dev/null | head -n 1)
fi

if [ -z "$PORT" ]; then
    echo "❌ ERROR: Bootloader port not found!"
    echo ""
    echo "Troubleshooting:"
    echo "1. Press and hold RESET for 1 second"
    echo "2. Release and immediately run: ls /dev/cu.usbmodem*"
    echo "3. You should see a port appear briefly"
    echo "4. Try double-tapping RESET (Leonardo bootloader trick)"
    exit 1
fi

echo "Found bootloader on: $PORT"
echo ""
echo "Starting restore..."
echo ""

# Run restore command with retries
MAX_RETRIES=2
RETRY_COUNT=0
SUCCESS=0

while [ $RETRY_COUNT -lt $MAX_RETRIES ]; do
    RETRY_COUNT=$((RETRY_COUNT + 1))
    echo "Attempt $RETRY_COUNT of $MAX_RETRIES..."
    
    if [ -z "$AVRDUDE_CONF" ]; then
        $AVRDUDE -p atmega32u4 -c avr109 -P $PORT -U flash:w:$FIRMWARE_FILE:i
    else
        $AVRDUDE -C $AVRDUDE_CONF -p atmega32u4 -c avr109 -P $PORT -U flash:w:$FIRMWARE_FILE:i
    fi
    
    if [ $? -eq 0 ]; then
        SUCCESS=1
        break
    fi
    
    if [ $RETRY_COUNT -lt $MAX_RETRIES ]; then
        echo ""
        echo "❌ Attempt failed. Press RESET button again and press ENTER..."
        read -p ""
        sleep 1
        # Re-detect port
        if [[ "$OSTYPE" == "darwin"* ]]; then
            PORT=$(ls /dev/cu.usbmodem* 2>/dev/null | head -n 1)
        elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
            PORT=$(ls /dev/ttyACM* 2>/dev/null | head -n 1)
        fi
        echo "Trying port: $PORT"
    fi
done

if [ $SUCCESS -eq 0 ]; then
    echo ""
    echo "========================================="
    echo "Restore failed after $MAX_RETRIES attempts!"
    echo "========================================="
    echo ""
    echo "Troubleshooting:"
    echo "1. Press the RESET button on the watch"
    echo "2. Wait for the bootloader LED pattern"
    echo "3. Run this script again immediately"
    echo "4. Or try: Double-tap RESET for bootloader mode"
    exit 1
fi

echo ""
echo "========================================="
echo "✅ Restore successful!"
echo "========================================="
echo "Your DStike Bad Watch has been restored"
echo "Please disconnect and reconnect the device"

