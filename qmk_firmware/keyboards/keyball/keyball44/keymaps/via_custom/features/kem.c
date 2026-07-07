/*
Copyright 2026 Tetsuya Imanishi
*/

/******************************************************************************
 * @file      kem.c
 * @project   Keyball44 Custom Firmware
 * @brief     KEM (Keyball Extension Module) Core
 *
 * @version   2.10
 * @date      2026-07-07
 *
 *-----------------------------------------------------------------------------
 * Revision History
 *-----------------------------------------------------------------------------
 * Ver 2.10  2026-07-07
 * - Routed LED processing through Universal LED Engine event API.
 *
 * Ver 2.00  2026-07-04
 * - Added Layer Engine processing.
 * - Initial KEM Core.
 *
 ******************************************************************************/

#include "kem.h"
#include "kem_layer.h"
#include "kem_led.h"

bool kem_process_record(uint16_t keycode, keyrecord_t *record)
{
    bool continue_process = true;

    kem_led_event_t led_event = {
        .type = KEM_LED_EVENT_KEY,
        .keycode = keycode,
        .record = record,
    };

    if (!kem_led_handle_event(&led_event))
    {
        continue_process = false;
    }

    if (!kem_led_process_record(keycode, record))
    {
        continue_process = false;
    }

    return continue_process;
}

void kem_task(void)
{
    kem_led_task();
}