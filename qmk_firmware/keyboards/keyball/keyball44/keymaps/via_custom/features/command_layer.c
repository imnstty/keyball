/*
 * --------------------------------------------------------------------------
 * Command Layer
 * --------------------------------------------------------------------------
 * Implements custom chord-based momentary layer switching.
 *
 * Design concept:
 *   Right hand selects a command layer.
 *   Left hand executes shortcut keys.
 *
 * Advantages:
 *   - No COMBO_ENABLE required
 *   - Small firmware size
 *   - Independent of keyboard layout (planned)
 *   - Easy to extend
 *
 * Version : 1.0
 * --------------------------------------------------------------------------
 */
/*
 * History
 * --------------------------------------------------------------------
 * v1.0  2026-06-29  Initial version
 */

#include "command_layer.h"

// --------------------------------------------------------------------
// Physical key state
// --------------------------------------------------------------------

static bool cmd_j = false;
static bool cmd_n = false;

bool command_process(uint16_t keycode, keyrecord_t *record)
{
    uint8_t row = record->event.key.row;
    uint8_t col = record->event.key.col;

    // J position
    if (row == 5 && col == 4) {
        cmd_j = record->event.pressed;
    }

    // N position
    if (row == 6 && col == 5) {
        cmd_n = record->event.pressed;
    }

    return true;
}

void command_matrix_scan(void)
{
}
