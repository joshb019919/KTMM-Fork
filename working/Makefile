obj-m += testc.o
KDIR ?= /lib/modules/`uname -r`/build
PWD := $(CURDIR)
BUILDDIR := $(PWD)/build

.PHONY: all
all:
	make -C $(KDIR) M=$(PWD) modules

.PHONY: clean
clean:
	make -C $(KDIR) M=$(PWD) clean
