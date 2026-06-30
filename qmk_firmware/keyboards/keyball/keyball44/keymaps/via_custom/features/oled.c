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

#include "oled.h"

#ifdef OLED_ENABLE

/******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t oled_page = 0;

/******************************************************************************
 * Local Functions
 ******************************************************************************/
static void render_page1(void)
{
    oled_write_ln_P(PSTR("Page1"), false);
    oled_write_ln_P(PSTR("Lyr"), false);
    oled_write_ln_P(PSTR("-----"), false);
}

static void render_page2(void)
{
    oled_write_ln_P(PSTR("Page2"), false);
    oled_write_ln_P(PSTR("CPI"), false);
    oled_write_ln_P(PSTR("0500"), false);
}

/******************************************************************************
 * Public Functions
 ******************************************************************************/
void oled_next_page(void)
{
    oled_page = (oled_page + 1) % 2;
}

bool oled_task_custom(void)
{
    oled_clear();

    if (oled_page == 0) {
        render_page1();
    } else {
        render_page2();
    }

    return false;
}

#endif