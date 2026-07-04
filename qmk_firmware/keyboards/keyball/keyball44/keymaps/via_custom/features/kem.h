/*
Copyright 2026 Tetsuya Imanishi

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.
*/

/******************************************************************************
 * @file      kem.h
 * @project   Keyball44 Custom Firmware
 * @brief     KEM (Keyball Extension Module) Core Interface
 *
 * @version   2.00
 * @date      2026-07-04
 *
 *-----------------------------------------------------------------------------
 * Description
 *-----------------------------------------------------------------------------
 * Central interface for all KEM modules.
 *
 * Planned modules:
 *   - Layer Engine
 *   - LED Engine
 *   - OLED Engine
 *   - Command Engine
 *
 ******************************************************************************/

#include "kem.h"
#include "kem_layer.h"

bool kem_process_record(uint16_t keycode, keyrecord_t *record) {
    if (!kem_layer_process_record(keycode, record)) {
        return false;
    }

    return true;
}