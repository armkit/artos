# Installation Instructions

For Microsoft Windows
=====================

Prerequisites
-------------
You only need to have **Docker Desktop** installed. You can find an installer
for Windows at this URL:
 * https://hub.docker.com/editions/community/docker-ce-desktop-windows/

Building
--------
You can simply run `build.bat` from **Windows NT Command Prompt**,
**Windows PowerShell**, or even by just double-clicking on `build.bat`
icon from **Windows Shell** GUI (**Windows Explorer**).

When you run `build.bat` for the first time, it will take like 10 minutes
to build the docker image that will be used to build **ARTOS**. This stage
downloads 500MB of Ubuntu packages. This is done only once. If you run
`build.bat` again, the data from docker cache will be used (unless Dockerfile
or docker scripts under `scripts/` directory change).

Emulation
---------
After build is done, you can find emulator executables (EXE files) under
`build/<board>/` directory, where `<board>` is the board that you would
like to emulate. The emulator will run **ARTOS** image (for the specified
board) on a customized **QEMU** instance.

The VIRT board is a customizable hypothetical machine for ARM that
**QEMU** is very good at emulating. You can find the EXE file under
`build/virt/` after building.

Running on real board
---------------------
You can find board images (EFI files) under `build/<board>/` directory.
The EFI image can be loaded to the board storage (e.g. MMC card), and UEFI
can be used to boot from that EFI image. You should figure it out by yourself.
