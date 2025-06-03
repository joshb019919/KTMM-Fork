KDIR = /lib/modules/`uname -r`/build
PWD := $(CURDIR)
BUILDDIR := $(PWD)/build

obj-m += ktmm.o
ktmm-y := ktmm_main.o 
ktmm-y += ktmm_hook.o 
ktmm-y += ktmm_vmscan.o

all:
	$(MAKE) -C $(KDIR) M=$(PWD) src=$(PWD) MO=$(BUILDDIR) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	