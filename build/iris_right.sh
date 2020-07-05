#!/bin/bash

set -euo pipefail

# Clean
rm -rf output
mkdir -p output

cflags=("-mmcu=atmega32u4" "-DF_CPU=16000000UL" "-DF_OSC=16000000UL" "-Os" "-fshort-enums" "-std=c99")
incs=("-Ifirmware/common" "-Ifirmware/iris" "-Ioutput")

# Compile
function compile { avr-gcc -c "${cflags[@]}" "${incs[@]}" "$1/$2.c" -o "output/$2.o"; }
compile firmware/iris main_right
compile firmware/iris wiring
compile firmware/common i2c_slave

# Link
avr-gcc "${cflags[@]}" output/*.o -o output/iris_right.elf

# Translate
avr-objcopy \
-R .eeprom -R .fuse -R .lock -R .signature \
-O ihex output/iris_right.elf output/iris_right.hex

# Summary
avr-size output/iris_right.hex
