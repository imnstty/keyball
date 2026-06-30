/******************************************************************************
 * @file      custom_keycodes.h
 * @project   Keyball44 Custom Firmware
 * @brief     Custom Keycode Definitions
 *
 * @version   1.00
 * @date      2026-06-30
 *
 * @details
 * Defines custom keycodes used by the Keyball44 custom firmware.
 *
 * Current custom keycodes:
 *   - OLED_NEXT : Switch OLED display page
 *
 * Future planned keycodes:
 *   - KEM_TOGGLE
 *   - LED_MODE_NEXT
 *   - DEBUG_TOGGLE
 *
 *-----------------------------------------------------------------------------
 * Revision History
 *-----------------------------------------------------------------------------
 * Ver 1.00  2026-06-30
 * - Initial release.
 *
 ******************************************************************************/

#pragma once

#include QMK_KEYBOARD_H

/******************************************************************************
 * Custom Keycodes
 ******************************************************************************/

enum custom_keycodes {
    OLED_NEXT = SAFE_RANGE,
};
