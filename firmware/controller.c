#include "controller.h"

#include <stdint.h>

#include "matrix.h"
#include "usb_keyboard.h"

// Modifiers layout
#define LEFT_CTRL 42
#define RIGHT_CTRL 55
#define LEFT_SHIFT 28
#define RIGHT_SHIFT 41
#define LEFT_GUI 14
#define RIGHT_GUI 27

struct
{
    uint8_t keys[6];
    uint8_t modifiers;
} controller =
{
    .keys = { 0, 0, 0, 0, 0, 0 },
    .modifiers = 0,
};

// Keys layout
static const uint8_t scancodes[84] =
{
    0, 0, KEY_TAB, KEY_SPACE, 0, 0, 0, 0, 0, 0, KEY_BACKSPACE, KEY_ENTER, 0, 0,
    0, KEY_TILDE, KEY_BACKSLASH, KEY_LEFT, KEY_RIGHT, 0, 0, 0, 0, KEY_DOWN, KEY_UP, KEY_LEFT_BRACE, KEY_RIGHT_BRACE, 0,
    0, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, 0, 0, KEY_N, KEY_M, KEY_COMMA, KEY_PERIOD, KEY_SLASH, 0,
    0, KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, 0, 0, KEY_H, KEY_J, KEY_K, KEY_L, KEY_SEMICOLON, 0,
    KEY_QUOTE, KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, 0, 0, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P, KEY_MINUS,
    KEY_ESC, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, 0, 0, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0, KEY_EQUAL,
};

// Update the keys according to the matrix state
static void update_keys()
{
    // Reset the keys
    for (uint8_t i=0; i<6; i++)
    {
        controller.keys[i] = 0;
    }

    // Accumulate at most 6 scancodes
    uint8_t keys_index = 0;
    for (uint8_t i=0; i<MATRIX_KEYS; i++)
    {
        // Skip the keys with no scancode
        if (scancodes[i] == 0) continue;

        // Skip the keys which are not pressed
        if (!matrix.list[i]) continue;

        if (keys_index < 6)
        {
            controller.keys[keys_index++] = scancodes[i];
        }
    }
}

// Update the modifiers according to the matrix state
static void update_modifiers()
{
    // Reset the modifiers
    controller.modifiers = 0;

    // Update control
    if (matrix.list[LEFT_CTRL] || matrix.list[RIGHT_CTRL])
    {
        controller.modifiers |= KEY_CTRL;
    }

    // Update shift
    if (matrix.list[LEFT_SHIFT] || matrix.list[RIGHT_SHIFT])
    {
        controller.modifiers |= KEY_SHIFT;
    }

    // Update GUI
    if (matrix.list[LEFT_GUI] || matrix.list[RIGHT_GUI])
    {
        controller.modifiers |= KEY_GUI;
    }
}

// Send an update over USB if the keys or modifiers changed
void update_usb()
{
    uint8_t updated = 0;

    // Test if the keys changed
    for (uint8_t i=0; i<6; i++)
    {
        if (controller.keys[i] != keyboard_keys[i])
        {
            keyboard_keys[i] = controller.keys[i];
            updated = 1;
        }
    }

    // Test if the modifiers changed
    if (controller.modifiers != keyboard_modifier_keys)
    {
        keyboard_modifier_keys = controller.modifiers;
        updated = 1;
    }

    if (updated) usb_keyboard_send();
}

void controller_update()
{
    update_keys();
    update_modifiers();
    update_usb();
}
