
# Project declaration
project('artos-emu', 'c', version: '1.0', license : 'GPL2')

# sources
sources=['main.c', 'binaries.S']

# create emulator program
exename  = 'artos'
exename += '-'
exename += meson.get_cross_property('machine')
exename += '-'
exename += meson.project_version()
executable(exename,
           sources,
           name_prefix: '',
           name_suffix: 'exe')
