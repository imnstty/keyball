/*
Copyright 2026 Tetsuya Imanishi
*/

/******************************************************************************
 * @file      kem_debug.h
 * @project   Keyball44 Custom Firmware
 * @brief     KEM Debug Engine
 *
 * @version   2.10
 * @date      2026-07-07
 *
 *-----------------------------------------------------------------------------
 * Revision History
 *-----------------------------------------------------------------------------
 * Ver 2.11  2026-07-07
 * - Added kem_debug_handle_event() declaration.
 *
 ******************************************************************************/

#pragma once

#include QMK_KEYBOARD_H

typedef enum
{
    KEM_DEBUG_EVENT_KEY = 0,
} kem_debug_event_type_t;

typedef struct
{
    kem_debug_event_type_t type;
    uint16_t keycode;
    bool pressed;
    uint8_t row;
    uint8_t col;
    uint16_t time;
    uint8_t led;
} kem_debug_event_t;

void kem_debug_handle_event(const kem_debug_event_t *event);
void kem_debug_send(const kem_debug_event_t *event);
void kem_debug_record_key_event(uint16_t keycode, keyrecord_t *record);
void kem_debug_record_key_event_with_led(uint16_t keycode, keyrecord_t *record, uint8_t led);

/* Matrix Debug */
void kem_debug_record_matrix_event(uint8_t row,
                                   uint8_t col,
                                   bool pressed);
#ifdef KEM_DEBUG_MATRIX_ENABLE
void kem_debug_record_matrix_event_with_led(uint8_t row,
                                            uint8_t col,
                                            bool pressed,
                                            uint8_t led);
#endif

void kem_debug_enable(bool enable);
bool kem_debug_is_enabled(void);
void kem_debug_toggle(void);
