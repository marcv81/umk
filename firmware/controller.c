#include "controller.h"

#include <stdint.h>

#include "config.h"
#include "matrix.h"
#include "usb.h"
#include "usb_keyboard.h"

// Modifiers layout
#define LEFT_CTRL 42
#define RIGHT_CTRL 55
#define LEFT_SHIFT 28
#define RIGHT_SHIFT 41
#define LEFT_GUI 14
#define RIGHT_GUI 27

static usb_keyboard_report_t report;

// Keys layout
static const uint8_t scancodes[MATRIX_KEYS] =
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
        report.keys[i] = 0;
    }

    // Accumulate at most 6 scancodes
    uint8_t keys_index = 0;
    for (uint8_t i=0; i<MATRIX_KEYS; i++)
    {
        // Skip the keys with no scancode
        if (scancodes[i] == 0) continue;

        // Skip the keys which are not pressed
        if (!matrix_pressed(i)) continue;

        if (keys_index < 6)
        {
            report.keys[keys_index++] = scancodes[i];
        }
    }
}

// Update the modifiers according to the matrix state
static void update_modifiers()
{
    // Reset the modifiers
    report.modifiers = 0;

    // Update control
    if (matrix_pressed(LEFT_CTRL) || matrix_pressed(RIGHT_CTRL))
    {
        report.modifiers |= KEY_CTRL;
    }

    // Update shift
    if (matrix_pressed(LEFT_SHIFT) || matrix_pressed(RIGHT_SHIFT))
    {
        report.modifiers |= KEY_SHIFT;
    }

    // Update GUI
    if (matrix_pressed(LEFT_GUI) || matrix_pressed(RIGHT_GUI))
    {
        report.modifiers |= KEY_GUI;
    }
}

// Send an update over USB if the keys or modifiers changed
void update_usb()
{
    uint8_t updated = 0;

    // Test if the keys changed
    for (uint8_t i=0; i<6; i++)
    {
        if (report.keys[i] != usb_keyboard_report.keys[i])
        {
            usb_keyboard_report.keys[i] = report.keys[i];
            updated = 1;
        }
    }

    // Test if the modifiers changed
    if (report.modifiers != usb_keyboard_report.modifiers)
    {
        usb_keyboard_report.modifiers = report.modifiers;
        updated = 1;
    }

    if (updated) usb_keyboard_report_update();
}

void controller_update()
{
    update_keys();
    update_modifiers();
    update_usb();
}
