#!/usr/bin/env bash

set -e

export WINEDLLOVERRIDES="mscoree,mshtml="

DIR="$( cd "$( dirname "$0" )" >/dev/null 2>&1 && pwd )"
cd $DIR
rm -rf build

for machine in `ls machines`
do
  if [ "$machine" = "rasp" ]
  then
    continue
  fi
  meson setup --cross-file machines/$machine/system.ini build/$machine/system system
  meson setup --cross-file machines/$machine/emulator.ini build/$machine/emulator emulator
  ninja -C build/$machine/system   | cat
  ninja -C build/$machine/emulator | cat
  cp build/$machine/system/*.efi build/$machine
  cp build/$machine/emulator/*.exe build/$machine
  wine build/$machine/*.exe | sed "s,\x1B\[[0-9;]*[a-zA-Z],,g"
done
