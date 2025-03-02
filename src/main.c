#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CSC450 Group 4");
MODULE_DESCRIPTION("Tiered memory module.");

static int __init tmem_init(void) {
	printk(KERN_INFO "Hello world!\n");
	return 0;
}

static int __exit tmem_exit(void)
{
	printk(KERN_INFO "Goodbye world!\n");
}

module_init(tmem_init);
module_exit(tmem_exit);
