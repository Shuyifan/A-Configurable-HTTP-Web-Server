#!/bin/bash -x

VERSION=3
STAMP_FILE=~/.cs130_devel.version

function get_script_dir() {
  echo ${0%/*}
}

if [[ $(lsb_release -sc) != "bionic" ]]
then
  echo "This script only supports Ubuntu Bionic"
  exit 1
fi

if [[ -e "$STAMP_FILE" && $(cat "$STAMP_FILE") == $VERSION ]]
then
  echo "Packages up to date"
  exit 0;
fi

# Exit if any steps fail
set -e

echo "Updating packages..."

sudo apt-get update

# Install packages matching the devel environment Dockerfile
sudo apt-get install -y \
    build-essential \
    cmake \
    curl \
    httpie \
    libboost-log-dev \
    libboost-regex-dev \
    libboost-signals-dev \
    libboost-system-dev \
    libgtest-dev \
    netcat 

echo "deb http://packages.cloud.google.com/apt cloud-sdk-$(lsb_release -sc) main" | sudo tee /etc/apt/sources.list.d/google-cloud-sdk.list && \
    curl -fsSL https://packages.cloud.google.com/apt/doc/apt-key.gpg | sudo apt-key add -
echo "deb [arch=amd64] https://download.docker.com/linux/ubuntu $(lsb_release -sc) stable" | sudo tee /etc/apt/sources.list.d/docker.list && \
    curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -

# Fetch newly added repositories
sudo apt-get update

# Install local dev tools
sudo apt-get install -y \
    docker-ce-cli \
    gcovr \
    gdbserver \
    git \
    git-review \
    google-cloud-sdk \
    less \
    lldb \
    locales \
    man \
    nano \
    netcat \
    sudo \
    vim

for SCRIPT in $(get_script_dir)/docker/*.sh; do
  sudo install -p "$SCRIPT" /usr/local/bin
done

echo $VERSION > "$STAMP_FILE"
