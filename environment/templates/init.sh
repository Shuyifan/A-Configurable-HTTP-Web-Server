#!/bin/bash

slash_escape() {
  echo ${1//\//\\/}
}

substitute_vars() {
  TEMP_FILE=template.tmp
  TEMPLATE_FILE=$1
  shift
  OUTPUT_FILE=$1
  shift
  SUBSTITUTIONS=$*

  if [[ $SUBSTITUTIONS ]]; then
    EXPRS=()

    for SUBSTITUTION in $SUBSTITUTIONS; do
        KEY=$(slash_escape ${SUBSTITUTION%%=*})
        VALUE=$(slash_escape ${SUBSTITUTION#*=})
        EXPRS+=("-e s/%%${KEY}%%/${VALUE}/g")
    done

    sed ${EXPRS[*]} "$TEMPLATE_FILE" > "$TEMP_FILE"

    if [[ ! $FORCE && -e "$OUTPUT_FILE" ]]; then
      diff -u "$OUTPUT_FILE" "$TEMP_FILE"
      if [[ $? == 0 ]]; then
        # No diff, so skip this file
        echo "Skipping '${OUTPUT_FILE}'... (unchanged)"
        rm "$TEMP_FILE"
      else
        echo "'${OUTPUT_FILE}' already exists. Do you wish to overwrite with the above changes?"
        select yn in "Yes" "No"; do
          case $yn in
            Yes)
              echo "Writing '${OUTPUT_FILE}'..."
              cp "$TEMP_FILE" "$OUTPUT_FILE"
              rm "$TEMP_FILE"
              break
              ;;
            No)
              echo "Skipping '${OUTPUT_FILE}'..."
              rm "$TEMP_FILE"
              break
              ;;
          esac
        done
      fi
    else
      echo "Writing '${OUTPUT_FILE}'..."
      cp "$TEMP_FILE" "$OUTPUT_FILE"
      rm "$TEMP_FILE"
    fi
  fi
}

print_usage() {
  echo "Syntax: $0 [-h] [-f] [-d REPO_DIR] [-n IMAGE_NAME]"
  exit 1
}

FORCE=

REPO_DIR=$(pwd)

while getopts ":hfd:n:" opt; do
  case $opt in
    h)
      print_usage
      ;;
    f)
      FORCE=1
      ;;
    d)
      REPO_DIR=$OPTARG
      ;;
    n)
      IMAGE_NAME=$OPTARG
      ;;
    \?)
      echo "Invalid option (-$OPTARG)"
      print_usage
      ;;
  esac
done

if [[ ! -d "$REPO_DIR/.git" ]]; then
  echo "Could not find a git repository in \"${REPO_DIR}\". Templates should be generated at the root of a git repository. Either run this script from a repository directory or specify a directory with -d."
  exit 1
fi

# Use the repository name as the image name if none is given.
if [[ -z $IMAGE_NAME ]]; then
  IMAGE_NAME=${REPO_DIR##*/}
fi

SCRIPT_DIR=${0%/*}

VARIABLES=("IMAGE_NAME=$IMAGE_NAME")

# ex: a/tools/templates/src
TEMPLATE_PREFIX=${SCRIPT_DIR}/src

for TEMPLATE_PATH in `find ${TEMPLATE_PREFIX} -print`; do
  if [[ $TEMPLATE_PATH == $TEMPLATE_PREFIX ]]
  then
    continue
  fi
  # ex: TEMPLATE_PATH=a/tools/templates/src/b/_c
  # ex: _c
  TEMPLATE_FILE=${TEMPLATE_PATH##*/}
  # ex: c
  OUTPUT_PATH_FILE=${TEMPLATE_FILE#_}
  # ex: a/tools/templates/src/b
  TEMPLATE_DIR=${TEMPLATE_PATH%/*}
  # ex: /b
  TEMPLATE_REL_DIR=${TEMPLATE_DIR#${TEMPLATE_PREFIX}}
  # ex: out/b
  OUTPUT_PATH_DIR=${REPO_DIR}${TEMPLATE_REL_DIR}
  # ex: out/b/c
  OUTPUT_PATH=${OUTPUT_PATH_DIR}/${OUTPUT_PATH_FILE}
  if [[ -d "$TEMPLATE_PATH" ]]; then
    if [[ ! -e "$OUTPUT_PATH" ]]; then
      mkdir -p "$OUTPUT_PATH"
    fi
  else
    substitute_vars "$TEMPLATE_PATH" "$OUTPUT_PATH" ${VARIABLES[*]}
  fi
done
