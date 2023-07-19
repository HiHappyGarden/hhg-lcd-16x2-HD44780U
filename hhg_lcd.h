/***************************************************************************
 * 
 * Hi Happy Garden LCD Driver
 * Copyright (C) 2023  Antonio Salsi <passy.linux@zresa.it>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 * 
 ***************************************************************************/

#ifndef _HHG_LCD_H_
#define _HHG_LCD_H_

#include <linux/types.h>

#define HHG_DRIVER_NAME "hhg_lcd"
#define HHG_CLASS_NAME "hhg_lcd"
#define HHG_MAJOR_NUM_START (0)
#define HHG_MINOR_NUM_COUNT (1)

#define HHG_ROWS (2)
#define HHG_COLS (16)


#define HHG_COMMAND_MODE (0)
#define HHG_DATA_MODE (1)

//Sets entire display (D) on/off,
// cursor on/off (C), and
// blinking of cursor position
// character (B).
enum hhg_lcd_flag
{
    HHG_LCD_DISPLAY_OFF = 0x00,
    HHG_LCD_BLINK_ON    = 0x01,
    HHG_LCD_CURSOR_ON   = 0x02,
    HHG_LCD_DISPLAY_ON  = 0x04
};

/**
 * @brief Sends a command to the LCD.
 *
 * This function sends a command to the LCD.
 *
 * @param command The command to be sent.
 */
 void hhg_lcd_send_command(u8 command);

/**
 * @brief Sends char to the LCD.
 *
 * This function sends the specified char to the LCD.
 *
 * @param byte The char to be sent.
 */
void hhg_lcd_send_char(char byte);

/**
 * @brief Sends a string to the LCD.
 *
 * This function sends the specified string to the LCD.
 *
 * @param buff Pointer to the string to be sent.
 * @param len The length of the string.
 */
void hhg_lcd_send_str(const char* buff);

void hhg_lcd_clear(void);

void hhg_lcd_select_line(u8 line);

void hhg_lcd_set_flags(u8 flags);

#endif