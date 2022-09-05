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

#include "gpio_config.h"

#include <linux/init.h>

#define HGD_IS_VALID(gpio, msg) \
    if (gpio_is_valid(gpio) == false) \
    { \
        hgd_error_new(error, HGD_ERROR_GPIO_NOT_VALID, msg); \
        return false; \
    } 
#define HGD_REQUEST(gpio, msg) \
    if (gpio_request(gpio, #gpio) < 0) \
    { \
        hgd_error_new(error, HGD_ERROR_GPIO_REQUEST, msg); \
        return false; \
    }

static bool hgd_gpio_consig_is_valid(hgd_error_t** error);
static bool hgd_gpio_config_request(hgd_error_t** error);

bool hgd_gpio_config_init(hgd_error_t** error)
{
    if (hgd_gpio_consig_is_valid(error) == false)
    {
        goto r_gpio;
    }
    

    if (hgd_gpio_config_request(error) == false)
    {
        goto r_gpio;
    }

    gpio_direction_input(HGD_GPIO_BUTTON);

#ifdef EN_DEBOUNCE
  //Debounce the button with a delay of 200ms
  if(gpio_set_debounce(HGD_GPIO_BUTTON, 200) < 0)
  {
    char err[10];
    sprintf(err, "%u", HGD_GPIO_BUTTON);
    hgd_error_new(error, HGD_ERROR_GPIO_DEBOUNCE, "Imppossible init debouce"); 
    return false;
  }
#endif

    gpio_direction_output(HGD_GPIO_LED, 0);
    gpio_direction_output(HGD_GPIO_RELEAY_IN1, 0);
    gpio_direction_output(HGD_GPIO_RELEAY_IN2, 0);
    gpio_direction_output(HGD_GPIO_RELEAY_IN3, 0);
    gpio_direction_output(HGD_GPIO_RELEAY_IN4, 0);
    gpio_direction_output(HGD_GPIO_LED_RS, 0);
    gpio_direction_output(HGD_GPIO_LED_RW, 0);
    gpio_direction_output(HGD_GPIO_LED_E, 0);
    gpio_direction_output(HGD_GPIO_LED_BL, 0);
    gpio_direction_output(HGD_GPIO_LED_D4, 0);
    gpio_direction_output(HGD_GPIO_LED_D5, 0);
    gpio_direction_output(HGD_GPIO_LED_D6, 0);
    gpio_direction_output(HGD_GPIO_LED_D7, 0);


    /* Using this call the GPIO 21 will be visible in /sys/class/gpio/
     ** Now you can change the gpio values by using below commands also.
     ** echo 1 > /sys/class/gpio/gpio21/value  (turn ON the LED)
     ** echo 0 > /sys/class/gpio/gpio21/value  (turn OFF the LED)
     ** cat /sys/class/gpio/gpio21/value  (read the value LED)
     **
     ** the second argument prevents the direction from being changed.
     */
    gpio_export(HGD_GPIO_BUTTON, false);
    gpio_export(HGD_GPIO_LED, false);
    gpio_export(HGD_GPIO_RELEAY_IN1, false);
    gpio_export(HGD_GPIO_RELEAY_IN2, false);
    gpio_export(HGD_GPIO_RELEAY_IN3, false);
    gpio_export(HGD_GPIO_RELEAY_IN4, false);
    gpio_export(HGD_GPIO_LED_RS, false);
    gpio_export(HGD_GPIO_LED_RW, false);
    gpio_export(HGD_GPIO_LED_E, false);
    gpio_export(HGD_GPIO_LED_BL, false);
    gpio_export(HGD_GPIO_LED_D4, false);
    gpio_export(HGD_GPIO_LED_D5, false);
    gpio_export(HGD_GPIO_LED_D6, false);
    gpio_export(HGD_GPIO_LED_D7, false);
    return true;

r_gpio:
    hgd_gpio_config_free();
    return false;
}


void hgd_gpio_config_unexport(void)
{
    gpio_unexport(HGD_GPIO_BUTTON);
    gpio_unexport(HGD_GPIO_LED);
    gpio_unexport(HGD_GPIO_RELEAY_IN1);
    gpio_unexport(HGD_GPIO_RELEAY_IN2);
    gpio_unexport(HGD_GPIO_RELEAY_IN3);
    gpio_unexport(HGD_GPIO_RELEAY_IN4);
    gpio_unexport(HGD_GPIO_LED_RS);
    gpio_unexport(HGD_GPIO_LED_RW);
    gpio_unexport(HGD_GPIO_LED_E);
    gpio_unexport(HGD_GPIO_LED_BL);
    gpio_unexport(HGD_GPIO_LED_D4);
    gpio_unexport(HGD_GPIO_LED_D5);
    gpio_unexport(HGD_GPIO_LED_D6);
    gpio_unexport(HGD_GPIO_LED_D7);
}

void hgd_gpio_config_free(void)
{
    gpio_free(HGD_GPIO_BUTTON);
    gpio_free(HGD_GPIO_LED);
    gpio_free(HGD_GPIO_RELEAY_IN1);
    gpio_free(HGD_GPIO_RELEAY_IN2);
    gpio_free(HGD_GPIO_RELEAY_IN3);
    gpio_free(HGD_GPIO_RELEAY_IN4);
    gpio_free(HGD_GPIO_LED_RS);
    gpio_free(HGD_GPIO_LED_RW);
    gpio_free(HGD_GPIO_LED_E);
    gpio_free(HGD_GPIO_LED_BL);
    gpio_free(HGD_GPIO_LED_D4);
    gpio_free(HGD_GPIO_LED_D5);
    gpio_free(HGD_GPIO_LED_D6);
    gpio_free(HGD_GPIO_LED_D7);
}

bool hgd_gpio_consig_is_valid(hgd_error_t** error)
{
    // Checking the GPIO is valid or not
    HGD_IS_VALID(HGD_GPIO_BUTTON, "HGD_GPIO_BUTTON")
    HGD_IS_VALID(HGD_GPIO_LED, "HGD_GPIO_LED")
    HGD_IS_VALID(HGD_GPIO_RELEAY_IN1, "HGD_GPIO_RELEAY_IN1")
    HGD_IS_VALID(HGD_GPIO_RELEAY_IN2, "HGD_GPIO_RELEAY_IN2")
    HGD_IS_VALID(HGD_GPIO_RELEAY_IN3, "HGD_GPIO_RELEAY_IN3")
    HGD_IS_VALID(HGD_GPIO_RELEAY_IN4, "HGD_GPIO_RELEAY_IN4")
    HGD_IS_VALID(HGD_GPIO_LED_RS, "HGD_GPIO_LED_RS")
    HGD_IS_VALID(HGD_GPIO_LED_RS, "HGD_GPIO_LED_RW")
    HGD_IS_VALID(HGD_GPIO_LED_E, "HGD_GPIO_LED_E")
    HGD_IS_VALID(HGD_GPIO_LED_BL, "HGD_GPIO_LED_BL")
    HGD_IS_VALID(HGD_GPIO_LED_D4, "HGD_GPIO_BUTTON")
    HGD_IS_VALID(HGD_GPIO_LED_D5, "HGD_GPIO_BUTTON")
    HGD_IS_VALID(HGD_GPIO_LED_D6, "HGD_GPIO_LED_D6")
    HGD_IS_VALID(HGD_GPIO_LED_D7, "HGD_GPIO_LED_D7")
    

    return true;
}

bool hgd_gpio_config_request(hgd_error_t** error)
{
    HGD_REQUEST(HGD_GPIO_BUTTON, "HGD_GPIO_BUTTON")
    HGD_REQUEST(HGD_GPIO_LED, "HGD_GPIO_LED")
    HGD_REQUEST(HGD_GPIO_RELEAY_IN1, "HGD_GPIO_RELEAY_IN1")
    // HGD_REQUEST(HGD_GPIO_RELEAY_IN2, "HGD_GPIO_RELEAY_IN2")
    HGD_REQUEST(HGD_GPIO_RELEAY_IN3, "HGD_GPIO_RELEAY_IN3")
    HGD_REQUEST(HGD_GPIO_RELEAY_IN4, "HGD_GPIO_RELEAY_IN4")
    HGD_REQUEST(HGD_GPIO_LED_RS, "HGD_GPIO_LED_RS")
    HGD_REQUEST(HGD_GPIO_LED_E, "HGD_GPIO_LED_E")
    HGD_REQUEST(HGD_GPIO_LED_BL, "HGD_GPIO_LED_BL")
    HGD_REQUEST(HGD_GPIO_LED_D4, "HGD_GPIO_BUTTON")
    HGD_REQUEST(HGD_GPIO_LED_D5, "HGD_GPIO_BUTTON")
    HGD_REQUEST(HGD_GPIO_LED_D6, "HGD_GPIO_LED_D6")
    HGD_REQUEST(HGD_GPIO_LED_D7, "HGD_GPIO_LED_D7")
    return true;
}