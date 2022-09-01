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


#ifndef _HDG_LED_
#define _HDG_LED_

#include <linux/init.h>

/**
 * @brief Init led to off
 * 
 */
void hgd_led_init(void);

/**
 * @brief Turn on/off led
 * 
 * @param set true = on
 * @param set false = off
 */
void hgd_led_set_state(bool set);

/**
 * @brief Togle led
 * 
 */
void hgd_led_toggle_state(void);

/**
 * @brief Get actula led status
 * 
 * @return true Turn on
 * @return false Turn off
 */
bool hgd_led_get_state(void);

#endif
