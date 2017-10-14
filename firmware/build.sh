#!/bin/bash
set -x

# Clean
rm -f *.o *.elf *.hex

CFLAGS="\
-mmcu=atmega32u4 \
-DF_CPU=16000000UL \
-DF_OSC=16000000UL \
-Os"

# Compile
function compile { avr-gcc -c $CFLAGS $1.c -o $1.o; }
compile firmware
compile led

# Link
avr-gcc $CFLAGS firmware.o led.o -o firmware.elf

# Translate
avr-objcopy \
-R .eeprom -R .fuse -R .lock -R .signature \
-O ihex firmware.elf firmware.hex
