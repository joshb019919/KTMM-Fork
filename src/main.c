#include <linux/module.h>
#include <linux/kernel.h>

#include "mnscan.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CSC450 Group 4");
MODULE_DESCRIPTION("Tiered memory module.");

int init_module(void) {
	printk(KERN_INFO "Hello world!\n");
	foo();
	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Goodbye world!\n");
}

