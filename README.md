# happy-gardenpi-driver
Simple driver for LCD 16x2 management with chip HITACHI HD44780U compatible.  

At the moment, only the 4-bit configuration is supported.  

To compile the module:
```
git clone https://github.com/HiHappyGarden/hhg-lcd-16x2-HD44780U.git
cd hhg-lcd-16x2-HD44780U
sudo make
```

To install the module:
```
insmod hhg_lcd.ko gpio_rs=26 gpio_en=19 gpio_db4=13 gpio_db5=6 gpio_db6=5 gpio_db7=11
```
gpio_rs=x gpio_en=x gpio_db4=x gpio_db5=x gpio_db6=x gpio_db7=x corrispondono ai pin dell'LCD riporaro nella documentazione linkata sotto.  

The above configuration has been tested on a Raspberry Pi 4.  

To remove the module:
```
sudo make remove
```

## Documentation reference
 * [HITACHI HD44780U](https://www.sparkfun.com/datasheets/LCD/HD44780.pdf)

