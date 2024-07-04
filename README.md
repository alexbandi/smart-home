# smart-home
## Prerequisites

Before being able to start developing, the following prerequisites are necessary.

### Visual Studio Code
Ensure [Visual Studio Code](https://code.visualstudio.com/) is installed.

### Docker
#### Docker Desktop
Ensure [Docker Desktop](https://www.docker.com/products/docker-desktop/) is installed and running on your machine

#### WSL 2
Docker Desktop uses WSL 2 as its backend on Windows. Ensure WSL 2 is installed and set up.
Follow the official [Microsoft guide](https://learn.microsoft.com/en-us/windows/wsl/install).

### usbipd-win
To be able to connect USB devices to WSL and then forward it to the dev container, [usbipd-win] must first be installed. Follow the official [Microsoft guide](https://learn.microsoft.com/en-us/windows/wsl/connect-usb) to install it.
As per note in the guide, you can install usbipd-win via the following Powershell command:
```shell
winget install --interactive --exact dorssel.usbipd-win
```

## Forwarding the ESP32 to the dev container
### Attaching the ESP32 device to WSL
On your Windows host, use the device manager (`Win + X`, then select Device Manager) to find the COM port associated with your ESP32 (e.g., `COM3`). You can find the device under `Ports (COM & LPT)`. If your device is plugged in but cannot be found in the device manager check the `Show hidden devices` via `View` in the menu bar.

Copy the name of the device (without the COM port), e.g. `Silicon Labs CP210x USB to UART Bridge (COM3)` -> `Silicon Labs CP210x USB to UART Bridge`.

Now run the following Powershell command where `<device name>` must be replaced with the corresponding name of the device found in the device manager:
```shell
.\scripts\AttachUsbToWsl.ps1 -DeviceName "<device name>"
```
Example:
```shell
.\scripts\AttachUsbToWsl.ps1 -DeviceName "Silicon Labs CP210x USB to UART Bridge"
```
If the device name is `Silicon Labs CP210x USB to UART Bridge` the `-DeviceName` option can be ommitted.

## Creating a symlink for the ESP32 device
Whenever a USB device gets attached it is potentially assigned a different bus id and device id. Therefore the device cannot be referenced easily in the `devcontainer.json`.
By running the following command as `root` inside WSL, a persistent symlink (`/dev/esp32`) is created, making it easier to identify and access the device by a consistent name regardless of which USB port it is connected to.
```shell
sh scripts/set-usb-serial-rule.sh <device name>
```
Replace `<device name>` with the name of the device found when running
```shell
lsusb
```
If ommitted, `Silicon Labs CP210x UART Bridge` is used.

## Troubleshooting
### Failed to send reload request: No such file or directory
If the error message `Failed to send reload request: No such file or directory` shows up when running `set-usb-serial-rule.sh` probably the udev deamon is not running. Start it manually with 
```shell
/lib/systemd/systemd-udevd --daemon
```