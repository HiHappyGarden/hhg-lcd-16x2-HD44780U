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
bool hgd_lcd_init(hgd_error_t** error)
{
    gpio_set_value(HGD_GPIO_LED_RS, false);
    gpio_set_value(HGD_GPIO_LED_RW, false);
    gpio_set_value(HGD_GPIO_LED_E, false);
    gpio_set_value(HGD_GPIO_LED_BL, false);
    gpio_set_value(HGD_GPIO_LED_D4, false);
    gpio_set_value(HGD_GPIO_LED_D5, false);
    gpio_set_value(HGD_GPIO_LED_D6, false);
    gpio_set_value(HGD_GPIO_LED_D7, false);

//hgd_lcd_send('c');
    usleep_range(40000, 2000);

    gpio_set_value(HGD_GPIO_LED_D4, true);
    gpio_set_value(HGD_GPIO_LED_D5, true);

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
    gpio_set_value(HGD_GPIO_LED_D4, (value >> 0 ) & 0x01);
    gpio_set_value(HGD_GPIO_LED_D5, (value >> 1 ) & 0x01);
    gpio_set_value(HGD_GPIO_LED_D6, (value >> 3 ) & 0x01);
    gpio_set_value(HGD_GPIO_LED_D7, (value >> 4 ) & 0x01);

    hgd_lcd_pulse();
}

static void hgd_lcd_send(__u8 value)
{
    hgd_lcd_write(value >> 0x04);
    hgd_lcd_write(value);
}