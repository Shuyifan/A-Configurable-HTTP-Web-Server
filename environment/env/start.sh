#!/bin/bash

DOCKER_SOCK=/var/run/docker.sock

function print_usage() {
  echo "Syntax: $0 -u user[:group] [-d SRC_DIR] [-h] [-i userid[:groupid]] [-r] [-- [docker run arguments]]"
cat << EOF

Required flags:
  -u  :  Specify the user name (and optionally group name) for the environment.

Optional flags:
  -d  :  Specify a source directory to map as /usr/src/project. Defaults to current directory.
  -h  :  Print this help.
  -i  :  Specify a user ID (and optionally group ID) for the running user and group. Defaults to auto-detected values on Linux and Mac.
  -n  :  Run without Docker container (requires supported Linux OS).
  -r  :  Enable remote access of headers (create mirror dir) and ports for remote debugging.
  --  :  Pass through additional arguments to "docker run". Useful to set up port forwarding.
EOF
  exit 1
}

function check_for_updates() {
  TOOLS_DIR=$(get_tools_dir)
  UPDATE_STAMP_FILE="${TOOLS_DIR}/.update.stamp"
  # find returns file name if older than time given
  STALE_STAMP=$(find "$UPDATE_STAMP_FILE" -mtime +1h 2>/dev/null)

  # Need update if stamp is stale or does not exist
  if [[ $STALE_STAMP != "" || ! -e "$UPDATE_STAMP_FILE" ]]; then
    echo "Updating tools repository..."
    # Quote all current arguments
    ARGS=''
    for ARG in "$@"; do
        ARG="${ARG//\\/\\\\}"
        ARGS="$ARGS \"${ARG//\"/\\\"}\""
    done
    touch $UPDATE_STAMP_FILE
    # Evalute command through bash, since this running script file may get updated
    exec bash -c "cd \"${TOOLS_DIR}\"; git pull; cd \"${PWD}\"; $0 $ARGS"
  fi
}

function check_not_devel_env() {
  # Ensure we are NOT running in the devel shell
  if [[ $DEVEL_SHELL == 1 ]]; then
    echo "Error, already running inside the devel shell, exiting."
    exit 1
  fi
}

function get_platform() {
  case $(uname) in
    Darwin)
      echo MACOS
      ;;
    Linux)
      if grep -qE "(Microsoft|WSL)" /proc/version &> /dev/null; then
        echo WSL
      else
        echo LINUX
      fi
      ;;
    *)
      echo UNKNOWN
  esac
}

function is_container_existing() {
  docker container inspect $1 > /dev/null 2>&1
}

function is_image_existing() {
  docker image inspect $1 > /dev/null 2>&1
}

function is_container_running() {
  RUNNING=$(docker container inspect --format="{{.State.Running}}" $1 2> /dev/null)

  if [[ $? != 0 ]]; then
    return 1
  fi

  if [[ "$RUNNING" == "true" ]]; then
    return 0
  fi

  return 1
}

function is_program_available() {
  which $1 > /dev/null 2>&1
}

function require() {
  if ! is_program_available $1; then
    echo "Error, could not find \"$1\" command."
    exit 1
  fi
}

function build_image() {
  # Set up apt package cacher
  start_apt_cacher

  if get_apt_cacher_ip > /dev/null; then
    APT_CACHER_IP=$(get_apt_cacher_ip)
    echo "Using devel_env_apt_cacher at $APT_CACHER_IP"
    DOCKER_BUILD_ARGS+=("--build-arg" "http_proxy=http://${APT_CACHER_IP}:3142")
  fi

  # Copy local package list
  USER_PKG_LIST_FILE=$(get_tools_dir)/user_pkg_list
  USER_PKG_LIST_TMP=$(get_script_dir)/docker/user_pkg_list.tmp
  if [[ -e "$USER_PKG_LIST_FILE" ]]; then
    cp "$USER_PKG_LIST_FILE" "$USER_PKG_LIST_TMP"
  else
    touch "$USER_PKG_LIST_TMP"
  fi

  # Build image
  docker build -t $1 \
    "${DOCKER_BUILD_ARGS[@]}" \
    "$(get_script_dir)/docker"
  
  BUILD_STATUS=$?

  # Clean up temp file
  rm "$USER_PKG_LIST_TMP"

  # Tear down apt cacher
  stop_apt_cacher

  return $BUILD_STATUS
}

