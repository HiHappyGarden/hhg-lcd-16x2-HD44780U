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

/**
 * @brief Sends data to the LCD.
 *
 * This function sends the specified data to the LCD.
 *
 * @param data The data to be sent.
 */
void hhg_lcd_send_data(__u8 data);

/**
 * @brief Sends a string to the LCD.
 *
 * This function sends the specified string to the LCD.
 *
 * @param str Pointer to the string to be sent.
 * @param len The length of the string.
 */
void hhg_lcd_send_str(const char* str, __u16 len);


#endif