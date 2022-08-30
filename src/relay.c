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


#include "relay.h"
#include "pin_config.h"

static __u8 get_relay_gpio(hgd_type_t relay)
{
    switch (relay)
    {
    case HGD_RELAY_1:
        return HGD_RELEAY_IN1_GPIO;
    case HGD_RELAY_2:
        return HGD_RELEAY_IN2_GPIO;
    case HGD_RELAY_3:
        return HGD_RELEAY_IN3_GPIO;
    case HGD_RELAY_4:
        return HGD_RELEAY_IN4_GPIO;
    default:
        return 0;
    }
}
void hgd_relay_init(void)
{
    hgd_relay_set_state(HGD_RELAY_1, false);
    hgd_relay_set_state(HGD_RELAY_2, false);
    hgd_relay_set_state(HGD_RELAY_3, false);
    hgd_relay_set_state(HGD_RELAY_4, false);
}

void hgd_relay_set_state(hgd_type_t relay, bool state)
{
    if(hgd_relay_get_state(relay) != state)
    {
        __u8 gpio = 0;
        if((gpio = get_relay_gpio(relay)) > 0)
        {
            gpio_set_value(gpio, !state);
        }
    }
}

inline void hgd_relay_toggle(hgd_type_t relay)
{
    __u8 gpio = 0;
    if((gpio = get_relay_gpio(relay)) > 0)
    {
        hgd_relay_set_state(gpio, !hgd_relay_get_state(gpio));
    }
}

inline bool hgd_relay_get_state(hgd_type_t relay)
{
    __u8 gpio = 0;
    if((gpio = get_relay_gpio(relay)) > 0)
    {
        return !gpio_get_value(gpio);
    }
    else
    {
        return false;
    }
}