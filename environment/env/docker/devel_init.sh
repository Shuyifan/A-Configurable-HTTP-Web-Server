#!/bin/bash

MIRROR_INCLUDES="boost gtest gmock"

MIRROR_PATH="/usr/src/projects/mirror"

COUNT_FILE=/var/lock/devel_env_count

# Run and keep track of counts
function run_and_exit() {
  if [[ -e $COUNT_FILE ]]; then
    # Already running, just add another process
    # Increment count
    COUNT=$(($(cat ${COUNT_FILE}) + 1))
    echo $COUNT > $COUNT_FILE
  else
    # Initial count
    echo 1 > $COUNT_FILE
  fi

  # Run arguments if they were given, fallback to bash
  if [[ $# > 0 ]]; then
      eval "$@"
  else
      /bin/bash --login
  fi

  # Decrement count
  COUNT=$(($(cat ${COUNT_FILE}) - 1))
  echo $COUNT > $COUNT_FILE

  # Exit code 1 if count > 0, exit code 0 if count == 0 (last process exited)
  if [[ $COUNT -gt 0 ]]; then
    exit 1
  else
    exit 0
  fi
}

# Ensure we are running in the devel shell
if [[ $DEVEL_SHELL != "1" ]]; then
  echo "This script is only to be run inside the devel shell."
  exit 1
fi

if [[ -e $COUNT_FILE ]]; then
  # Already running, don't initialize environment
  run_and_exit
fi

echo "Starting devel environment"

while getopts "m" opt; do
  case $opt in
    m)
      MIRROR=1
      ;;
  esac
done

if [[ -e /var/run/docker.sock ]]; then
  echo "Setting up docker socket..."
  sudo chown :$(id -gn) /var/run/docker.sock
fi

echo "Setting up git..."
git config --global user.email "${USER}@cs130.org"
git config --global user.name "$(getent passwd $USER | cut -d ':' -f 5)"
git config --global gitreview.remote "origin"

if [[ $MIRROR ]]; then
  echo "Checking for mirrored headers..."
  # Copy installed header files to mirror for host IDE
  for PACKAGE in ${MIRROR_INCLUDES}; do
    if [[ ! -e ${MIRROR_PATH}/include/${PACKAGE} ]]; then
      echo -n "Copying ${PACKAGE} headers to host mirror..."
      mkdir -p ${MIRROR_PATH}/include/${PACKAGE}
      cp -R /usr/include/${PACKAGE} ${MIRROR_PATH}/include
      echo " Done"
    fi
  done
fi

echo "Checking SSH key..."
if [[ ! -e ~/.ssh/id_rsa ]]; then
  YELLOW="\033[1;33m"
  NC="\033[0m"

  echo "Generating SSH key..."
  ssh-keygen -q -t rsa -b 4096 -N "" -f ~/.ssh/id_rsa
  echo "Copy the yellow text below to a new SSH key at https://code.cs130.org/settings/#SSHKeys"
  echo -e "${YELLOW}"
  cat ~/.ssh/id_rsa.pub
  echo -e "${NC}"
fi

echo "Finished initializing development environment"

run_and_exit
