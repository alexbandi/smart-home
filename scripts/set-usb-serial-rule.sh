#!/bin/bash

# Set the default device name
DEFAULT_DEVICE_NAME="Silicon Labs CP210x UART Bridge"

# Check if a device name is provided as an argument
DEVICE_NAME="${1:-$DEFAULT_DEVICE_NAME}"

# Get the device information using lsusb
DEVICE_INFO=$(lsusb | grep "$DEVICE_NAME")

# Check if the device was found
if [ -z "$DEVICE_INFO" ]; then
    echo "Device '$DEVICE_NAME' not found."
    exit 1
fi

# Extract the Vendor ID and Product ID
VID=$(echo "$DEVICE_INFO" | awk '{print $6}' | cut -d':' -f1)
PID=$(echo "$DEVICE_INFO" | awk '{print $6}' | cut -d':' -f2)

# Create the udev rule
UDEV_RULE="SUBSYSTEM==\"usb\", ATTR{idVendor}==\"$VID\", ATTR{idProduct}==\"$PID\", SYMLINK+=\"esp32\""

# Write the udev rule to the rules.d directory
echo "$UDEV_RULE" | sudo tee /etc/udev/rules.d/99-usb-serial.rules

# Reload udev rules and trigger
sudo udevadm control --reload-rules
sudo udevadm trigger

echo "udev rule added for device '$DEVICE_NAME' with VID:PID $VID:$PID"
