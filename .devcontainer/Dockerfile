FROM ubuntu:24.04

# Update the package list and install necessary packages
RUN apt-get update && apt-get install -y \
    curl \
    git \
    python3 \
    python3-venv \
    && rm -rf /var/lib/apt/lists/*

## Install PlatformIO Core (CLI)
# See https://docs.platformio.org/en/latest/core/installation/methods/installer-script.html
RUN curl -fsSL -o /tmp/get-platformio.py https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py \
    && python3 /tmp/get-platformio.py \
    && rm /tmp/get-platformio.py

# Add PlatformIO to the PATH variable
ENV PATH="/home/vscode/.platformio/penv/bin:${PATH}"

# # Set the working directory
WORKDIR /workspace