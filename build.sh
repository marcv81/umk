#!/bin/bash
set -e
set -x

# Clean
rm -f firmware/*.o firmware/*.elf firmware/*.hex contrib/*.o

CFLAGS="\
-mmcu=atmega32u4 \
-DF_CPU=16000000UL \
-DF_OSC=16000000UL \
-DMILLIS_TIMER=MILLIS_TIMER1 \
-Os \
-std=c99"

INCS="-Ifirmware -Icontrib"

# Compile
function compile { avr-gcc -c $CFLAGS $INCS $1.c -o $1.o; }
compile firmware/firmware
compile firmware/led
compile firmware/matrix_right
compile firmware/matrix_left
compile firmware/matrix
compile firmware/debounce
compile firmware/controller
compile contrib/usb_keyboard
compile contrib/twimaster
compile contrib/millis

# Link
avr-gcc $CFLAGS \
firmware/firmware.o \
firmware/led.o \
firmware/matrix_right.o \
firmware/matrix_left.o \
firmware/matrix.o \
firmware/debounce.o \
firmware/controller.o \
contrib/usb_keyboard.o \
contrib/twimaster.o \
contrib/millis.o \
-o firmware/firmware.elf

# Translate
avr-objcopy \
-R .eeprom -R .fuse -R .lock -R .signature \
-O ihex firmware/firmware.elf firmware/firmware.hex
