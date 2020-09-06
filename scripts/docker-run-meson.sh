#!/usr/bin/env bash

set -e

cd /artos

rm -rf build
for board in `ls boards`
do
  if [ -f boards/$board/config.ini ]
  then
    printf "\033[36;1m"
    echo ""
    echo "================================================================="
    echo "                  Building for board: $board"
    echo "                         ./build/$board"
    echo "================================================================="
    echo ""
    printf "\033[0m"
    unbuffer meson build/$board --cross-file boards/$board/config.ini
    ninja -C build/$board
    printf "\033[32;1m"
    wine64 build/$board/*.exe 2> /dev/null
    printf "\033[0m"
  fi
done
echo ""
