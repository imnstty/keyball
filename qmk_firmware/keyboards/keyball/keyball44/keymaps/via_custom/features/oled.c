/******************************************************************************
 * @file      oled.c
 * @project   Keyball44 Custom Firmware
 * @brief     OLED Display Control Module
 *
 * @version   1.00
 * @date      2026-06-30
 *
 * @details
 * This module manages the OLED display for the Keyball44 custom firmware.
 *
 * Features
 *   - OLED page management
 *   - Layer status display
 *   - Lock key status display
 *   - Auto Mouse Layer (ALM) status display
 *   - Keyball Easy Mouse (KEM) status display
 *   - Key information display
 *   - Ball information display
 *
 * Pages
 *   Page1 : Status
 *   Page2 : Debug
 *   Page3 : Reserved
 *
 *-----------------------------------------------------------------------------
 * Revision History
 *-----------------------------------------------------------------------------
 * Ver 1.00  2026-06-30
 * - Initial release.
 *
 ******************************************************************************/
 /*
 * -----------------------------------------------------------------------------
 * File      : oled.c
 * Project   : Keyball44 Custom Firmware
 * Author    : Tetsuya Imanishi
 * Version   : 1.00
 * Date      : 2026-06-30
 *
 * Description
 *   OLED display control module.
 *   Handles page management and OLED rendering.
 *
 * Page Configuration
 *   Page1 : Status
 *   Page2 : Debug
 *   Page3 : Reserved
 *
 * History
 * -----------------------------------------------------------------------------
 * Ver 1.00  2026-06-30
 *   - Initial version.
 * -----------------------------------------------------------------------------
 */
 
#include "oled.h"

#ifdef OLED_ENABLE

static uint8_t oled_page = 0;

void oled_next_page(void) {
    oled_page = (oled_page + 1) % 2;
}

bool oled_task_custom(void) {
    oled_clear();

    if (oled_page == 0) {
        oled_write_ln_P(PSTR("Lyr"), false);
        oled_write_ln_P(PSTR("-----"), false);
        oled_write_ln_P(PSTR(""), false);
        oled_write_ln_P(PSTR("Num"), false);
        oled_write_ln_P(PSTR("Scrl"), false);
        oled_write_ln_P(PSTR("Caps"), false);
        oled_write_ln_P(PSTR(""), false);
        oled_write_ln_P(PSTR("ALM"), false);
        oled_write_ln_P(PSTR("KEM"), false);
    } else {
        oled_write_ln_P(PSTR("CPI"), false);
        oled_write_ln_P(PSTR("0500"), false);
        oled_write_ln_P(PSTR(""), false);
        oled_write_ln_P(PSTR("Ball"), false);
        oled_write_ln_P(PSTR("X +0"), false);
        oled_write_ln_P(PSTR("Y +0"), false);
        oled_write_ln_P(PSTR("H +0"), false);
        oled_write_ln_P(PSTR("V +0"), false);
    }

    return false;
}

#endif