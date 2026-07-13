/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://gnu.org>.
*/

/******************************************************************************
 * @file      keymap.c
 * @project   Keyball44 Custom Firmware
 * @brief     Keymap and custom key processing
 *
 * @version   2.20
 * @date      2026-07-08
 *
 *-----------------------------------------------------------------------------
 * Revision History
 *-----------------------------------------------------------------------------
 * Ver 2.23  2026-07-08
 * - Added KEM_DEBUG_MATRIX_ENABLE guard for temporary Matrix Debug output.
 * - Kept matrix-based LED event path always enabled.
 *
 * Ver 2.22  2026-07-08
 * - Removed duplicated direct key LED output from process_record_user().
 * - Kept LED number lookup for Debug Framework.
 * - LED output is now handled by KEM LED matrix event path.
 *
 * Ver 2.10  2026-07-07
 * - Routed key events to KEM Debug Engine.
 * - No functional changes.
 *
 * Ver 2.02  2026-07-06
 * - Updated KEM LED initialization for state synchronization.
 *
 * Ver 2.01  2026-07-06
 * - Added KEM LED initialization during keyboard startup.
 *
 * Ver 2.00  2026-07-04
 * - Updated header version for KEM Ver.2.0.
 *
 * Ver 1.15  2026-07-01
 * - Added KEM toggle and OLED status display.
 *
 ******************************************************************************/

#include QMK_KEYBOARD_H

#include "quantum.h"

// Command Layer
#include "features/command_layer.h"

// KEM Core
#include "features/kem.h"
#include "features/kem_led.h"
#include "features/kem_debug.h"

// OLED Custom
#include "features/oled.h"

#ifdef OLED_ENABLE
bool oled_task_user(void)
{
    return oled_task_custom();
}
#endif

// void keyball_send_led_event(uint8_t led, bool pressed);

// Keyball LED Event Synchronization
// enum custom_keycodes {
// KEM_TOG = KEYBALL_SAFE_RANGE,
//};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    // =========================================================
    // Layer 0 : Base
    // =========================================================
    [0] = LAYOUT_universal(
        KC_ESC        , KC_Q     , KC_W   , KC_E     , KC_R     , KC_T     ,                                    KC_Y,    KC_U,    KC_I   , KC_O     , KC_P         , KC_DEL      ,
        LSFT_T(KC_TAB), KC_A     , KC_S   , KC_D     , KC_F     , KC_G     ,                                    KC_H,    KC_J,    KC_K   ,LT(3,KC_L),LT(1,KC_MINS) ,LT(3,KC_INT3),
        KC_LCTL       , KC_Z     , KC_X   , KC_C     , KC_V     , KC_B     ,                                    KC_N,    KC_M,    KC_COMM, KC_DOT   , KC_SLSH      , KC_INT1     ,
                             KC_LALT, KC_LGUI ,LT(3,KC_LNG2),LT(2,KC_SPC),LT(1,KC_ENT),          LT(1,KC_INT4),LT(2,KC_SPC), _______ , _______ , KEM_L5
    ),

    // =========================================================
    // Layer 1 : Mouse / Navigation / Function
    // =========================================================
    [1] = LAYOUT_universal(
        KC_ESC        , KC_F1    , KC_F2  , KC_F3    , KC_F4    , KC_BSPC  ,                                    KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9, KC_F10,
        LSFT_T(KC_TAB), KC_MENU  , AML_TO , KC_NO    ,S(KC_UP)  , KC_EQL   ,                                    KC_MINS, KC_UP, KC_BTN1,   KC_BTN2, KC_NO, KC_NO ,
        KC_LCTL       , KC_SELECT, KC_NO  ,S(KC_LEFT),S(KC_DOWN),S(KC_RGHT),                                    KC_LEFT, KC_DOWN, KC_RGHT, KC_BTN3, KC_NO, MO(3) ,
                              KC_LALT , KC_LGUI ,LT(3,KC_LNG2),LT(2,KC_SPC), _______ ,                _______ ,LT(2,KC_SPC), _______ , _______ , KEM_L5
    ),

    // =========================================================
    // Layer 2 : Numbers / Symbols
    // =========================================================
    [2] = LAYOUT_universal(
        KC_ESC        , KC_NO, KC_7, KC_8, KC_9, KC_BSPC ,                                                     S(KC_SCLN),S(KC_QUOT)     , KC_LBRC  , KC_EQL   , KC_INT3 , KC_DEL  ,
        LSFT_T(KC_TAB), KC_NO, KC_4, KC_5, KC_6, KC_EQL  ,                                                      KC_MINS  ,LSFT_T(KC_SLSH), KC_SCLN  ,S(KC_QUOT), KC_LBRC , KC_RBRC ,
        KC_LCTL       , KC_NO, KC_1, KC_2, KC_3,S(KC_8)  ,                                                     S(KC_9)   ,S(KC_INT1)     ,S(KC_COMM),S(KC_DOT) , KC_NO   , KC_NUHS ,
                       KC_0 , KC_DOT , KC_LSFT , _______ , KC_ENT ,                                    KC_ENT , _______ , _______ , _______ , KEM_L5
    ),

    // =========================================================
    // Layer 3 : Settings
    // =========================================================
    [3] = LAYOUT_universal(
        KC_ESC        , KC_NUM  , KC_SCRL , KC_PSCR , KC_PAUS     , KC_BSPC ,                                  S(KC_SCLN),S(KC_QUOT) , KC_7 , KC_8 , KC_9 , KC_DEL ,
        LSFT_T(KC_TAB), KC_CAPS , KC_HOME , KC_PGUP ,LCTL(KC_PGUP), KC_EQL  ,                                   KC_MINS  , KC_SLSH   , KC_4 , KC_5 , KC_6 , KC_NO  ,
        KC_LCTL       , KC_NO   , KC_END  , KC_PGDN ,LCTL(KC_PGDN),S(KC_8)  ,                                  S(KC_9)  , KC_0      , KC_1 , KC_2 , KC_3 , KC_NO  ,
                                  KC_LALT , KC_LGUI , _______ , LT(2,KC_SPC), _______ ,           LT(1,KC_ENT),LT(2,KC_SPC), _______ , _______ , KC_PDOT
    ),

    [5] = LAYOUT_universal(
        KC_ESC        ,LCTL(KC_1), KC_F2    , KC_E     , KC_DEL   , KC_BSPC  ,                                   KC_NO   , KC_NO   , KC_NO   , KC_NO   ,LCTL(KC_P), KC_DEL ,
        LSFT_T(KC_TAB),LCTL(KC_A),QK_MACRO_2,QK_MACRO_1,QK_MACRO_0, KC_COMM  ,                                   KC_NO   , KC_UP   , KC_BTN1 , KC_BTN2 , KC_BTN3  , AML_TO ,
        KC_LCTL       ,LCTL(KC_Z),LCTL(KC_X),LCTL(KC_C),LCTL(KC_V),LCTL(KC_Y),                                   KC_LEFT , KC_DOWN , KC_RGHT , KC_NO   , KC_NO    , KC_NO  ,
                               KC_LALT , KC_LGUI ,LT(3,KC_LEFT),LT(2,KC_RGHT),LT(1,KC_ENT),        LT(1,KC_INT4),LT(2,KC_SPC), _______ , _______ , TO(0)
    ),
};
// clang-format on

#define LAYER_RGB_VAL 50

layer_state_t layer_state_set_user(layer_state_t state)
{
    uint8_t layer = get_highest_layer(state);

    // Auto enable scroll mode when the highest layer is 3
    keyball_set_scroll_mode(layer == 3);

#ifdef RGBLIGHT_ENABLE
    switch (layer)
    {
    case 0:
        rgblight_sethsv_noeeprom(0, 0, LAYER_RGB_VAL); // White
        break;

    case 1:
        rgblight_sethsv_noeeprom(128, 255, LAYER_RGB_VAL); // Cyan
        break;

    case 2:
        rgblight_sethsv_noeeprom(43, 255, LAYER_RGB_VAL); // Yellow
        break;

    case 3:
        rgblight_sethsv_noeeprom(85, 255, LAYER_RGB_VAL); // Green
        break;

    case 4:
        rgblight_sethsv_noeeprom(191, 255, LAYER_RGB_VAL); // Purple
        break;

    case 5:
        rgblight_sethsv_noeeprom(0, 255, LAYER_RGB_VAL); // Red
        break;
    }
#endif

    return state;
}

