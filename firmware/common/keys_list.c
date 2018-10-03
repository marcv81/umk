#include "keys_list.h"

#include "config.h"
#include <stdbool.h>

static struct {
    uint8_t keys[MATRIX_KEYS];
    uint8_t size;
} list;

void keys_list_add(uint8_t key)
{
    if (list.size == MATRIX_KEYS) return;
    list.keys[list.size] = key;
    list.size++;
}

void keys_list_remove(uint8_t key)
{
    uint8_t i = 0;

    // Find the index of the key
    // Or return if it is not in the list
    while (true)
    {
        if (i == MATRIX_KEYS) return;
        if (list.keys[i] == key) break;
        i++;
    }

    // Left-shift all the keys with a higher index
    // This deletes (the first occurrence of) the key and leaves no gap
    while (i < list.size-1)
    {
        list.keys[i] = list.keys[i+1];
        i++;
    }

    list.size--;
}

void keys_list_iterate(void (*func)(uint8_t key))
{
    for (uint8_t i=0; i<list.size; i++)
    {
        func(list.keys[i]);
    }
}
