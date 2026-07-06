/*
Copyright 2026 Tetsuya Imanishi
*/

/******************************************************************************
 * @file      kem_led.h
 * @project   Keyball44 Custom Firmware
 * @brief     KEM LED Engine Interface
 *
 * @version   2.03
 * @date      2026-07-06
 *
 *-----------------------------------------------------------------------------
 * Revision History
 *-----------------------------------------------------------------------------
 * Ver 2.04  2026-07-06
 * - Replaced RGB synchronization with KEM LED State synchronization.
 * - Completed split Hold Candidate Indicator.
 * - Added KEM LED state transport framework.
 *
 *  * Ver 2.03  2026-07-06
 * - Added KEM LED initialization interface.
 *
 * Ver 2.01  2026-07-04
 * - Added Hold Candidate Indicator interface.
 *
 ******************************************************************************/

#pragma once

#include QMK_KEYBOARD_H

bool kem_led_process_record(uint16_t keycode, keyrecord_t *record);
void kem_led_task(void);
void kem_led_init(void);