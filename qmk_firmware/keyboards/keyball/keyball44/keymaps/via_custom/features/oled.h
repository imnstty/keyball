/******************************************************************************
 * @file      oled.h
 * @project   Keyball44 Custom Firmware
 * @brief     OLED Display Control Header
 *
 * @version   2.00
 * @date      2026-07-04
 *
 * @details
 * Header file for the OLED display control module.
 *
 *-----------------------------------------------------------------------------
 * Revision History
 *-----------------------------------------------------------------------------
 * Ver 2.00  2026-07-04
 * - Updated header version for KEM Ver.2.0.
 *
 * Ver 1.00  2026-06-30
 * - Initial release.
 *
 ******************************************************************************/

#pragma once

#include QMK_KEYBOARD_H

#ifdef OLED_ENABLE
bool oled_task_custom(void);
void oled_next_page(void);
#endif

void oled_record_key(uint16_t keycode, keyrecord_t *record);
void oled_record_key_with_led(uint16_t keycode, keyrecord_t *record, uint8_t led);