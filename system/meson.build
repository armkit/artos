
# Project declaration
project('artos-img', 'c', version: '1.0', license : 'GPL2')

# sources
sources=['boot/main.c']

# create operating system image as an ELF library
libname  = 'artos'
libname += '-'
libname += meson.get_cross_property('machine')
libname += '-'
libname += meson.project_version()
lib = library(libname,
              sources,
              name_prefix: '',
              name_suffix: 'so')

# convert to UEFI PE executable
objname  = libname + '.efi'
command  = [find_program('objcopy')]
command += meson.get_cross_property('objcopy_args')
command += [lib.full_path()]
command += [objname]
custom_target(objname,
              output: objname,
              depends: lib,
              command: command,
              build_by_default: true)
