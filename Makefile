# make -C /lib/modules/$(uname -r)/build M=$(pwd) modules

happygardenpidriver-objs := src/happygardenpidriver.o 

PWD := $(CURDIR)
 
all: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules 
 
clean: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean


