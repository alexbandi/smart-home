FROM alpine:3.20.1

# Install necessary packages
RUN apk add --no-cache \
    git 

# Set the user to avoid permission issues
RUN adduser -D vscode
USER vscode

# Set the working directory
WORKDIR /workspace