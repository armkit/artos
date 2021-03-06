#**************************************************************************
#*
#*                   ARTOS Operating System.
#*                 Copyright (C) 2020  ARMKit.
#*
#***************************************************************************
#* @file   meson.build
#* @brief  Meson build instructions.
#***************************************************************************
#*
#* This program is free software; you can redistribute it and/or
#* modify it under the terms of the GNU General Public License
#* as published by the Free Software Foundation; either version 2
#* of the License, or (at your option) any later version.
#*
#* This program is distributed in the hope that it will be useful,
#* but WITHOUT ANY WARRANTY; without even the implied warranty of
#* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#* GNU General Public License for more details.
#*
#* You should have received a copy of the GNU General Public License
#* along with this program; if not, write to the Free Software
#* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
#* 02110-1301, USA.
#*
#*****************************************************************************

# Project declaration
project('artos', 'c', version: '21.04', license : 'GPL2')

# base name
basename  = meson.project_name()
basename += '-'
basename += meson.project_version()
basename += '-'
basename += host_machine.system()

# sources
sources=['boot/src/main.c',
         'boot/src/init.c',
         'boot/src/splash.c',
         'boot/src/memmap.c',
         'boot/src/exit.c',
         'port/src/serial.c',
         'port/src/translation.c',
         'port/src/thread.c',
         'kernel/src/core.c',
         'kernel/src/print.c',
         'kernel/src/memory.c',
         'kernel/src/process.c',
         'kernel/src/thread.c',
         'kernel/src/power.c']

# create operating system image as an ELF library
lib = library(basename, sources, name_prefix: '', name_suffix: 'so')

# convert to UEFI PE executable
efiname  = basename + '.efi'
command  = [find_program('objcopy')]
command += meson.get_cross_property('objcopy_args')
command += [lib.full_path()]
command += [efiname]
efi = custom_target(efiname,
                    output: efiname,
                    depends: lib,
                    command: command,
                    build_by_default: true)

# convert to an emulator executable
exename  = basename + '.exe'
command  = [find_program('mingw')]
command += ['-o', exename]
command += ['-DQEMU_FILE="' + meson.get_cross_property('qemu_file') + '"']
command += ['-DEFI_FILE="' + meson.get_cross_property('efi_file') + '"']
command += ['-DARTOS_FILE="' + efiname + '"']
command += [meson.source_root() + '/emulator/src/main.c']
exe = custom_target(exename,
                    output: exename,
                    depends: efi,
                    command: command,
                    build_by_default: true)
