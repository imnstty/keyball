/*
Copyright 2026 Tetsuya Imanishi
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
 * Revision History
 *-----------------------------------------------------------------------------
 * Ver 2.00  2026-07-04
 * - Added KEM_L5 custom keycode.
 * - Added KEM Core interface.
 *
 ******************************************************************************/

#pragma once

#include QMK_KEYBOARD_H

enum kem_keycodes {
    KEM_TOG = KEYBALL_SAFE_RANGE,
    KEM_L5,
};

bool kem_process_record(uint16_t keycode, keyrecord_t *record);