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