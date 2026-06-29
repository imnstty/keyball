/*
 * --------------------------------------------------------------------------
 * Command Layer
 * --------------------------------------------------------------------------
 * Momentary Layer control using custom key combinations.
 *
 * Current feature:
 *   - Hold J + N to activate Layer 5
 *   - Release either key to return to the previous layer
 *
 * This module is independent from QMK Combo to minimize firmware size.
 * It is designed to be easily extended with additional command combinations.
 *
 * Version : 1.0
 * Author  : Tetsuya + ChatGPT
 * --------------------------------------------------------------------------
 */
/*
 * History
 * --------------------------------------------------------------------
 * v1.0  2026-06-29  Initial version
 */

#pragma once

#include QMK_KEYBOARD_H

bool command_process(uint16_t keycode, keyrecord_t *record);
void command_matrix_scan(void);
