#!/bin/bash
set -e
set -x

# Clean
rm -rf output
mkdir -p output

# Generate keymap
python3 keymap/build_ergodox.py > output/keymap.c

CFLAGS="\
-mmcu=atmega32u4 \
-DF_CPU=16000000UL \
-DF_OSC=16000000UL \
-Os \
-std=c99"

INCS="-Ifirmware/common -Ifirmware/ergodox -Ioutput"

# Compile
function compile { avr-gcc -c $CFLAGS $INCS $1/$2.c -o output/$2.o; }
compile firmware/ergodox main
compile firmware/ergodox led
compile firmware/ergodox matrix_right
compile firmware/ergodox matrix_left
compile firmware/ergodox matrix
compile firmware/common controller
compile firmware/common keys
compile firmware/common usb
compile firmware/common i2c

# Link
avr-gcc $CFLAGS output/*.o -o output/ergodox.elf

# Translate
avr-objcopy \
-R .eeprom -R .fuse -R .lock -R .signature \
-O ihex output/ergodox.elf output/ergodox.hex

# Summary
avr-size output/ergodox.hex
