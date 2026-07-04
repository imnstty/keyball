/*
Copyright 2026 Tetsuya Imanishi
*/

/******************************************************************************
 * @file      kem.c
 * @project   Keyball44 Custom Firmware
 * @brief     KEM (Keyball Extension Module) Core
 *
 * @version   2.00
 * @date      2026-07-04
 *
 *-----------------------------------------------------------------------------
 * Revision History
 *-----------------------------------------------------------------------------
 * Ver 2.00  2026-07-04
 * - Added Layer Engine processing.
 * - Initial KEM Core.
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