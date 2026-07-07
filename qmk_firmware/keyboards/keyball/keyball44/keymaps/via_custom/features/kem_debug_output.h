/*
Copyright 2026 Tetsuya Imanishi
*/

#pragma once

#include QMK_KEYBOARD_H
#include "kem_debug.h"

void kem_debug_output_send(const kem_debug_event_t *event);