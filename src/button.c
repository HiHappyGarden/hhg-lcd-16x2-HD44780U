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

#include "button.h"
#include "button.h"

#include <linux/delay.h>
#include <linux/gpio.h> //GPIO
#include <linux/interrupt.h>
/* Since debounce is not supported in Raspberry pi, I have addded this to disable
** the false detection (multiple IRQ trigger for one interrupt).
** Many other hardware supports GPIO debounce, I don't want care about this even
** if this has any overhead. Our intention is to explain the GPIO interrupt.
** If you want to disable this extra coding, you can comment the below macro.
** This has been taken from : https://raspberrypi.stackexchange.com/questions/8544/gpio-interrupt-debounce
**
** If you want to use Hardaware Debounce, then comment this EN_DEBOUNCE.
**
*/

#include "error.h"
#include "constants.h"
#include "gpio_config.h"

#include <linux/jiffies.h>

#ifdef pr_fmt
#undef pr_fmt
#define pr_fmt(fmt) HGD_NAME ": " fmt
#endif

#define DIFF_JIFFIES 50

extern unsigned long volatile jiffies;
unsigned long old_jiffies = 0;



static __u32 gpio_irq_number;
static atomic_t thread_busy = ATOMIC_INIT(0);

static irqreturn_t gpio_irq_handler(int irq, void *dev_id);
static irqreturn_t gpio_interrupt_thread_fn(int irq, void *dev_id);

bool hgd_button_init(hgd_error_t **error)
{
  // Get the IRQ number for our GPIO
  gpio_irq_number = gpio_to_irq(HGD_BUTTON_GPIO);

  if (request_threaded_irq(gpio_irq_number,           // IRQ number
                           (void *)gpio_irq_handler,  // IRQ handler (Top half)
                           gpio_interrupt_thread_fn,  // IRQ Thread handler (Bottom half)
                           IRQF_TRIGGER_FALLING,         // Handler will be called in raising edge
                           HGD_NAME,                  // used to identify the device name using this IRQ
                           NULL))                     // device id for shared IRQ
  {
    hgd_error_new(error, HGD_ERROR_GPIO_IRQ, "cannot register IRQ");
    return false;
  }

  return true;
}

inline bool hgd_button_get_state(void)
{
  return !gpio_get_value(HGD_BUTTON_GPIO);
}

inline void hgd_button_free(void)
{
  free_irq(gpio_irq_number, NULL);
}

// Interrupt handler for GPIO 25. This will be called whenever there is a raising edge detected.
irqreturn_t gpio_irq_handler(int irq, void *dev_id)
{
  //avoid continuos click
  unsigned long diff = jiffies - old_jiffies;
  if (diff < DIFF_JIFFIES)
  {
    return IRQ_HANDLED;
  }

  old_jiffies = jiffies;


  pr_info("Interrupt(IRQ Handler) %lu \n", jiffies);


  if (atomic_read(&thread_busy) > 0)
  {
      pr_info("Thread busy\n");
      return IRQ_HANDLED;
  }

  atomic_inc(&thread_busy);

  /*
  ** If you don't want to call the thread fun, then you can just return
  ** IRQ_HANDLED. If you return IRQ_WAKE_THREAD, then thread fun will be called.
  */
  return IRQ_WAKE_THREAD;
}

irqreturn_t gpio_interrupt_thread_fn(int irq, void *dev_id) 
{

  __u32  i = 100;
  while (true && i)
  {
    pr_info("Interrupt(Threaded Handler) : irq: %d HGD_BUTTON_GPIO : %d ", irq, hgd_button_get_state());
    mdelay(5);
    i--;
  }
  
  atomic_sub(1, &thread_busy);

  return IRQ_HANDLED;
}