#!/usr/bin/env bash

set -e

cd /artos

rm -rf build
for board in `ls boards`
do
  if [ -f boards/$board/config.ini ]
  then
    echo "\033[36;1m"
    echo "================================================================="
    echo "                  Building for board: $board"
    echo "                         ./build/$board"
    echo "================================================================="
    echo "\033[0m"
    meson build/$board --cross-file boards/$board/config.ini
    ninja -C build/$board
    wine64 build/$board/*.exe 2> /dev/null
  fi
done
echo ""
