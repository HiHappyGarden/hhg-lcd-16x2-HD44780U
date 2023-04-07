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



#ifndef _HDG_LCD_
#define _HDG_LCD_
#include "constants.h"
#include "error.h"

#include <linux/init.h>

#define HDG_LCD_WIDTH (16)
#define HDG_LCD_HEIGHT (2)

/**
 * @brief Init lcd
 * 
 * @param[out] error if pass NULL not return erro 
 * @return true init correctly
 * @return false some error
 */
bool hgd_lcd_init(struct hgd_error** error);

#endif
