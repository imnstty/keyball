/*
Copyright 2026 Tetsuya Imanishi
*/

/******************************************************************************
 * @file      kem_layer.c
 * @project   Keyball44 Custom Firmware
 * @brief     KEM Layer Engine
 *
 * @version   2.00
 * @date      2026-07-04
 *
 *-----------------------------------------------------------------------------
 * Revision History
 *-----------------------------------------------------------------------------
 * Ver 2.00  2026-07-04
 * - Added Tap/Hold Layer5 key.
 *
 ******************************************************************************/

#include "kem_layer.h"
#include "kem.h"

#define KEM_LAYER5 5

static bool     kem_l5_active      = false;
static bool     kem_l5_interrupted = false;
static uint16_t kem_l5_timer       = 0;

bool kem_layer_process_record(uint16_t keycode, keyrecord_t *record) {
    if (kem_l5_active && keycode != KEM_L5 && record->event.pressed) {
        kem_l5_interrupted = true;
    }

    switch (keycode) {
        case KEM_L5:
            if (record->event.pressed) {
                kem_l5_active      = true;
                kem_l5_interrupted = false;
                kem_l5_timer       = timer_read();

                layer_on(KEM_LAYER5);
            } else {
                bool is_tap = timer_elapsed(kem_l5_timer) < TAPPING_TERM && !kem_l5_interrupted;

                kem_l5_active = false;

                if (is_tap) {
                    layer_move(KEM_LAYER5);
                } else {
                    layer_off(KEM_LAYER5);
                }
            }
            return false;
    }

    return true;
}

bool kem_layer_is_l5_active(void) {
    return kem_l5_active;
}

bool kem_layer_is_l5_hold_candidate(void) {
    return kem_l5_active && !kem_l5_interrupted;
}

bool kem_layer_is_l5_hold_confirmed(void) {
    if (!kem_l5_active) {
        return false;
    }

    return kem_l5_interrupted || timer_elapsed(kem_l5_timer) >= TAPPING_TERM;
}