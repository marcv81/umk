# Micro Mechanical Keyboard (uMK)

This is a minimal keyboard firmware for the ATmega32U4.

It is not recommended for casual users; other firmwares have more features. It is however a good choice for learning about keyboards and microcontrollers.

## Supported keyboards

- XD75

# Hacking

## Build

Please install the AVR toolchain and Python 3, or build in the provided Docker container.

    ./builder.sh build/<firmware>.sh

## Flash

    dfu-programmer atmega32u4 erase
    dfu-programmer atmega32u4 flash <firmware>.hex
