#!/bin/bash
set -e
set -x

# Clean
rm -rf output
mkdir -p output

# Generate keymap
python3 keymap/xd75.py > output/keymap.inc

CFLAGS="\
-mmcu=atmega32u4 \
-DF_CPU=16000000UL \
-DF_OSC=16000000UL \
-Os \
-fshort-enums \
-std=c99"

INCS="-Ifirmware/common -Ifirmware/xd75 -Ioutput"

# Compile
function compile { avr-gcc -c $CFLAGS $INCS $1/$2.c -o output/$2.o; }
compile firmware/xd75 main
compile firmware/xd75 matrix
compile firmware/xd75 rgb
compile firmware/xd75 wiring
compile firmware/common backlight
compile firmware/common core
compile firmware/common debouncer
compile firmware/common keymap
compile firmware/common keys_list
compile firmware/common layers
compile firmware/common report_builder
compile firmware/common usb

# Link
avr-gcc $CFLAGS output/*.o -o output/xd75.elf

# Translate
avr-objcopy \
-R .eeprom -R .fuse -R .lock -R .signature \
-O ihex output/xd75.elf output/xd75.hex

# Summary
avr-size output/xd75.hex
