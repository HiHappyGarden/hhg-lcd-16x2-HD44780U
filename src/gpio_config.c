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
    pr_info("GPIOs configuration it's valid!");

    gpio_direction_input(HGD_BUTTON_GPIO);

#ifdef EN_DEBOUNCE
  //Debounce the button with a delay of 200ms
  if(gpio_set_debounce(HGD_BUTTON_GPIO, 200) < 0)
  {
    char err[10];
    sprintf(err, "%u", HGD_BUTTON_GPIO);
    hgd_error_new(error, HGD_ERROR_GPIO_DEBOUNCE, "Imppossible init debouce"); 
    return false;
  }
#endif

    gpio_direction_output(HGD_LED_GPIO, 0);
    gpio_direction_output(HGD_RELEAY_IN1_GPIO, 0);
    gpio_direction_output(HGD_RELEAY_IN2_GPIO, 0);
    gpio_direction_output(HGD_RELEAY_IN3_GPIO, 0);
    gpio_direction_output(HGD_RELEAY_IN4_GPIO, 0);
    gpio_direction_output(HGD_LCD_RS_GPIO, 0);
    gpio_direction_output(HGD_LCD_E_GPIO, 0);
    gpio_direction_output(HGD_LCD_BL_GPIO, 0);
    gpio_direction_output(HGD_LCD_D4_GPIO, 0);
    gpio_direction_output(HGD_LCD_D5_GPIO, 0);
    gpio_direction_output(HGD_LCD_D6_GPIO, 0);
    gpio_direction_output(HGD_LCD_D7_GPIO, 0);


    /* Using this call the GPIO 21 will be visible in /sys/class/gpio/
     ** Now you can change the gpio values by using below commands also.
     ** echo 1 > /sys/class/gpio/gpio21/value  (turn ON the LED)
     ** echo 0 > /sys/class/gpio/gpio21/value  (turn OFF the LED)
     ** cat /sys/class/gpio/gpio21/value  (read the value LED)
     **
     ** the second argument prevents the direction from being changed.
     */
    gpio_export(HGD_BUTTON_GPIO, false);
    gpio_export(HGD_LED_GPIO, false);
    gpio_export(HGD_RELEAY_IN1_GPIO, false);
    gpio_export(HGD_RELEAY_IN2_GPIO, false);
    gpio_export(HGD_RELEAY_IN3_GPIO, false);
    gpio_export(HGD_RELEAY_IN4_GPIO, false);
    gpio_export(HGD_LCD_RS_GPIO, false);
    gpio_export(HGD_LCD_E_GPIO, false);
    gpio_export(HGD_LCD_BL_GPIO, false);
    gpio_export(HGD_LCD_D4_GPIO, false);
    gpio_export(HGD_LCD_D5_GPIO, false);
    gpio_export(HGD_LCD_D6_GPIO, false);
    gpio_export(HGD_LCD_D7_GPIO, false);
    return true;

r_gpio:
    hgd_gpio_config_free();
    return false;
}


void hgd_gpio_config_unexport(void)
{
    gpio_unexport(HGD_BUTTON_GPIO);
    gpio_unexport(HGD_LED_GPIO);
    gpio_unexport(HGD_RELEAY_IN1_GPIO);
    gpio_unexport(HGD_RELEAY_IN2_GPIO);
    gpio_unexport(HGD_RELEAY_IN3_GPIO);
    gpio_unexport(HGD_RELEAY_IN4_GPIO);
    gpio_unexport(HGD_LCD_RS_GPIO);
    gpio_unexport(HGD_LCD_E_GPIO);
    gpio_unexport(HGD_LCD_BL_GPIO);
    gpio_unexport(HGD_LCD_D4_GPIO);
    gpio_unexport(HGD_LCD_D5_GPIO);
    gpio_unexport(HGD_LCD_D6_GPIO);
    gpio_unexport(HGD_LCD_D7_GPIO);
}

