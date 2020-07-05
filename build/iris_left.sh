#!/bin/bash

set -euo pipefail

# Clean
rm -rf output
mkdir -p output

# Generate keymap
python3 keymap/iris.py > output/keymap.inc

cflags=("-mmcu=atmega32u4" "-DF_CPU=16000000UL" "-DF_OSC=16000000UL" "-Os" "-fshort-enums" "-std=c99")
incs=("-Ifirmware/common" "-Ifirmware/iris" "-Ioutput")

# Compile
function compile { avr-gcc -c "${cflags[@]}" "${incs[@]}" "$1/$2.c" -o "output/$2.o"; }
compile firmware/iris main_left
compile firmware/iris matrix
compile firmware/iris wiring
compile firmware/common core
compile firmware/common debouncer
compile firmware/common i2c_master
compile firmware/common keymap
compile firmware/common keys_list
compile firmware/common layers
compile firmware/common report_builder
compile firmware/common usb

# Link
avr-gcc "${cflags[@]}" output/*.o -o output/iris_left.elf

# Translate
avr-objcopy \
-R .eeprom -R .fuse -R .lock -R .signature \
-O ihex output/iris_left.elf output/iris_left.hex

# Summary
avr-size output/iris_left.hex
