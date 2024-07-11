#!/bin/bash

# Default device name
DEFAULT_DEVICE_NAME="Silicon Labs CP210x UART Bridge"

# Check if a device name was provided as an argument
DEVICE_NAME=${1:-$DEFAULT_DEVICE_NAME}

# Find the device using lsusb
DEVICES=$(lsusb | grep "$DEVICE_NAME")

# Check if no device was found
if [ -z "$DEVICES" ]; then
  echo "Device '$DEVICE_NAME' not found."
  exit 1
fi

# Check if multiple devices were found
DEVICE_COUNT=$(echo "$DEVICES" | wc -l)
if [ "$DEVICE_COUNT" -gt 1 ]; then
  echo "Warning: Multiple '$DEVICE_NAME' devices found. Using the first one."
fi

# Get the first device
DEVICE=$(echo "$DEVICES" | head -n 1)

# Extract the vendor and product ID from the device string
VENDOR_ID=$(echo $DEVICE | awk '{print $6}' | cut -d: -f1)
PRODUCT_ID=$(echo $DEVICE | awk '{print $6}' | cut -d: -f2)

# Check if the vendor and product IDs were extracted successfully
if [ -z "$VENDOR_ID" ] || [ -z "$PRODUCT_ID" ]; then
  echo "Failed to extract vendor and product ID."
  exit 1
fi

# Define the udev rule
UDEV_RULE="SUBSYSTEM==\"tty\", ATTRS{idVendor}==\"$VENDOR_ID\", ATTRS{idProduct}==\"$PRODUCT_ID\", MODE=\"0666\""

# Create the udev rule file
UDEV_FILE="/etc/udev/rules.d/99-usb-serial.rules"

# Write the rule to the file
echo $UDEV_RULE | sudo tee $UDEV_FILE

# Check if the file was written successfully
if [ $? -ne 0 ]; then
  echo "Failed to write the udev rule file."
  exit 1
fi

# Reload udev rules
sudo udevadm control --reload-rules

# Check if the udev rules were reloaded successfully
if [ $? -ne 0 ]; then
  echo "Failed to reload udev rules."
  exit 1
fi

# Trigger udev
sudo udevadm trigger

# Check if the udev trigger was successful
if [ $? -ne 0 ]; then
  echo "Failed to trigger udev."
  exit 1
fi

echo "Udev rule created and reloaded successfully."
