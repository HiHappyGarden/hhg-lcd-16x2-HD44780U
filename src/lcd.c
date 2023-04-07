/* 
 * This file is part of the Happy GardenPI distribution (https://github.com/HappyGardenPI/happy-gardenpi-driver).
 * Copyright (c) 2022 Antonio Salsi.
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


static void hgd_lcd_pulse(void);
static void hgd_lcd_write(__u8 value);
static void hgd_lcd_send(__u8 value);

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

//hgd_lcd_send('c');
    usleep_range(41*1000, 50*1000);	// wait for more than 40 ms once the power is on

    gpio_set_value(HGD_GPIO_LED_DB4, true);
    gpio_set_value(HGD_GPIO_LED_DB5, true);
    usleep_range(5*1000, 6*1000);	// wait for more than 4.1 ms
    
    gpio_set_value(HGD_GPIO_LED_DB4, true);
    gpio_set_value(HGD_GPIO_LED_DB5, true);
    usleep_range(100,200);		// wait for more than 100 us

    gpio_set_value(HGD_GPIO_LED_DB4, true);
    gpio_set_value(HGD_GPIO_LED_DB5, true);
    usleep_range(100,200);		// wait for more than 100 us

    gpio_set_value(HGD_GPIO_LED_DB4, false);
    gpio_set_value(HGD_GPIO_LED_DB5, true);
    usleep_range(100,200);		// wait for more than 100 us

    gpio_set_value(HGD_GPIO_LED_DB4, false);
    gpio_set_value(HGD_GPIO_LED_DB5, true);
    usleep_range(100,200);		// wait for more than 100 us

    gpio_set_value(HGD_GPIO_LED_DB4, false);
    gpio_set_value(HGD_GPIO_LED_DB5, false);
    gpio_set_value(HGD_GPIO_LED_DB7, true);
    usleep_range(41*1000,50*1000);

    usleep_range(41*1000,50*1000);
    gpio_set_value(HGD_GPIO_LED_DB7, false);
    gpio_set_value(HGD_GPIO_LED_DB7, true); 
    usleep_range(100,200);		// wait for more than 100 us

    return true;
}


void hgd_lcd_pulse(void)
{
    gpio_set_value(HGD_GPIO_LED_E, false);
    usleep_range(1, 10);
    gpio_set_value(HGD_GPIO_LED_E, true);
    usleep_range(1, 10);
    gpio_set_value(HGD_GPIO_LED_E, false);
    usleep_range(100, 200);
}

void hgd_lcd_write(__u8 value)
{
    gpio_set_value(HGD_GPIO_LED_DB4, (value >> 0 ) & 0x01);
    gpio_set_value(HGD_GPIO_LED_DB5, (value >> 1 ) & 0x01);
    gpio_set_value(HGD_GPIO_LED_DB6, (value >> 3 ) & 0x01);
    gpio_set_value(HGD_GPIO_LED_DB7, (value >> 4 ) & 0x01);

    hgd_lcd_pulse();
}

static void hgd_lcd_send(__u8 value)
{
    hgd_lcd_write(value >> 0x04);
    hgd_lcd_write(value);
}