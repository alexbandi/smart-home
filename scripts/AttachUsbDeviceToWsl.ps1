# Parse command line arguments
param (
    [string]$DeviceName = "Silicon Labs CP210x USB to UART Bridge"
)

# Gets all USB devices.
function Get-UsbDevices {
    return usbipd list | Where-Object {
        $_ -match '^(\d+-\d+)\s+([0-9a-f]{4}:[0-9a-f]{4})\s+(.*?)\s{2,}(\w.*)$'
    } | ForEach-Object {
        New-Object -Type PSObject -Property @{
            'BUSID'    = $matches[1]
            'VID:PID'  = $matches[2]
            'DEVICE'   = $matches[3].Trim()
            'STATE'    = $matches[4].Trim()
        }
    }
}

# Gets a specific USB device based on provided device name.
function Get-UsbDevice {
    param (
        [string]$Device
    )

    $usbDevices = Get-UsbDevices
    $filteredDevices = $usbDevices | Where-Object {
        $null -eq $Device -or $_.DEVICE -like "*$Device*"
    }

    return $filteredDevices
}

# Attaches the USB device by finding the correct busid using the provided device name
function Connect-UsbDevice {
    param (
        [Parameter(Mandatory=$true)]
        [string]$DeviceName
    )

    # Get the USB device that matches the provided device name
    $usbDevices = Get-UsbDevice -Device $DeviceName
    if (@($usbDevices).Count -eq 1) {
        Write-Output "Found one device matching the name '$DeviceName'."
        $busId = $usbDevices[0].BUSID
        $state = $usbDevices[0].STATE
        if ($state -eq "Not shared") {
            Write-Output "Device is not shared. Binding USB device with bus id $busId..."
            $output = usbipd bind --busid $busId 2>&1
            if ($null -ne $output) {
                Write-Output "Cannot bind USB device: $output"
                Write-Output "Exiting."
                return
            }

            Write-Output "Successfully bound USB device."
        }

        Write-Output "Attaching USB device with BUSID $busId..."
        usbipd attach --wsl --auto-attach --busid $busId
    } elseif (@($usbDevices).Count -gt 1) {
        Write-Output "Multiple devices found matching the name '$DeviceName'. Please specify further."
    } else {
        Write-Output "No devices found matching the name '$DeviceName'."
    }
}

Connect-UsbDevice -deviceName $DeviceName
