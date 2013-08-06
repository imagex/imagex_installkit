#!/bin/bash
# Usage: sh ./build.sh [arg1, [arg2], ...]
# Ensure that the script is executable prior to executing: chmod -x ./build.sh

build_ixm() {
  echo "Executing the IX install base build"
}

case $1 in
  build)
    if [[ -n $2 ]]; then
      BUILD_PATH=$2
    else
      echo "Usage: build.sh build [build_path]"
      exit 1;
    fi

    build_ixm $BUILD_PATH
    ;;
  *)
    echo "Usage: build.sh [command] [arg1, [arg2, [...]]]"
    exit 1;
    ;;
esac