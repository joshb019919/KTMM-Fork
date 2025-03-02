#include <linux/module.h>
#include <linux/kernel.h>

#include "mnscan.h"

void foo(void) {
	printk( KERN_INFO "Call from other file.\n" );
}
