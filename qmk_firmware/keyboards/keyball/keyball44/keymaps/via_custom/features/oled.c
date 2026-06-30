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
 * Ver 1.10  2026-06-30
 * - Added real ball movement display on Page2.
 *
 * Ver 1.09  2026-06-30
 * - Clear OLED only when page changes.
 *
 * Ver 1.08  2026-06-30
 * - Added Page2 debug layout skeleton.
 *
 * Ver 1.07  2026-06-30
 * - Added ALM status display.
 *
 * Ver 1.06  2026-06-30
 * - Added custom key position and keycode display.
 *
 * Ver 1.05  2026-06-30
 * - Added built-in key information display.
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
static uint8_t last_row = 0;
static uint8_t last_col = 0;
static uint16_t last_kc = 0;
static bool oled_page_changed = true;

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
static void render_page1(void);
static void render_page2(void);
static void render_layer(void);
static void render_lock_status(void);
static void render_keyball_status(void);
static void render_key_info(void);
static void render_ball_info(void);
static void render_cpi_info(void);
static void oled_write_signed_2digit(char label, int8_t value);

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
 * @brief Render Keyball feature status
 ******************************************************************************/
static void render_keyball_status(void)
{
#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
    oled_write_ln_P(PSTR("ALM"), get_auto_mouse_enable());
#else
    oled_write_ln_P(PSTR("ALM"), false);
#endif

    oled_write_ln_P(PSTR("KEM"), false);
}

/******************************************************************************
 * @brief Render last key position and keycode
 ******************************************************************************/
static void render_key_info(void)
{
    static const char hex[] = "0123456789ABCDEF";

    char pos[5];
    pos[0] = 'R';
    pos[1] = '0' + last_row;
    pos[2] = 'C';
    pos[3] = '0' + last_col;
    pos[4] = '\0';

    oled_write_ln(pos, false);

    char kc[4];
    kc[0] = 'K';
    kc[1] = hex[(last_kc >> 4) & 0x0F];
    kc[2] = hex[last_kc & 0x0F];
    kc[3] = '\0';

    oled_write_ln(kc, false);
}

/******************************************************************************
 * @brief Write signed 2-digit value
 ******************************************************************************/
static void oled_write_signed_2digit(char label, int8_t value)
{
    char buf[5];
    uint8_t abs_value;

    buf[0] = label;

    if (value < 0) {
        buf[1] = '-';
        abs_value = (uint8_t)(-value);
    } else {
        buf[1] = '+';
        abs_value = (uint8_t)value;
    }

    if (abs_value > 99) {
        abs_value = 99;
    }

    buf[2] = '0' + (abs_value / 10);
    buf[3] = '0' + (abs_value % 10);
    buf[4] = '\0';

    oled_write_ln(buf, false);
}

/******************************************************************************
 * @brief Render CPI information
 ******************************************************************************/
static void render_cpi_info(void)
{
    oled_write_ln_P(PSTR("CPI"), false);
    oled_write_ln_P(PSTR("0500"), false);
}

/******************************************************************************
 * @brief Render ball movement information
 ******************************************************************************/
static void render_ball_info(void)
{
    oled_write_ln_P(PSTR("Ball"), false);
    oled_write_signed_2digit('X', keyball.last_mouse.x);
    oled_write_signed_2digit('Y', keyball.last_mouse.y);
    oled_write_signed_2digit('H', keyball.last_mouse.h);
    oled_write_signed_2digit('V', keyball.last_mouse.v);
}

/******************************************************************************
 * @brief Render Page 1 (Status Page)
 ******************************************************************************/
static void render_page1(void)
{
    render_layer();
    render_lock_status();

    oled_write_ln_P(PSTR(""), false);
    render_keyball_status();

    oled_write_ln_P(PSTR(""), false);
    render_key_info();
}

/******************************************************************************
 * @brief Render Page 2 (Debug Page)
 ******************************************************************************/
static void render_page2(void)
{
    render_cpi_info();

    oled_write_ln_P(PSTR(""), false);
    render_ball_info();

    oled_write_ln_P(PSTR(""), false);
    render_key_info();
}

/******************************************************************************
 * Public Functions
 ******************************************************************************/
void oled_next_page(void)
{
    oled_page = (oled_page + 1) % 2;
    oled_page_changed = true;
}

/******************************************************************************
 * @brief Record last pressed key information
 ******************************************************************************/
void oled_record_key(uint16_t keycode, keyrecord_t *record)
{
    if (record->event.pressed) {
        last_row = record->event.key.row;
        last_col = record->event.key.col;
        last_kc = keycode & 0xFF;
    }
}

bool oled_task_custom(void)
{
    if (oled_page_changed) {
        oled_clear();
        oled_page_changed = false;
    }

    oled_set_cursor(0, 0);

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
