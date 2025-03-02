#include <linux/delay.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/sched.h>
#include <linux/stat.h>


#include "mnscan.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CSC450 Group 4");
MODULE_DESCRIPTION("Tiered memory module.");

static struct task_struct *kernel_thread1;


int thread_function(void *arg) {
	int i = 0;
	while (!kthread_should_stop()) {
		pr_info("%d\n", i++);
		msleep(1000);
	}
	return 0;
}


static int __init tmem_init(void) {
	printk(KERN_INFO "Hello world!\n");
	kernel_thread1 = kthread_run(&thread_function, NULL, "thread_function");
	return 0;
}


static void __exit tmem_exit(void) {
	printk(KERN_INFO "Goodbye world!\n");
	kthread_stop(kernel_thread1);
}


module_init(tmem_init);
module_exit(tmem_exit);
