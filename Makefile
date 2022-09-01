# make -C /lib/modules/$(uname -r)/build M=$(pwd) modules

obj-m += happy_gardenpi_driver.o 
happy_gardenpi_driver-objs := src/main.o \
							src/button.o \
							src/lcd.o \
							src/relay.o \
							src/gpio_config.o \
							src/error.o \
							src/led.o \
							src/parser.o 

ccflags-y := -std=gnu11 -Wno-declaration-after-statement
EXTRA_CFLAGS:= -D TEST=2

PWD := $(CURDIR)
 
all: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules 
 
clean: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean


