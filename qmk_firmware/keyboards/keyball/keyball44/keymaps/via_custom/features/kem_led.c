/*
Copyright 2026 Tetsuya Imanishi
*/

/******************************************************************************
 * @file      kem_led.c
 * @project   Keyball44 Custom Firmware
 * @brief     KEM LED Engine
 *
 * @version   2.02
 * @date      2026-07-06
 *
 *-----------------------------------------------------------------------------
 * Revision History
 *-----------------------------------------------------------------------------
 * Ver 2.02  2026-07-06
 * - Refactored KEM LED state and renderer.
 * - Changed Hold indicator color from light blue to blue.
 *
 * Ver 2.01  2026-07-04
 * - Added Hold Candidate Indicator for KEM_L5.
 *
 ******************************************************************************/

#include "kem_led.h"
#include "kem.h"
#include "kem_layer.h"

#ifdef RGBLIGHT_ENABLE

#ifdef SPLIT_KEYBOARD
#include "transactions.h"
#endif

#define KEM_NO_LED 255

#define KEM_TAP_R 255
#define KEM_TAP_G 255
#define KEM_TAP_B 255

#define KEM_HOLD_R 0
#define KEM_HOLD_G 0
#define KEM_HOLD_B 255

typedef enum
{
    KEM_LED_STATE_OFF = 0,
    KEM_LED_STATE_TAP,
    KEM_LED_STATE_HOLD,
} kem_led_state_t;

typedef struct
{
    uint8_t led;
    bool is_left_side;
    bool is_active;
    kem_led_state_t state;
} kem_led_context_t;

typedef struct
{
    uint8_t led;
    uint8_t r;
    uint8_t g;
    uint8_t b;
} kem_rgb_sync_event_t;

static kem_led_context_t kem_l5_led_ctx = {
    .led = KEM_NO_LED,
    .is_left_side = true,
    .is_active = false,
    .state = KEM_LED_STATE_OFF,
};

static kem_rgb_sync_event_t kem_rgb_sync_event = {0};
static bool kem_rgb_sync_pending = false;

static const uint8_t left_key_to_led[4][6] = {
    {17, 14, 10, 6, 3, 0},
    {18, 15, 11, 7, 4, 1},
    {19, 16, 12, 8, 5, 2},
    {KEM_NO_LED, 13, 9, KEM_NO_LED, KEM_NO_LED, KEM_NO_LED},
};

static const uint8_t right_key_to_led[4][6] = {
    {10, 13, 17, 20, 23, 26},
    {11, 14, 18, 21, 24, 27},
    {12, 15, 19, 22, 25, 28},
    {KEM_NO_LED, 16, KEM_NO_LED, KEM_NO_LED, KEM_NO_LED, KEM_NO_LED},
};

static uint8_t kem_led_get_led_index(keyrecord_t *record, bool *is_left_side)
{
    uint8_t row = record->event.key.row;
    uint8_t col = record->event.key.col;

    if (col >= 6)
    {
        return KEM_NO_LED;
    }

    if (row < 4)
    {
        *is_left_side = true;
        return left_key_to_led[row][col];
    }

    *is_left_side = false;
    return right_key_to_led[row - 4][col];
}

static void kem_led_apply_rgb(uint8_t led, bool is_left_side, uint8_t r, uint8_t g, uint8_t b)
{
    if (led == KEM_NO_LED)
    {
        return;
    }

    if (is_left_side)
    {
        if (is_keyboard_left())
        {
            rgblight_setrgb_at(r, g, b, led);
        }
        else
        {
            keyball_send_led_event(led, true);
        }
    }
    else
    {
        if (!is_keyboard_left())
        {
            rgblight_setrgb_at(r, g, b, led + 30);
        }
        else
        {
            keyball_send_led_event(led, true);
        }
    }
}

static void kem_led_restore_layer_color(void)
{
    uint8_t layer = get_highest_layer(layer_state);

    switch (layer)
    {
    case 0:
        rgblight_sethsv_noeeprom(0, 0, 50);
        break;
    case 1:
        rgblight_sethsv_noeeprom(128, 255, 50);
        break;
    case 2:
        rgblight_sethsv_noeeprom(43, 255, 50);
        break;
    case 3:
        rgblight_sethsv_noeeprom(85, 255, 50);
        break;
    case 4:
        rgblight_sethsv_noeeprom(191, 255, 50);
        break;
    case 5:
        rgblight_sethsv_noeeprom(0, 255, 50);
        break;
    }
}

static void kem_led_render_context(const kem_led_context_t *ctx)
{
    if (!ctx->is_active || ctx->led == KEM_NO_LED)
    {
        return;
    }

    switch (ctx->state)
    {
    case KEM_LED_STATE_TAP:
        kem_led_apply_rgb(ctx->led, ctx->is_left_side, KEM_TAP_R, KEM_TAP_G, KEM_TAP_B);
        break;

    case KEM_LED_STATE_HOLD:
        kem_led_apply_rgb(ctx->led, ctx->is_left_side, KEM_HOLD_R, KEM_HOLD_G, KEM_HOLD_B);
        break;

    case KEM_LED_STATE_OFF:
    default:
        break;
    }
}

static void kem_led_update_l5_state(void)
{
    if (!kem_l5_led_ctx.is_active)
    {
        return;
    }

    if (kem_layer_is_l5_hold_confirmed())
    {
        kem_l5_led_ctx.state = KEM_LED_STATE_HOLD;
    }
    else
    {
        kem_l5_led_ctx.state = KEM_LED_STATE_TAP;
    }
}

bool kem_led_process_record(uint16_t keycode, keyrecord_t *record)
{
    if (keycode != KEM_L5)
    {
        return true;
    }

    if (record->event.pressed)
    {
        kem_l5_led_ctx.led = kem_led_get_led_index(record, &kem_l5_led_ctx.is_left_side);
        kem_l5_led_ctx.is_active = true;
        kem_l5_led_ctx.state = KEM_LED_STATE_TAP;

        kem_led_render_context(&kem_l5_led_ctx);
    }
    else
    {
        kem_l5_led_ctx.is_active = false;
        kem_l5_led_ctx.state = KEM_LED_STATE_OFF;

        kem_led_restore_layer_color();
    }

    return true;
}

void kem_led_task(void)
{
    kem_led_update_l5_state();
    kem_led_render_context(&kem_l5_led_ctx);
}

#else

bool kem_led_process_record(uint16_t keycode, keyrecord_t *record)
{
    return true;
}

void kem_led_task(void)
{
}

#endif