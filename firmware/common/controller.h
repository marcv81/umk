#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdint.h>

void controller_init();
void controller_update();

void controller_on_pressed(uint8_t key);
void controller_on_released(uint8_t key);

#endif // CONTROLLER_H
