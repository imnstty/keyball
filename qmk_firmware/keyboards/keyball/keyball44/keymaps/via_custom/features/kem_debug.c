/*
Copyright 2026 Tetsuya Imanishi
*/

/******************************************************************************
 * @file      kem_debug.c
 * @project   Keyball44 Custom Firmware
 * @brief     KEM Debug Engine
 *
 * @version   2.10
 * @date      2026-07-07
 *
 *-----------------------------------------------------------------------------
 * Revision History
 *-----------------------------------------------------------------------------
 * Ver 2.12  2026-07-07
 * - Routed Debug Engine output through Debug Output Driver.
 * - No functional changes.
 *
 * Ver 2.11  2026-07-07
 * - Added Debug Event Handler API.
 * - Routed key event debug records through kem_debug_handle_event().
 * - No functional changes.
 *
 * Ver 2.10  2026-07-07
 * - Added KEM Debug Framework skeleton.
 * - Added key event debug structure and no-op send function.
 * - No functional changes.
 *
 ******************************************************************************/

#include "kem_debug.h"
#include "kem_debug_output.h"

static bool s_debug_enabled = false;

void kem_debug_handle_event(const kem_debug_event_t *event)
{
    if (!event)
    {
        return;
    }

    if (!s_debug_enabled)
    {
        return;
    }

    kem_debug_send(event);
}

void kem_debug_send(const kem_debug_event_t *event)
{
    // Debug output is not implemented yet.
    // This function intentionally does nothing.
    kem_debug_output_send(event);
}

void kem_debug_record_key_event(uint16_t keycode, keyrecord_t *record)
{
    if (!record)
    {
        return;
    }

    kem_debug_event_t event = {
        .type = KEM_DEBUG_EVENT_KEY,
        .keycode = keycode,
        .pressed = record->event.pressed,
        .row = record->event.key.row,
        .col = record->event.key.col,
        .time = timer_read(),
        .led = 255,
    };

    kem_debug_handle_event(&event);
}

void kem_debug_record_key_event_with_led(uint16_t keycode, keyrecord_t *record, uint8_t led)
{
    if (!record)
    {
        return;
    }

    kem_debug_event_t event = {
        .type = KEM_DEBUG_EVENT_KEY,
        .keycode = keycode,
        .pressed = record->event.pressed,
        .row = record->event.key.row,
        .col = record->event.key.col,
        .time = timer_read(),
        .led = led,
    };

    kem_debug_handle_event(&event);
}

void kem_debug_record_matrix_event(uint8_t row,
                                   uint8_t col,
                                   bool pressed)
{
    kem_debug_event_t event = {
        .type = KEM_DEBUG_EVENT_KEY,
        .keycode = 0xFFFF, // Matrix Event識別用
        .pressed = pressed,
        .row = row,
        .col = col,
        .time = timer_read(),
        .led = 255,
    };

    kem_debug_handle_event(&event);
}

void kem_debug_record_matrix_event_with_led(uint8_t row,
                                            uint8_t col,
                                            bool pressed,
                                            uint8_t led)
{
    kem_debug_event_t event = {
        .type = KEM_DEBUG_EVENT_KEY,
        .keycode = 0xFFFF,
        .pressed = pressed,
        .row = row,
        .col = col,
        .time = timer_read(),
        .led = led,
    };

    kem_debug_handle_event(&event);
}

void kem_debug_enable(bool enable)
{
    s_debug_enabled = enable;
}

bool kem_debug_is_enabled(void)
{
    return s_debug_enabled;
}

void kem_debug_toggle(void)
{
    s_debug_enabled = !s_debug_enabled;
}