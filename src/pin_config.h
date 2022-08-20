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

#define HGD_BUTTON_PIN      5 //!< IN | Button pin

#define HGD_RELEAY_IN1_PIN  22 //!< OUT | Releay port 1
#define HGD_RELEAY_IN2_PIN  24 //!< OUT | Releay port 2
#define HGD_RELEAY_IN3_PIN  26 //!< OUT | Releay port 3
#define HGD_RELEAY_IN4_PIN  28 //!< OUT | Releay port 4

#define HGD_LCD_RS_PIN      26 //!< OUT | Register select  
#define HGD_LCD_E_PIN       19 //!< OUT | Enable
#define HGD_LCD_BL_PIN      23 //!< OUT | Balcklighting anode
#define HGD_LCD_D4_PIN      33 //!< OUT | Bit 0
#define HGD_LCD_D5_PIN      31 //!< OUT | Bit 1
#define HGD_LCD_D6_PIN      29 //!< OUT | Bit 2
#define HGD_LCD_D7_PIN      27 //!< OUT | Bit 3

/**
 * @brief Init pin configuration
 * 
 * @return true init correctly
 * @return false some error
 */
_Bool init_pin_config(void);

#endif
