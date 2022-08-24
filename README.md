# happy-gardenpi-driver
Driver to get access to hardware resources

## Set access mode n o root
Create the different groups (gpio is the specific for this question):
```
sudo groupadd -g 123 spi
sudo groupadd -g 124 gpio
sudo groupadd happy_gardenpi_driver
```

Assign them to your user:  
```
sudo usermod -a -G spi $USER
sudo usermod -a -G gpio $USER
sudo usermod -a -G i2c $USER
sudo usermod -a -G happy_gardenpi_driver  $USER
```

And create the file /etc/udev/rules.d/999-com.rules with the content:
```
SUBSYSTEM=="spidev" , GROUP="spi" , MODE="0660"
KERNEL=="i2c-0"     , GROUP="i2c" , MODE="0660"
KERNEL=="i2c-[1-9]*", GROUP="i2c" , MODE="0660"
KERNEL=="gpiomem"   , GROUP="gpio", MODE="0660"
KERNEL=="happy_gardenpi_driver"   , GROUP="happy_gardenpi_driver", MODE="0660"
```