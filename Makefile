KDIR ?= /lib/modules/`uname -r`/build
PWD := $(CURDIR)
BUILD_DIR := $(PWD)/build/
	
all: $(BUILD_DIR)
	$(MAKE) -C $(KDIR) M=$(BUILD_DIR) src=$(PWD) modules

$(BUILD_DIR):
	mkdir -p -v $@
	touch $@/Makefile

clean:
	$(MAKE) -C $(KDIR) M=$(BUILD_DIR) src=$(PWD) clean
	rm -r $(BUILD_DIR)
