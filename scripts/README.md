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