void hgd_gpio_config_free(void)
{
    gpio_free(HGD_BUTTON_GPIO);
    gpio_free(HGD_LED_GPIO);
    gpio_free(HGD_RELEAY_IN1_GPIO);
    gpio_free(HGD_RELEAY_IN2_GPIO);
    gpio_free(HGD_RELEAY_IN3_GPIO);
    gpio_free(HGD_RELEAY_IN4_GPIO);
    gpio_free(HGD_LCD_RS_GPIO);
    gpio_free(HGD_LCD_E_GPIO);
    gpio_free(HGD_LCD_BL_GPIO);
    gpio_free(HGD_LCD_D4_GPIO);
    gpio_free(HGD_LCD_D5_GPIO);
    gpio_free(HGD_LCD_D6_GPIO);
    gpio_free(HGD_LCD_D7_GPIO);
}

bool hgd_gpio_consig_is_valid(hgd_error_t** error)
{
    // Checking the GPIO is valid or not
    HGD_IS_VALID(HGD_BUTTON_GPIO, "HGD_BUTTON_GPIO")
    HGD_IS_VALID(HGD_LED_GPIO, "HGD_LED_GPIO")
    HGD_IS_VALID(HGD_RELEAY_IN1_GPIO, "HGD_RELEAY_IN1_GPIO")
    HGD_IS_VALID(HGD_RELEAY_IN2_GPIO, "HGD_RELEAY_IN2_GPIO")
    HGD_IS_VALID(HGD_RELEAY_IN3_GPIO, "HGD_RELEAY_IN3_GPIO")
    HGD_IS_VALID(HGD_RELEAY_IN4_GPIO, "HGD_RELEAY_IN4_GPIO")
    HGD_IS_VALID(HGD_LCD_RS_GPIO, "HGD_LCD_RS_GPIO")
    HGD_IS_VALID(HGD_LCD_E_GPIO, "HGD_LCD_E_GPIO")
    HGD_IS_VALID(HGD_LCD_BL_GPIO, "HGD_LCD_BL_GPIO")
    HGD_IS_VALID(HGD_LCD_D4_GPIO, "HGD_BUTTON_GPIO")
    HGD_IS_VALID(HGD_LCD_D5_GPIO, "HGD_BUTTON_GPIO")
    HGD_IS_VALID(HGD_LCD_D6_GPIO, "HGD_LCD_D6_GPIO")
    HGD_IS_VALID(HGD_LCD_D7_GPIO, "HGD_LCD_D7_GPIO")
    

    return true;
}

bool hgd_gpio_config_request(hgd_error_t** error)
{
    HGD_REQUEST(HGD_BUTTON_GPIO, "HGD_BUTTON_GPIO")
    HGD_REQUEST(HGD_LED_GPIO, "HGD_LED_GPIO")
    HGD_REQUEST(HGD_RELEAY_IN1_GPIO, "HGD_RELEAY_IN1_GPIO")
    // HGD_REQUEST(HGD_RELEAY_IN2_GPIO, "HGD_RELEAY_IN2_GPIO")
    HGD_REQUEST(HGD_RELEAY_IN3_GPIO, "HGD_RELEAY_IN3_GPIO")
    HGD_REQUEST(HGD_RELEAY_IN4_GPIO, "HGD_RELEAY_IN4_GPIO")
    HGD_REQUEST(HGD_LCD_RS_GPIO, "HGD_LCD_RS_GPIO")
    HGD_REQUEST(HGD_LCD_E_GPIO, "HGD_LCD_E_GPIO")
    HGD_REQUEST(HGD_LCD_BL_GPIO, "HGD_LCD_BL_GPIO")
    HGD_REQUEST(HGD_LCD_D4_GPIO, "HGD_BUTTON_GPIO")
    HGD_REQUEST(HGD_LCD_D5_GPIO, "HGD_BUTTON_GPIO")
    HGD_REQUEST(HGD_LCD_D6_GPIO, "HGD_LCD_D6_GPIO")
    HGD_REQUEST(HGD_LCD_D7_GPIO, "HGD_LCD_D7_GPIO")
    return true;
}