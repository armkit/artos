#!/usr/bin/env bash

set -e

cd /
date
dpkg --add-architecture i386
dpkg --add-architecture arm64

apt-get -y update 

apt-get install -y wget
apt-get install -y software-properties-common
apt-get install -y ca-certificates
apt-get install -y gnupg
apt-get install -y p7zip-full
apt-get install -y dos2unix
apt-get install -y git
apt-get install -y nano
apt-get install -y vim
apt-get install -y hexedit
apt-get install -y python3
apt-get install -y autoconf
apt-get install -y automake
apt-get install -y meson
apt-get install -y ninja-build
apt-get install -y build-essential
apt-get install -y binutils-aarch64-linux-gnu
apt-get install -y gcc-aarch64-linux-gnu
apt-get install -y mingw-w64
apt-get install -y mingw-w64-tools
apt-get install -y gnu-efi:arm64
apt-get install -y bison
apt-get install -y doxygen
apt-get install -y gettext
apt-get install -y qemu-system
apt-get install -y wine
apt-get install -y wine32

wget -O qemu-w64-setup.exe https://qemu.weilnetz.de/w64/2020/qemu-w64-setup-20200814.exe
7z x -oqemu-setup qemu-w64-setup.exe
mkdir qemu
cp qemu-setup/qemu-system-aarch64.exe qemu/
cp qemu-setup/*.dll qemu/
cp qemu-setup/*.rom qemu/
tar -cf /usr/share/qemu-win.tar qemu
rm -rf qemu
rm -rf qemu-setup
rm -rf qemu-w64-setup.exe

wineboot -u

sleep 20

echo DONE