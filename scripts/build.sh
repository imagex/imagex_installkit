#!/bin/bash

CURRENT_DIR=$(pwd)
BUILDER_DIR="$CURRENT_DIR/.builder"

function display_help() {
  echo "Usage: command [arg, [arg2, [arg3 ...]]] --opt1 --opt2"
  exit 0
}

function init() {
  if [ builder_is_inited ]; then
    echo "Initializing builder directory."
    mkdir "$BUILDER_DIR"

    echo "> Cloning Git Repository: $1"
    git clone $1 "$BUILDER_DIR/git" &>/dev/null

    if [[ -f "$BUILDER_DIR/git/build-$2.make" ]]; then
      echo "> Making a copy of the $2 file."
      cp "$BUILDER_DIR/git/build-$2.make" "$BUILDER_DIR/build-$2.make" &>/dev/null
    else
      echo "Build file: $2 does not exist within cloned repository $1."
    fi

    echo "> Removing the cloned repository."
    rm -fR "$BUILDER_DIR/git"

    echo "Initialization complete."
  else
    echo "The builder directory is already initialized."
  fi
}

function build() {
  local build_file="$BUILDER_DIR/build-$1.make"
  if [ -f "$build_file" ]; then
    if [ -e $2 ]; then
      echo "Error: The specified build path already exists."
      exit 1
    fi
  fi

  echo "> Executing drush make command, building project."
  drush make $build_file $2 --concurrency=1 --prepare-install &>/dev/null

  echo "Build complete."
}

function builder_is_inited() {
  if [ -d "$BUILDER_DIR" ]; then
    return 1
  else
    return 0
  fi
}

function cleanup() {
  if [ builder_is_inited ]; then
    echo "Removing the $CURRENT_DIR/builder directory."
    rm -fR "$BUILDER_DIR"
  fi
}

if [[ $# -lt 1 || $1 == "--help" || $1 == "-h" ]]; then
  display_help
fi

builder_is_inited

case $1 in
  init)
    # ./build.sh git@github.com:example/repo.git project
    init $2 $3
    ;;
  build)
    # ./build.sh project build/path/here
    if [ $2 ] && [ $3 ]; then
      build $2 $3;
    fi
    ;;
  cleanup)
    # removes the builder directory.
    cleanup
    ;;
  *)
    display_help
    ;;
esac