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



#ifndef _HDG_GPIO_CONFIG_
#define _HDG_GPIO_CONFIG_

#include "error.h"
#include "constants.h"

#include <linux/gpio.h>

/// IN | Button pin
#define HGD_GPIO_BUTTON      (3) 

/// OUT | Led pin
#define HGD_GPIO_LED        (24)

/// OUT | Releay port 1
#define HGD_GPIO_RELEAY_IN1 (25) 

/// OUT | Releay port 2
#define HGD_GPIO_RELEAY_IN2 (8)

/// OUT | Releay port 3
#define HGD_GPIO_RELEAY_IN3 (7) 

/// OUT | Releay port 4
#define HGD_GPIO_RELEAY_IN4 (1) 

/// OUT | Register select
#define HGD_GPIO_LED_RS     (26)  

 /// OUT | Read and write
#define HGD_GPIO_LED_RW     (9)

 /// OUT | Enable
#define HGD_GPIO_LED_E      (19)

/// OUT | Balcklighting anode
#define HGD_GPIO_LED_BL     (13) 

/// OUT | Bit 0
#define HGD_GPIO_LED_DB4     (13) 

/// OUT | Bit 1
#define HGD_GPIO_LED_DB5     (6) 

/// OUT | Bit 2
#define HGD_GPIO_LED_DB6     (5) 

///< OUT | Bit 3
#define HGD_GPIO_LED_DB7     (11) 


/**
 * @brief Init pin configuration
 * 
 * @param[out] error if pass NULL not return erro 
 * @return 0 init correctly
 * @return false some error
 */
bool hgd_gpio_config_init(struct hgd_error** error);

/**
 * @brief Free pin config
 * 
 */
void hgd_gpio_config_unexport(void);

/**
 * @brief Free pin config
 * 
 */
void hgd_gpio_config_free(void);

#endif
