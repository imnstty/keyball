/******************************************************************************
 * @file      oled.h
 * @project   Keyball44 Custom Firmware
 * @brief     OLED Display Control Header
 *
 * @version   1.00
 * @date      2026-06-30
 *
 * @details
 * Header file for the OLED display control module.
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
 * File      : oled.h
 * Project   : Keyball44 Custom Firmware
 * Author    : Tetsuya Imanishi
 * Version   : 1.00
 * Date      : 2026-06-30
 *
 * Description
 *   OLED display module header.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include QMK_KEYBOARD_H

#ifdef OLED_ENABLE
bool oled_task_custom(void);
void oled_next_page(void);
#endif