#!/bin/bash
set -e
set -x

# Clean
rm -f firmware/*.o firmware/*.elf firmware/*.hex contrib/*.o

CFLAGS="\
-mmcu=atmega32u4 \
-DF_CPU=16000000UL \
-DF_OSC=16000000UL \
-Os \
-std=c99"

INCS="-Ifirmware -Icontrib"

# Compile
function compile { avr-gcc -c $CFLAGS $INCS $1.c -o $1.o; }
compile firmware/firmware
compile firmware/led
compile firmware/matrix_right
compile contrib/usb_keyboard

# Link
avr-gcc $CFLAGS \
firmware/firmware.o \
firmware/led.o \
firmware/matrix_right.o \
contrib/usb_keyboard.o \
-o firmware/firmware.elf

# Translate
avr-objcopy \
-R .eeprom -R .fuse -R .lock -R .signature \
-O ihex firmware/firmware.elf firmware/firmware.hex