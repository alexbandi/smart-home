# Attaching the USB device to WSL
In Windows the USB device must first be attached to WSL2 before it can be accessed from within the dev container. The following steps describe how the device can be attached.
All these steps combined are part of the Powershell script inside this folder.

## Identifying the device 
Use
```shell
usbipd list
```
and identify your device. 
Example:
> ```
> PS C:\Users\Alex> usbipd list 
> Connected:
> BUSID  VID:PID    DEVICE                                                        STATE
> 1-8    10c4:ea60  Silicon Labs CP210x USB to UART Bridge (COM3)                 Not shared
> ...
> ```

In this case `Silicon Labs CP210x USB to UART Bridge (COM3)` is the required device and the bus id `1-8` is used in the next steps.

## Sharing the device
Before you can attach the USB device to WSL2 make sure the device is shared, i. e. the `STATE` is `Shared`.
Use the following Powershell command as administator to share the device.
```shell
usbipd bind --busid <busid>
```
Example:
```shell
usbipd bind --busid 1-8
```
Now running `usbipd list` should look like the following
> ```
> PS C:\Users\Alex> usbipd list \
> Connected:\
> BUSID  VID:PID    DEVICE                                                        STATE
> 1-8    10c4:ea60  Silicon Labs CP210x USB to UART Bridge (COM3)                 Shared
> ...
> ```

## Attaching the device to WSL2

Now that you have identified your device attach it to your WSL2 by using the following command
```shell
usbipd attach --wsl --busid <busid>
```
Example:
```shell
usbipd attach --wsl --busid 1-8
```

_Remark:_ \
_Use the `--auto-attach` option on `ubsipd attach` to automatically re-attach when the device is detached or unplugged._


To verify if the device has been correctly attached to WSL2 open your WSL terminal and use the following command
```shell
lsusb
```
Example:
```
root@ALEX-PC:/mnt/c/Users/Alex# lsusb
Bus 002 Device 001: ID 1d6b:0003 Linux Foundation 3.0 root hub
Bus 001 Device 002: ID 10c4:ea60 Silicon Labs CP210x UART Bridge
Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
```
You should find your device in the list, e. g. `Silicon Labs CP210x UART Bridge`. The bus `001` and the device `002` are required later

# Creating a symlink for a USB device
Whenever a USB device gets attached it is potentially assigned a different bus id and device id. Therefore the device cannot be referenced easily in the `devcontainer.json`.
The vendor id and product id though, stay the same. By creating a udev rule, we can create a symlink of the device such that it can be referenced by the same device independent of the Bus id and device id.
So instead of being required to reference the device via `/dev/bus/usb/<bus id>/<device id>`, we can reference it statically, e. g. via `/dev/esp32`.
This can be realized by adding the following line to `/etc/udev/rules.d/99-usb-serial.rules`
```
SUBSYSTEM=="usb", ATTR{idVendor}=="<vendor id>", ATTR{idProduct}=="<product id>", SYMLINK+="<device name>"

```
where `<vendor id>` and `<product id>` are the ids found when using `lsusb` (e. g. `10c4:ea60` where `10c4` ist the vendor id and `ea60` is the product id) and `<device name>` the desired symlink (e. g. `esp32`).