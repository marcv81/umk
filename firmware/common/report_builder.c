#include "report_builder.h"

#include <stdbool.h>

static struct {
    // Target report
    usb_report_t *report;
    // Current number of usage codes in the report
    uint8_t size;
} report_builder;

void report_builder_init(usb_report_t *report)
{
    report_builder.report = report;
}

void report_builder_reset()
{
    report_builder.size = 0;

    // Empty the report
    report_builder.report->modifiers = 0;
    for (uint8_t i=0; i<6; i++)
    {
        report_builder.report->keys[i] = 0;
    }
}

// Return if there is no more space for usage codes
static bool is_full()
{
    return (report_builder.size >= 6);
}

// Return if the code is already in the report
static bool has_key(uint8_t code)
{
    for (uint8_t i=0; i<report_builder.size; i++)
    {
        if (report_builder.report->keys[i] == code) return true;
    }
    return false;
}

void report_builder_add_key(uint8_t code)
{
    if (is_full() || has_key(code)) return;
    report_builder.report->keys[report_builder.size++] = code;
}

void report_builder_add_modifier(uint8_t mask)
{
    report_builder.report->modifiers |= mask;
}