#ifdef OLED_ENABLE

#include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void)
{
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
    // keyball_oled_render_keminfo();
}
#endif

#define NO_LED 255

static const uint8_t left_key_to_led[4][6] = {
    {17, 14, 10, 6, 3, 0}, // ESC Q W E R T
    {18, 15, 11, 7, 4, 1}, // TAB A S D F G
    {19, 16, 12, 8, 5, 2}, // SHIFT Z X C V B
    {NO_LED, 13, 9, NO_LED, NO_LED, NO_LED},
    //    {NO_LED, 13, 9, 27, 28, 29},
};

static const uint8_t right_key_to_led[4][6] = {
    {10, 13, 17, 20, 23, 26}, // Y U I O P DEL
    {11, 14, 18, 21, 24, 27}, // H J K L ; '
    {12, 15, 19, 22, 25, 28}, // N M , . / \,
    {NO_LED, 16, NO_LED, NO_LED, NO_LED, NO_LED},
    //    {NO_LED, 16, NO_LED, NO_LED, 2, 1},
};

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{

#ifdef OLED_ENABLE
    oled_record_key(keycode, record);
#endif

    switch (keycode)
    {
    case KC_MENU:
        if (record->event.pressed)
        {
            if (get_mods() & MOD_MASK_SHIFT)
            {
                kem_debug_toggle();
            }
            else
            {
#ifdef OLED_ENABLE
                oled_next_page();
#endif
            }
        }
        return false;

    case KC_SELECT:
        if (record->event.pressed)
        {
            keyball_toggle_kem();
        }
        return false;
    }

    if (!command_process(keycode, record))
    {
        return false;
    }

    bool kem_continue = kem_process_record(keycode, record);

    if (!keyball_get_kem_enabled())
    {
        return kem_continue;
    }

    uint8_t row = record->event.key.row;
    uint8_t col = record->event.key.col;
    uint8_t debug_led = NO_LED;

    if (col >= 6)
    {
        return true;
    }

    if (row < 4)
    {
        debug_led = left_key_to_led[row][col];
    }
    else
    {
        debug_led = right_key_to_led[row - 4][col];
    }

#ifdef OLED_ENABLE
    oled_record_key_with_led(keycode, record, debug_led);
#endif

    kem_debug_record_key_event_with_led(keycode, record, debug_led);
    return kem_continue;
}

void keyboard_post_init_user(void)
{
    kem_led_init();
}

void matrix_scan_user(void)
{
    static matrix_row_t prev_matrix[MATRIX_ROWS] = {0};

    for (uint8_t row = 0; row < MATRIX_ROWS; row++)
    {
        matrix_row_t current = matrix_get_row(row);
        matrix_row_t changed = current ^ prev_matrix[row];

        if (changed)
        {
            for (uint8_t col = 0; col < MATRIX_COLS; col++)
            {
                if (changed & ((matrix_row_t)1 << col))
                {
                    bool pressed = current & ((matrix_row_t)1 << col);

                    // ここでDebug出力
                    // keycodeは未確定なので仮で0xFFFFを送る
                    uint8_t led = NO_LED;

                    if (col < 6)
                    {
                        if (row < 4)
                        {
                            led = left_key_to_led[row][col];
                        }
                        else
                        {
                            led = right_key_to_led[row - 4][col];
                        }
                    }

#ifdef KEM_DEBUG_MATRIX_ENABLE
                    kem_debug_record_matrix_event_with_led(row, col, pressed, led);
#endif

                    kem_led_process_matrix_event(row, col, pressed, led);
                }
            }

            prev_matrix[row] = current;
        }
    }

    kem_task();
}