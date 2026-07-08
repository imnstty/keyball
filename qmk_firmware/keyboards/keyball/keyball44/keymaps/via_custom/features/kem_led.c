/*
Copyright 2026 Tetsuya Imanishi
*/

/******************************************************************************
 * @file      kem_led.c
 * @project   Keyball44 Custom Firmware
 * @brief     KEM LED Engine
 *
 * @version   2.20
 * @date      2026-07-08
 *
 *-----------------------------------------------------------------------------
 * Revision History
 *-----------------------------------------------------------------------------
 * - Introduced Universal LED Engine event API.
 * - Added kem_led_handle_event() as the unified event entry point.
 * - No functional changes.
 * * Ver 2.04  2026-07-06
 * - Replaced RGB synchronization with KEM LED state synchronization.
 * - Added split state synchronization for KEM_L5 Hold indicator.
 *
 * Ver 2.21  2026-07-08
 * - Added Tap/Hold LED tracking context.
 * - Added promotion from matrix candidate to Tap/Hold tracking.
 * - No LED behavior changes.
 *
 * Ver 2.17  2026-07-08
 * - Added Tap/Hold keycode classifier.
 * - No LED behavior changes.
 *
 * Ver 2.16  2026-07-08
 * - Added Matrix LED Event API.
 * - Added instant white LED output on matrix press.
 * - Added layer color restore on matrix release.
 *
 * Ver 2.15  2026-07-07
 * - Centralized LED hardware access through kem_led_hw_set_rgb().
 * - No functional changes.
 *
 * Ver 2.14  2026-07-07
 * - Separated LED output responsibility from render context.
 * - Kept state-based output without RGB transport.
 * - No functional changes.
 *
 * Ver 2.13  2026-07-07
 * - Separated KEM_L5 LED state update from event handling.
 * - No functional changes.
 *
 * Ver 2.12  2026-07-07
 * - Added Key LED Event API wrapper.
 * - No functional changes.
 *
 * Ver 2.11  2026-07-07
 * - Separated KEM_L5 LED state update from key event handling.
 * - No functional changes.
 *
 * Ver 2.03  2026-07-06
 * - Prepared KEM Split RGB synchronization infrastructure.
 * - Added RPC initialization framework.
 *
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

#ifdef SPLIT_KEYBOARD
#include "transactions.h"
#endif

#ifdef RGBLIGHT_ENABLE

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
    uint8_t state;
} kem_led_sync_event_t;

static kem_led_context_t kem_l5_led_ctx = {
    .led = KEM_NO_LED,
    .is_left_side = true,
    .is_active = false,
    .state = KEM_LED_STATE_OFF,
};

typedef struct
{
    bool active;
    uint8_t row;
    uint8_t col;
    uint8_t led;
    bool is_left_side;
    uint16_t press_time;
    bool hold_confirmed;
} kem_tap_hold_context_t;

static kem_tap_hold_context_t kem_tap_hold_ctx = {
    .active = false,
    .row = 0,
    .col = 0,
    .led = KEM_NO_LED,
    .is_left_side = true,
    .press_time = 0,
    .hold_confirmed = false,
};

#ifdef SPLIT_KEYBOARD
static kem_led_sync_event_t kem_led_sync_event = {0};
static bool kem_led_sync_pending = false;
#endif

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

static void kem_led_hw_set_rgb(uint8_t led, uint8_t r, uint8_t g, uint8_t b)
{
    if (led == KEM_NO_LED)
    {
        return;
    }

    rgblight_setrgb_at(r, g, b, led);
}

static void kem_led_apply_local_rgb(uint8_t led, bool is_left_side, uint8_t r, uint8_t g, uint8_t b)
{
    if (led == KEM_NO_LED)
    {
        return;
    }

    if (is_left_side)
    {
        kem_led_hw_set_rgb(led, r, g, b);
    }
    else
    {
        kem_led_hw_set_rgb(led + 30, r, g, b);
    }
}

static void kem_led_apply_state(uint8_t led, bool is_left_side, kem_led_state_t state)
{
    switch (state)
    {
    case KEM_LED_STATE_TAP:
        kem_led_apply_local_rgb(led, is_left_side, KEM_TAP_R, KEM_TAP_G, KEM_TAP_B);
        break;

    case KEM_LED_STATE_HOLD:
        kem_led_apply_local_rgb(led, is_left_side, KEM_HOLD_R, KEM_HOLD_G, KEM_HOLD_B);
        break;

    case KEM_LED_STATE_OFF:
    default:
        break;
    }
}

static void kem_led_apply_remote_state(uint8_t led, kem_led_state_t state)
{
    if (led == KEM_NO_LED)
    {
        return;
    }

    switch (state)
    {
    case KEM_LED_STATE_TAP:
        kem_led_hw_set_rgb(led, KEM_TAP_R, KEM_TAP_G, KEM_TAP_B);
        break;

    case KEM_LED_STATE_HOLD:
        kem_led_hw_set_rgb(led, KEM_HOLD_R, KEM_HOLD_G, KEM_HOLD_B);
        break;

    case KEM_LED_STATE_OFF:
    default:
        break;
    }
}

#ifdef SPLIT_KEYBOARD
static void kem_led_sync_send(uint8_t led, kem_led_state_t state)
{
    if (!is_keyboard_master() || led == KEM_NO_LED)
    {
        return;
    }

    kem_led_sync_event.led = led;
    kem_led_sync_event.state = (uint8_t)state;
    kem_led_sync_pending = true;
}

static void kem_led_sync_flush(void)
{
    if (!kem_led_sync_pending || !is_keyboard_master())
    {
        return;
    }

    if (transaction_rpc_send(USER_KEM_LED_SYNC, sizeof(kem_led_sync_event), &kem_led_sync_event))
    {
        kem_led_sync_pending = false;
    }
}

static void kem_led_sync_handler(uint8_t in_buflen, const void *in_data,
                                 uint8_t out_buflen, void *out_data)
{
    const kem_led_sync_event_t *ev = (const kem_led_sync_event_t *)in_data;

    if (ev->led >= RGBLED_NUM)
    {
        return;
    }

    kem_led_apply_remote_state(ev->led, (kem_led_state_t)ev->state);
}
#endif

static bool kem_led_is_local_side(bool is_left_side)
{
    return (is_left_side && is_keyboard_left()) || (!is_left_side && !is_keyboard_left());
}

static void kem_led_output_state(uint8_t led, bool is_left_side, kem_led_state_t state)
{
    if (kem_led_is_local_side(is_left_side))
    {
        kem_led_apply_state(led, is_left_side, state);
    }
    else
    {
#ifdef SPLIT_KEYBOARD
        kem_led_sync_send(led, state);
#endif
    }
}

static void kem_led_render_context(const kem_led_context_t *ctx)
{
    if (!ctx->is_active || ctx->led == KEM_NO_LED)
    {
        return;
    }

    kem_led_output_state(ctx->led, ctx->is_left_side, ctx->state);
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

static bool __attribute__((unused)) kem_led_is_tap_hold_keycode(uint16_t keycode)
{
    uint16_t type = keycode & 0xF000;

    return type == 0x2000 || type == 0x4000;
}

static void kem_led_track_matrix_candidate(uint8_t row,
                                           uint8_t col,
                                           bool pressed,
                                           uint8_t led)
{
    if (!pressed || led == KEM_NO_LED)
    {
        return;
    }

    kem_tap_hold_ctx.active = false;
    kem_tap_hold_ctx.row = row;
    kem_tap_hold_ctx.col = col;
    kem_tap_hold_ctx.led = led;
    kem_tap_hold_ctx.is_left_side = row < 4;
    kem_tap_hold_ctx.press_time = timer_read();
    kem_tap_hold_ctx.hold_confirmed = false;
}

static void kem_led_promote_tap_hold_tracking(uint16_t keycode,
                                              keyrecord_t *record)
{
    if (!record->event.pressed)
    {
        return;
    }

    if (!kem_led_is_tap_hold_keycode(keycode))
    {
        return;
    }

    if (kem_tap_hold_ctx.row != record->event.key.row ||
        kem_tap_hold_ctx.col != record->event.key.col)
    {
        return;
    }

    kem_tap_hold_ctx.active = true;
    kem_tap_hold_ctx.hold_confirmed = false;
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

static void kem_led_update_l5_key_state(keyrecord_t *record)
{
    if (record->event.pressed)
    {
        kem_l5_led_ctx.led = kem_led_get_led_index(record, &kem_l5_led_ctx.is_left_side);
        kem_l5_led_ctx.is_active = true;
        kem_l5_led_ctx.state = KEM_LED_STATE_TAP;
    }
    else
    {
        kem_l5_led_ctx.is_active = false;
        kem_l5_led_ctx.state = KEM_LED_STATE_OFF;
    }
}

bool kem_led_process_record(uint16_t keycode, keyrecord_t *record)
{
    kem_led_promote_tap_hold_tracking(keycode, record);

    if (keycode != KEM_L5)
    {
        return true;
    }

    kem_led_update_l5_key_state(record);

    if (record->event.pressed)
    {
        kem_led_render_context(&kem_l5_led_ctx);
    }
    else
    {
        kem_led_output_state(
            kem_l5_led_ctx.led,
            kem_l5_led_ctx.is_left_side,
            KEM_LED_STATE_OFF);

        kem_led_restore_layer_color();
    }

    return true;
}

bool kem_led_handle_key_event(uint16_t keycode, keyrecord_t *record)
{
    return kem_led_process_record(keycode, record);
}

bool kem_led_handle_event(const kem_led_event_t *event)
{
    if (!event)
    {
        return true;
    }

    switch (event->type)
    {
    case KEM_LED_EVENT_KEY:
        if (!event->record)
        {
            return true;
        }
        return kem_led_process_record(event->keycode, event->record);

    default:
        return true;
    }
}

void kem_led_task(void)
{
    kem_led_update_l5_state();
    kem_led_render_context(&kem_l5_led_ctx);

#ifdef SPLIT_KEYBOARD
    kem_led_sync_flush();
#endif
}

void kem_led_init(void)
{
#ifdef SPLIT_KEYBOARD
    if (!is_keyboard_master())
    {
        transaction_register_rpc(USER_KEM_LED_SYNC, kem_led_sync_handler);
    }
#endif
}

bool kem_led_process_matrix_event(uint8_t row,
                                  uint8_t col,
                                  bool pressed,
                                  uint8_t led)
{
    if (led == KEM_NO_LED)
    {
        return true;
    }

    bool is_left_side = row < 4;

    if (pressed)
    {
        kem_led_track_matrix_candidate(row, col, pressed, led);
        kem_led_output_state(led, is_left_side, KEM_LED_STATE_TAP);
    }
    else
    {
        if (kem_tap_hold_ctx.row == row && kem_tap_hold_ctx.col == col)
        {
            kem_tap_hold_ctx.active = false;
            kem_tap_hold_ctx.hold_confirmed = false;
        }

        kem_led_restore_layer_color();
    }

    return true;
}

#else

bool kem_led_process_matrix_event(uint8_t row,
                                  uint8_t col,
                                  bool pressed,
                                  uint8_t led)
{
    return true;
}

bool kem_led_process_record(uint16_t keycode, keyrecord_t *record)
{
    return true;
}

void kem_led_task(void)
{
}

void kem_led_init(void)
{
}

#endif