#include <linux/delay.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/sched.h>
#include <linux/stat.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Josh Borthick");

int thread_function(void*);
int thread_function(void *arg) {
    int i = 0;
    while (!kthread_should_stop()) {
        pr_info("%d\n", i++);
        msleep(1000);
    }
    return 0;
}
 
static struct task_struct *kernel_thread1;
static int __init threaded_module(void) {
    kernel_thread1 = kthread_run(&thread_function, NULL, "thread_function");

    // if (kernel_thread) {
    //     wake_up_process(kernel_thread);
    // } else {
    //     pr_err("The kernel thread was not created with error %d\n", kernel_thread->exit_code);
    // }

    return 0;
}

static void __exit threaded_exit(void) {
    pr_info("Kernel thread removed");
    kthread_stop(kernel_thread1);
}

module_init(threaded_module);
module_exit(threaded_exit);