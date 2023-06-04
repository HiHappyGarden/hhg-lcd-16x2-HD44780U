#!/bin/bash

sudo insmod lcd_driver/hgd_lcd.ko gpio_rs=25 gpio_en=24 gpio_db4=23 gpio_db5=22 gpio_db6=21 gpio_db7=14 
lsmod | grep lcd
