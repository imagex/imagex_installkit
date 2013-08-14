#!/bin/bash

function display_help() {
  echo "Usage: command [arg, [arg2, [arg3 ...]]]"
  echo "Usage: build path-to/drush.make path-to/destination-dir"
  exit 0
}

function build_from_make() {
  if [[ ! -f $1 ]] || [[ ! -r $1 ]]; then
    echo "Error: The drush make file does not appear to be a file."
    exit 1
  fi

  if [[ -e $2 ]]; then
    echo "Error: The build path appears to exist."
    exit 1
  fi

  drush make $1 $2 --concurrency=1 --prepare-install
  echo "Build complete."
}


if [[ $# -lt 1 || $1 == "--help" || $1 == "-h" ]]; then
  display_help
fi

case $1 in
  build)
    if [[ -f $2 ]] && [[ $3 ]]; then
      build_from_make $2 $3;
    fi
    ;;
  *)
    display_help
    ;;
esac