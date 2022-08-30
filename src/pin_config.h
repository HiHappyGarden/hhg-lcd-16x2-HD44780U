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

/// IN | Button pin
#define HGD_BUTTON_GPIO      (3) 

/// OUT | Led pin
#define HGD_LED_GPIO         (24)

/// OUT | Releay port 1
#define HGD_RELEAY_IN1_GPIO  (25) 

/// OUT | Releay port 2
#define HGD_RELEAY_IN2_GPIO  (8)

/// OUT | Releay port 3
#define HGD_RELEAY_IN3_GPIO  (7) 

/// OUT | Releay port 4
#define HGD_RELEAY_IN4_GPIO  (1) 

/// OUT | Register select
#define HGD_LCD_RS_GPIO      (26)  

 /// OUT | Enable
#define HGD_LCD_E_GPIO       (19)

/// OUT | Balcklighting anode
#define HGD_LCD_BL_GPIO      (13) 

/// OUT | Bit 0
#define HGD_LCD_D4_GPIO      (6) 

/// OUT | Bit 1
#define HGD_LCD_D5_GPIO      (5) 

/// OUT | Bit 2
#define HGD_LCD_D6_GPIO      (0) 

///< OUT | Bit 3
#define HGD_LCD_D7_GPIO      (11) 

#include "error.h"
#include "constants.h"

#include <linux/gpio.h>

/**
 * @brief Init pin configuration
 * 
 * @param[out] error if pass NULL not return erro 
 * @return 0 init correctly
 * @return true some error
 */
bool hgd_pin_config_init(hgd_error_t** error);

/**
 * @brief Free pin config
 * 
 */
void hgd_pin_config_unexport(void);

/**
 * @brief Free pin config
 * 
 */
void hgd_pin_config_free(void);

#endif
