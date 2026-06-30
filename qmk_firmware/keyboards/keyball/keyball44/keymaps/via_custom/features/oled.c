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
 * Ver 1.05  2026-06-30
 * - Added key position and keycode display.
 *
 * Ver 1.04  2026-06-30
 * - Changed layer label to "Layer".
 * - Added lock key status display.
 *
 * Ver 1.03  2026-06-30
 * - Changed OLED orientation to vertical.
 *
 * Ver 1.02  2026-06-30
 * - Added Page1 layout skeleton.
 *
 * Ver 1.01  2026-06-30
 * - Added active layer status display.
 *
 * Ver 1.00  2026-06-30
 * - Initial release.
 *
 ******************************************************************************/

#include "oled.h"
#include "lib/keyball/keyball.h"

#ifdef OLED_ENABLE

/******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t oled_page = 0;

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
static void render_page1(void);
static void render_page2(void);
static void render_layer(void);
static void render_lock_status(void);
static void render_key_info(void);

/******************************************************************************
 * Local Functions
 ******************************************************************************/

/******************************************************************************
 * @brief Render active layer status
 ******************************************************************************/
static void render_layer(void)
{
    oled_write_ln_P(PSTR("Layer"), false);

    char buf[6];

    for (uint8_t i = 1; i <= 5; i++) {
        buf[i - 1] = layer_state_is(i) ? ('0' + i) : '-';
    }

    buf[5] = '\0';

    oled_write_ln(buf, false);
}

/******************************************************************************
 * @brief Render lock key status
 ******************************************************************************/
static void render_lock_status(void)
{
    led_t led_state = host_keyboard_led_state();

    oled_write_ln_P(PSTR(""), false);
    oled_write_ln_P(PSTR("Num"), led_state.num_lock);
    oled_write_ln_P(PSTR("Scrl"), led_state.scroll_lock);
    oled_write_ln_P(PSTR("Caps"), led_state.caps_lock);
}

/******************************************************************************
 * @brief Render last key position and keycode
 ******************************************************************************/
static void render_key_info(void)
{
    char buf[6];

    snprintf(buf, sizeof(buf), "R%dC%d", keyball.last_pos.row, keyball.last_pos.col);
    oled_write_ln(buf, false);

    snprintf(buf, sizeof(buf), "K%02X", keyball.last_kc & 0xFF);
    oled_write_ln(buf, false);
}

/******************************************************************************
 * @brief Render Page 1 (Status Page)
 ******************************************************************************/
static void render_page1(void)
{
    render_layer();
    render_lock_status();

    oled_write_ln_P(PSTR(""), false);
    oled_write_ln_P(PSTR("ALM"), false);
    oled_write_ln_P(PSTR("KEM"), false);

    oled_write_ln_P(PSTR(""), false);
    
    render_key_info();
}

/******************************************************************************
 * @brief Render Page 2 (Debug Page)
 ******************************************************************************/
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

/******************************************************************************
 * @brief Initialize OLED rotation
 ******************************************************************************/
oled_rotation_t oled_init_user(oled_rotation_t rotation)
{
    return OLED_ROTATION_270;
}

#endif
