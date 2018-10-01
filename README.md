# Micro Mechanical Keyboard (uMK)

This is a minimal keyboard firmware for the ATmega32U4.

It is not recommended for casual users; other firmwares have more features. It is however a good choice for learning about keyboards and microcontrollers.

## Supported keyboards

- XD75

# Hacking

## Build

Please install the AVR toolchain and Python 3, or build in the provided Docker container.

    docker build docker -t umk-builder
    docker run -it --rm -v `pwd`:/umk umk-builder

There is a build script for each of the supported keyboards.

## Flash

    dfu-programmer atmega32u4 erase
    dfu-programmer atmega32u4 flash <firmware.hex>
