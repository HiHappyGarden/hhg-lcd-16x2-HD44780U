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



#ifndef _HDG_RELAY_
#define _HDG_RELAY_

#include "constants.h"

#include <linux/init.h>

/**
 * @brief Set all port to off
 */
void hgd_relay_init(void);

/**
 * @brief Turn on/off relay
 * @param relay id
 * @param set true = on
 * @param set false = off
 */
void hgd_relay_set_state(enum hgd_type relay, bool set);

/**
 * @brief Togle relay
 * 
 */
void hgd_relay_toggle_state(enum hgd_type relay);

/**
 * @brief Get actual relay status
 * 
 * @return true Turn on
 * @return false Turn off
 */
bool hgd_relay_get_state(enum hgd_type relay);


#endif
