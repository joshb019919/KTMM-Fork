obj-m += kthread_counter.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(CURDIR)

all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(WD) clean