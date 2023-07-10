program_name = cdev

obj-m += hhg_lcd.o 

ccflags-y := -std=gnu11 -Wno-declaration-after-statement
EXTRA_CFLAGS:= -D TEST=1

PWD := $(CURDIR)
 
all: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules 
 
clean: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

# Utilities for fast testing ...
insert:
	sudo insmod $(program_name).ko	# insert $ make i
remove:
	sudo rmmod $(program_name)		# remove $ make r
print:
	sudo dmesg				# print  $ make p
c:
	sudo dmesg -c		# clear dmesg	 $ make c
info:
	sudo modinfo $(program_name).ko #userspace program prints .modinfo section

