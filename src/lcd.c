/* 
 * This file is part of the Happy GardenPI distribution (https://github.com/HappyGardenPI/happy-gardenpi-driver).
 * Copyright (c) 2022-23 Antonio Salsi.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */


#include "lcd.h"
#include "gpio_config.h"

#include <linux/delay.h>

#define RS_COMMAND_MODE		(0)   // command mode to select Insruction register with RS signal
#define RS_DATA_MODE		(1)   // data mode to select Data register with RS signal

#define LCD_FIRST_LINE		(1)
#define LCD_SECOND_LINE		(2)

static void hgd_lcd_bootstap(void);
static void hgd_lcd_write_nibble(__u8 value);
static void hgd_lcd_write_nibble_debug(__u8 value, const char* msg);
static void hgd_lcd_write(__u8 value);
static void hgd_lcd_data(char data);

/**
 * @brief Init lcd
 * 
 */
bool hgd_lcd_init(struct hgd_error** error)
{
    gpio_set_value(HGD_GPIO_LED_RS, false);
    gpio_set_value(HGD_GPIO_LED_RW, false);
    gpio_set_value(HGD_GPIO_LED_E, false);
    gpio_set_value(HGD_GPIO_LED_BL, false);
    gpio_set_value(HGD_GPIO_LED_DB4, false);
    gpio_set_value(HGD_GPIO_LED_DB5, false);
    gpio_set_value(HGD_GPIO_LED_DB6, false);
    gpio_set_value(HGD_GPIO_LED_DB7, false);

    hgd_lcd_bootstap();

    hgd_lcd_data('c');
    hgd_lcd_data('i');
    hgd_lcd_data('a');

    return true;
}

static void hgd_lcd_bootstap(void)
{
	usleep_range(41*1000, 50*1000);	// wait for more than 40 ms once the power is on

	hgd_lcd_write(0x30);		// Instruction 0011b (Function set)
	usleep_range(5*1000, 6*1000);	// wait for more than 4.1 ms

	hgd_lcd_write(0x30);		// Instruction 0011b (Function set)
	usleep_range(100,200);		// wait for more than 100 us

	hgd_lcd_write(0x30);		// Instruction 0011b (Function set)
	usleep_range(100,200);		// wait for more than 100 us

	hgd_lcd_write(0x20);		/* Instruction 0010b (Function set)
					   Set interface to be 4 bits long
					*/
	usleep_range(100,200);		// wait for more than 100 us

	hgd_lcd_write(0x20);		// Instruction 0010b (Function set)
	hgd_lcd_write(0x80);		/* Instruction NF**b
					   Set N = 1, or 2-line display
					   Set F = 0, or 5x8 dot character font
					 */
	usleep_range(41*1000,50*1000);

					/* Display off */
	hgd_lcd_write(0x00);		// Instruction 0000b
	hgd_lcd_write(0x80);		// Instruction 1000b
	usleep_range(100,200);

					/* Display clear */
	hgd_lcd_write(0x00);		// Instruction 0000b
	hgd_lcd_write(0x10);		// Instruction 0001b
	usleep_range(100,200);

					/* Entry mode set */
	hgd_lcd_write(0x00);		// Instruction 0000b
	hgd_lcd_write(0x60);		/* Instruction 01(I/D)Sb -> 0110b
					   Set I/D = 1, or increment or decrement DDRAM address by 1
					   Set S = 0, or no display shift
					*/
	usleep_range(100,200);

	/* Initialization Completed, but set up default LCD setting here */

					/* Display On/off Control */
	hgd_lcd_write(0x00);		// Instruction 0000b
	hgd_lcd_write(0xF0);		/* Instruction 1DCBb  
					   Set D= 1, or Display on
					   Set C= 1, or Cursor on
					   Set B= 1, or Blinking on
					*/
	usleep_range(100,200);
}

void hgd_lcd_write_nibble(__u8 value)
{
    gpio_set_value(HGD_GPIO_LED_DB4, (value >> 0 ) & 0x01);
    gpio_set_value(HGD_GPIO_LED_DB5, (value >> 1 ) & 0x01);
    gpio_set_value(HGD_GPIO_LED_DB6, (value >> 2 ) & 0x01);
    gpio_set_value(HGD_GPIO_LED_DB7, (value >> 3 ) & 0x01);

}


void hgd_lcd_write_nibble_debug(__u8 value, const char* msg)
{
    hgd_lcd_write_nibble(value);

    pr_info("%s DB4:%u DB5:%u DB6:%u DB7:%u ", msg, (value >> 0 ) & 0x01, (value >> 1 ) & 0x01, (value >> 2 ) & 0x01, (value >> 3 ) & 0x01);
}

void hgd_lcd_write(__u8 value)
{
    hgd_lcd_write_nibble(value >> 0x04);
    hgd_lcd_write_nibble(value);
}

void hgd_lcd_data(char data)
{
    // Part 1.  Upper 4 bit data (from bit 7 to bit 4)
	usleep_range(2000, 3000); 	// added delay instead of busy checking

    hgd_lcd_write_nibble_debug( data >> 0x04, "hight");

    // Part 1. Set to data mode
	gpio_set_value(HGD_GPIO_LED_RS, RS_DATA_MODE);
	usleep_range(5, 10);

	// Part 1. Simulate falling edge triggered clock
	gpio_set_value(HGD_GPIO_LED_E, 1);
	usleep_range(5, 10);
	gpio_set_value(HGD_GPIO_LED_E, 0);

    // Part 2. Lower 4 bit data (from bit 3 to bit 0)
	usleep_range(2000, 3000);	// added delay instead of busy checking

    // Part 2. Lower 4 bit data (from bit 3 to bit 0)
    hgd_lcd_write_nibble_debug(data, "low");

    // Part 2. Set to data mode
	gpio_set_value(HGD_GPIO_LED_RS, RS_DATA_MODE);
    usleep_range(5, 10);

	// Part 2. Simulate falling edge triggered clock
	gpio_set_value(HGD_GPIO_LED_E, 1);
	usleep_range(5, 10);
	gpio_set_value(HGD_GPIO_LED_E, 0);	
}