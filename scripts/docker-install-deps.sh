#!/usr/bin/env bash

set -e

cd /

dpkg --add-architecture arm64

apt-get -y update

apt-get install -y ca-certificates                  \
                   wget                             \
                   p7zip-full                       \
                   python3                          \
                   python3-pip                      \
                   meson                            \
                   ninja-build                      \
                   binutils                         \
                   gcc                              \
                   binutils-aarch64-linux-gnu       \
                   gcc-aarch64-linux-gnu            \
                   mingw-w64                        \
                   gdb-multiarch                    \
                   gnu-efi:arm64                    \
                   expect                           \
                   qemu-system                      \
                   wine                             \
                   ;

pip3 install greenlet==0.4.14
pip3 install pygdbmi==0.10.0.0
pip3 install gdbgui==0.14.0.0

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