function get_absolute_path() {
  # Make SRC_DIR into an absolute path if it is not already
  if [[ ! -z $1 ]]; then
    # If the specified repo dir is absolute, use it
    if [[ $1 == /* ]]; then
      echo $1
    else
      # Otherwise, append it to the existing absolute dir
      echo "$(pwd)/$1"
    fi
  else
    pwd
  fi
}

function create_container() {
  SRC_DIR=$(get_absolute_path $SRC_DIR_ARG)

  echo "Using source directory '$SRC_DIR', mapping to '/usr/src/projects'"

  if [[ -S $DOCKER_SOCK ]]; then
    echo "Enabling docker in docker"
    DOCKER_RUN_ARGS+=("-v" "${DOCKER_SOCK}:${DOCKER_SOCK}")
  else
    echo "Could not find docker sock file (${DOCKER_SOCK}), docker will be unavailable inside devel environment."
  fi

  # Run image
  docker run -dit --rm \
    -v ${USER_N}_home:/home/${USER_N} \
    -v "${SRC_DIR}":/usr/src/projects:delegated \
    --security-opt seccomp=unconfined --cap-add SYS_PTRACE \
    --name $1 \
    --hostname $1 \
    "${DOCKER_RUN_ARGS[@]}" \
    $1
}

function stop_container() {
  docker container stop $1
}

function remove_container() {
  docker container rm $1
}

function exec_shell() {
  FLAGS=
  if [[ $REMOTE ]]; then
    FLAGS="-m"
  fi
  docker exec -it $1 /usr/local/bin/devel_init.sh $FLAGS
}

function get_script_dir() {
  echo ${0%/*}
}

function get_tools_dir() {
  echo "${0%/*}/.."
}

function setup_docker_on_wsl() {
  if [[ -z $DOCKER_HOST ]]; then
    cat << EOF
Error, DOCKER_HOST environment variable not found, docker will not be able to run inside ubuntu.

Make sure Docker for Windows is exposing the daemon on tcp://localhost:2375, and export DOCKER_HOST=tcp://localhost:2375
EOF
    exit 1
  fi

  if [[ ! -S $DOCKER_SOCK ]]; then
    if is_program_available socat; then
      echo "Docker socket file not found, will attempt to create with socat."
      sudo echo "Spawning socat..."
      sudo socat unix-listen://${DOCKER_SOCK},fork,user=$(id -un),group=$(id -gn),umask=027 ${DOCKER_HOST//\/\//} &
      echo "PID is $!"
      sleep 1
      if [[ -S $DOCKER_SOCK ]]; then
        echo "Socket created."
      else
        echo "Error creating socket."
      fi
    else
      echo "Could not find \"socat\" command, will not be able to create docker socket file."
      echo "Run \"sudo apt-get install socat\" to install socat and enable docker in the devel environment."
    fi
  fi
}

function set_user_id_var() {
  # Detect user ID for $USER_N
  if [[ -z $USER_ID ]]; then
    USER_ID=$(id -u $USER_N 2>/dev/null)
  fi
  # Fall through to $USER if $USER_N does not exist
  if [[ -z $USER_ID ]]; then
    USER_ID=$(id -u $USER 2>/dev/null)
  fi
}

function set_group_vars() {
  # Detect group
  if [[ -z $GROUP ]]; then
    GROUP=$(id -gn $USER_N 2>/dev/null)
  fi
  # Fall through to $USER if $USER_N does not exist
  if [[ -z $GROUP ]]; then
    GROUP=$(id -gn $USER 2>/dev/null)
  fi

  # Detect group ID
  if [[ -z $GROUP_ID && ! -z $GROUP ]]; then
    case $HOST_PLATFORM in
      LINUX|WSL)
        GROUP_ID=$(getent group $GROUP | cut -d: -f3)
        ;;
      MACOS)
        GROUP_ID=$(dscl . -read /Groups/${GROUP} 2>/dev/null | awk '($1 == "PrimaryGroupID:") { print $2 }')
        ;;
      *)
        ;;
    esac
  fi
}

function set_user_full_name_var() {
  # Get full user name
  USER_FULL_NAME=$(get_full_name_for_user $USER_N)
  if [[ -z $USER_FULL_NAME ]]; then
    USER_FULL_NAME=$(get_full_name_for_user $USER)
  fi
}

function get_full_name_for_user() {
  case $HOST_PLATFORM in
    LINUX|WSL)
      getent passwd $1 | cut -d ':' -f 5 | cut -d ',' -f 1
      ;;
    MACOS)
      id -F $1 2>/dev/null
      ;;
    *)
      echo "Devel User"
      ;;
  esac
}

function start_apt_cacher() {
  if ! is_container_running devel_env_apt_cacher; then
    docker build -t devel_env_apt_cacher \
      -f "$(get_script_dir)/docker/apt-cacher.Dockerfile" \
      "$(get_script_dir)/docker"
  
    docker run --rm -d \
      -p 3142:3142 \
      -v apt_cacher_cache:/var/cache/apt-cacher-ng \
      --name devel_env_apt_cacher \
      devel_env_apt_cacher
  fi
}

function stop_apt_cacher() {
  docker container stop devel_env_apt_cacher
}

function get_apt_cacher_ip() {
  if is_container_running devel_env_apt_cacher; then
    docker container inspect --format="{{.NetworkSettings.IPAddress}}" devel_env_apt_cacher 2> /dev/null
  else
    return 1;
  fi
}

function start_docker_env() {
  require docker
  if [[ $HOST_PLATFORM == "WSL" ]]; then
    setup_docker_on_wsl
  fi

  if [[ $REMOTE ]]; then
    DOCKER_RUN_ARGS+=("-p" "127.0.0.1:7100-7110:7100-7110")
  fi

  # Detect user ID, group and group ID, and full name
  set_user_id_var
  set_group_vars
  set_user_full_name_var

  # Set Docker build args
  DOCKER_BUILD_ARGS=()

  # Set user
  DOCKER_BUILD_ARGS+=("--build-arg" "user=$USER_N")

  if [[ ! -z $USER_ID ]]; then
    DOCKER_BUILD_ARGS+=("--build-arg" "user_id=$USER_ID")
  fi
  if [[ ! -z $GROUP ]]; then
    DOCKER_BUILD_ARGS+=("--build-arg" "group=$GROUP")
  fi
  if [[ ! -z $GROUP_ID ]]; then
    DOCKER_BUILD_ARGS+=("--build-arg" "group_id=${GROUP_ID}")
  fi
  if [[ ! -z $USER_FULL_NAME ]]; then
    DOCKER_BUILD_ARGS+=("--build-arg" "user_name=\"$USER_FULL_NAME\"")
  fi

  ENV_NAME=${USER_N}_devel_env

  # Look for running environment
  if is_container_running $ENV_NAME; then
    echo "Running shell in existing devel environment"
    exec_shell $ENV_NAME
  else
    if is_container_existing $ENV_NAME; then
      echo "Removing stale container"
      remove_container $ENV_NAME
    fi
    if ! build_image $ENV_NAME; then
      echo "Error building devel environment"
      if ! is_image_existing $ENV_NAME; then
        exit 1
      fi

      echo "Found a previously built image ($ENV_NAME), do you want to try running with that image?"
      select yn in "Yes" "No"; do
        case $yn in
          "Yes")
            break
            ;;
          "No")
            exit 1
            ;;
        esac
      done
    fi

    echo "Starting devel environment"
    create_container $ENV_NAME
    exec_shell $ENV_NAME
  fi

  # If this is the last shell (exit code 0 from exec_shell) then shut down container
  if [[ $? == 0 ]]; then
    echo "Shutting down devel environment"
    stop_container $ENV_NAME
  fi
}

function start_native_env() {
  bash "$(get_script_dir)/update_packages.sh"
  if [[ $? != 0 ]]; then
    exit 1
  fi

  DEVEL_SHELL=1 /usr/local/bin/devel_init.sh
}

check_for_updates "$@"
check_not_devel_env

HOST_PLATFORM=$(get_platform)

while getopts ":d:hi:nru:" opt; do
  case $opt in
    d)
      SRC_DIR_ARG=$OPTARG
      ;;
    h)
      print_usage
      ;;
    i)
      USER_ID=${OPTARG%%:*}
      if [[ $OPTARG =~ ":" ]]; then
        GROUP_ID=${OPTARG#*:}
      fi
      ;;
    n)
      NATIVE=1
      ;;
    r)
      REMOTE=1
      ;;
    u)
      USER_N=${OPTARG%%:*}
      if [[ $OPTARG =~ ":" ]]; then
        GROUP=${OPTARG#*:}
      fi
      ;;
    \?)
      echo -e "Error, invalid option (-$OPTARG)\n"
      print_usage
      ;;
  esac
done

# Remove processed arguments
shift $(($OPTIND - 1))
# Preserve quoted strings in extra arguments by using array
DOCKER_RUN_ARGS=()
while [[ $# > 0 ]]; do
  DOCKER_RUN_ARGS+=("$1")
  shift
done

if [[ -z $USER_N ]]; then
  echo -e "Error, user must be specified with -u flag\n"
  print_usage
fi

if [[ $NATIVE ]]; then
  start_native_env
else
  start_docker_env
fi
