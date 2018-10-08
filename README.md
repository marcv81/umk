# Micro Mechanical Keyboard (uMK)

This is a minimal keyboard firmware for the ATmega32U4.

It is not recommended for casual users; other firmwares have more features. It is however a good choice for learning about keyboards and microcontrollers.

## Supported keyboards

- Iris
- XD75

# Hacking

## Build

Please install the AVR toolchain and Python 3, or build in the provided Docker container.

    ./builder.sh build/<firmware>.sh

## Flash

### DFU

    dfu-programmer atmega32u4 erase
    dfu-programmer atmega32u4 flash <firmware>.hex

### AVRDude

    avrdude -c avr109 -p m32u4 -P /dev/ttyACM<N> -U flash:w:output/<firmware>.hex

## Dev

### Hardware support

The following is required to support a new keyboard.

- A build script in `build/`.
- A keymap in `keymap/`.
- A directory in `firmware/`. Define the wiring according to the pins connected to the matrix rows and columns, and to the diodes orientation. Wrap the wiring in a matrix to expose a common interface to the core.

### Features

The logic is implemented in loosely coupled translation units. The core hooks them up together. Follow the same pattern to implement custom features.

### Keymaps

A Python script in `keymap/` generates the keymap for each board. Edit it to remap the keys. Add new key types to trigger custom features.
