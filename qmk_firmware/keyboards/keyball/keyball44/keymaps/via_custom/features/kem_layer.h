/*
Copyright 2026 Tetsuya Imanishi
*/

/******************************************************************************
 * @file      kem_layer.h
 * @project   Keyball44 Custom Firmware
 * @brief     KEM Layer Engine Interface
 *
 * @version   2.00
 * @date      2026-07-04
 *
 *-----------------------------------------------------------------------------
 * Revision History
 *-----------------------------------------------------------------------------
 * Ver 2.00  2026-07-04
 * - Initial Layer Engine interface.
 *
 ******************************************************************************/

#pragma once

#include QMK_KEYBOARD_H

bool kem_layer_process_record(uint16_t keycode, keyrecord_t *record);