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

In WSL2 you should now find your device at `/dev/ttyUSB0`.

_Remark:_ \
_If the device does not show up you might need to manually start the USB-to-serial driver that corresponds to the USB-to-serial chip on the ESP32 board. Most ESP32 boards, including the DevKitC V4, use either the CP210x or the CH340G USB-to-serial converter chips. Start the CP210xdriver in WSL2 by running `sudo modprobe cp210x` or use another driver if necessary._

### Creating a udev rule for the right permissions
For the development container to be able to access the device, the correct permissions have to be set. By defining a udev rule those permissions can be set automatically for every device with the same vendor and product id.

To set the rule, run
```shell
sh scripts/create_udev_rule.sh <device name>
```
in your WSL2 shell where `<device name>` is the name listed when using `lsusb`. If the device name is `Silicon Labs CP210x UART Bridge` the device name can be ommited.