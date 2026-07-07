/*
Copyright 2026 Tetsuya Imanishi
*/

/******************************************************************************
 * @file      kem_debug_output.c
 * @project   Keyball44 Custom Firmware
 * @brief     KEM Debug Output Driver
 *
 * @version   2.11
 * @date      2026-07-07
 *
 *-----------------------------------------------------------------------------
 * Revision History
 *-----------------------------------------------------------------------------
 * Ver 2.11  2026-07-07
 * - Implemented Raw HID debug output packet for key events.
 *
 * Ver 2.10  2026-07-07
 * - Added KEM Debug Output Driver skeleton.
 * - No functional changes.
 *
 ******************************************************************************/

#include "kem_debug_output.h"

#ifdef RAW_ENABLE
#include "raw_hid.h"
#endif

#define KEM_DEBUG_PACKET_SIZE 32
#define KEM_DEBUG_MAGIC 0x4B
#define KEM_DEBUG_VERSION 0x01

#define KEM_DEBUG_FLAG_PRESSED 0x01

static void kem_debug_output_raw_hid_send(const kem_debug_event_t *event)
{
#ifndef KEM_DEBUG_ENABLE
    (void)event;
    return;
#else
#ifdef RAW_ENABLE
    if (!event)
    {
        return;
    }

    uint8_t packet[KEM_DEBUG_PACKET_SIZE] = {0};

    packet[0] = KEM_DEBUG_MAGIC;
    packet[1] = KEM_DEBUG_VERSION;
    packet[2] = (uint8_t)event->type;
    packet[3] = event->pressed ? KEM_DEBUG_FLAG_PRESSED : 0x00;

    packet[4] = (uint8_t)(event->keycode & 0xFF);
    packet[5] = (uint8_t)((event->keycode >> 8) & 0xFF);

    packet[6] = event->row;
    packet[7] = event->col;

    packet[8] = (uint8_t)(event->time & 0xFF);
    packet[9] = (uint8_t)((event->time >> 8) & 0xFF);

    raw_hid_send(packet, KEM_DEBUG_PACKET_SIZE);
#else
    (void)event;
#endif
#endif
}

void kem_debug_output_send(const kem_debug_event_t *event)
{
    kem_debug_output_raw_hid_send(event);
}