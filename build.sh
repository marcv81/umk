#!/bin/bash
set -e
set -x

# Clean
rm -f firmware/*.o firmware/*.elf firmware/*.hex firmware/keymap.c

# Generate keymap
python3 keymap/build.py > firmware/keymap.c

CFLAGS="\
-mmcu=atmega32u4 \
-DF_CPU=16000000UL \
-DF_OSC=16000000UL \
-Os \
-std=c99"

INCS="-Ifirmware"

# Compile
function compile { avr-gcc -c $CFLAGS $INCS $1.c -o $1.o; }
compile firmware/firmware
compile firmware/led
compile firmware/matrix_right
compile firmware/matrix_left
compile firmware/matrix
compile firmware/controller
compile firmware/usb
compile firmware/i2c

# Link
avr-gcc $CFLAGS \
firmware/firmware.o \
firmware/led.o \
firmware/matrix_right.o \
firmware/matrix_left.o \
firmware/matrix.o \
firmware/controller.o \
firmware/usb.o \
firmware/i2c.o \
-o firmware/firmware.elf

# Translate
avr-objcopy \
-R .eeprom -R .fuse -R .lock -R .signature \
-O ihex firmware/firmware.elf firmware/firmware.hex
