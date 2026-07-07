/*
Copyright 2026 Tetsuya Imanishi
*/

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
} kem_debug_event_t;

void kem_debug_send(const kem_debug_event_t *event);
void kem_debug_record_key_event(uint16_t keycode, keyrecord_t *record);