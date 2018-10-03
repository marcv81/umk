#ifndef KEYS_LIST_H
#define KEYS_LIST_H

#include <stdint.h>

void keys_list_add(uint8_t key);
void keys_list_remove(uint8_t key);
void keys_list_iterate(void (*func)(uint8_t key));

#endif // KEYS_LIST_H
