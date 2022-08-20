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



#ifndef _HDG_PIN_CONFIG_
#define _HDG_PIN_CONFIG_

#define HGD_BUTTON_PIN      5 //!< Button pin

#define HGD_RELEAY_IN1_PIN  22 //!< Releay port 1
#define HGD_RELEAY_IN2_PIN  24 //!< Releay port 2
#define HGD_RELEAY_IN3_PIN  26 //!< Releay port 3
#define HGD_RELEAY_IN4_PIN  28 //!< Releay port 4

#define HGD_LCD_RS_PIN      26 //!< Register select  
#define HGD_LCD_E_PIN       19 //!< Enable
#define HGD_LCD_BL_PIN      23 //!< Balcklighting anode
#define HGD_LCD_D4_PIN      33 //!< Bit 0
#define HGD_LCD_D5_PIN      31 //!< Bit 1
#define HGD_LCD_D6_PIN      29 //!< Bit 2
#define HGD_LCD_D7_PIN      27 //!< Bit 3

/**
 * @brief Init pin configuration
 * 
 * @return true init correctly
 * @return false some error
 */
_Bool init_pin_config(void);

#endif
