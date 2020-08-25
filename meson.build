# Project declaration
project('artos', 'c', version: '21.04', license : 'GPL2')

# base name
basename  = meson.project_name()
basename += '-'
basename += meson.project_version()
basename += '-'
basename += host_machine.system()

# sources
sources=['boot/main.c']

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
command += [meson.source_root() + '/emulator/main.c']
exe = custom_target(exename,
                    output: exename,
                    depends: efi,
                    command: command,
                    build_by_default: true)
