FROM alpine:3.20.1

# Arguments for user details
ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID

# Install necessary packages
RUN apk add --no-cache \
    git \
    python3

# Set the user to avoid permission issues
RUN addgroup -g $USER_GID $USERNAME \ 
    && adduser -u $USER_UID -G $USERNAME -s /bin/sh -D $USERNAME
# Set the default user
USER vscode
    
# Install PlatformIO Core (CLI)
# See https://docs.platformio.org/en/latest/core/installation/methods/installer-script.html
RUN wget -O /tmp/get-platformio.py https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py \
    && python3 /tmp/get-platformio.py \
    && rm /tmp/get-platformio.py

# Set the working directory
WORKDIR /workspace