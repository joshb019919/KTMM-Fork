#ifndef __TESTC_H_
#define __TESTC_H_

#include <linux/ftrace.h>
#include <linux/init.h>
#include <linux/kallsyms.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Josh Borthick");
MODULE_DESCRIPTION("Simple test kernel module.");

#define NUM_HOOKED 1

/************************************************************************************************/
/**************************************** Structures ********************************************/
/************************************************************************************************/

/**
 * struct ftrace_hook    describes the hooked function
 *
 * @name:           the name of the hooked function
 *
 * @function:       the address of the wrapper function that will be called instead of 
 *                     the hooked function
 *
 * @original:           a pointer to the place where the address 
 *                     of the hooked function should be stored, filled out during installation of 
 *             the hook
 *
 * @address:        the address of the hooked function, filled out during installation 
 *             of the hook
 *
 * @ops:                ftrace service information, initialized by zeros;
 *                      initialization is finished during installation of the hook
 */
struct ftrace_hook {
        const char *name;
        void *function;
        void *original;
 
        unsigned long address;
        struct ftrace_ops ops;
};

#define HOOK(_name, _function, _original)                    \
        {                                                    \
            .name = (_name),                                 \
            .function = (_function),                         \
            .original = (_original),                         \
        }

int fh_install_hook(struct ftrace_hook *hook);
void fh_remove_hook(struct ftrace_hook *hook);

/************************************************************************************************/
/********************************** Real Pointer and Wrapper ************************************/
/************************************************************************************************/

/*
 * It’s a pointer to the original system call handler execve().
 * It can be called from the wrapper. It’s extremely important to keep the function signature
 * without any changes: the order, types of arguments, returned value,
 * and ABI specifier (pay attention to “asmlinkage”).
 */
static asmlinkage long (*real_sys_execve)(const char __user *filename,
                const char __user *const __user *argv,
                const char __user *const __user *envp);
 
/*
 * This function will be called instead of the hooked one. Its arguments are
 * the arguments of the original function. Its return value will be passed on to
 * the calling function. This function can execute arbitrary code before, after, 
 * or instead of the original function.
 */
static asmlinkage long fh_sys_execve(const char __user *filename,
                const char __user *const __user *argv,
                const char __user *const __user *envp)
{
        long ret;
 
        pr_debug("execve() called: filename=%p argv=%p envp=%p\n",
                filename, argv, envp);
 
        ret = real_sys_execve(filename, argv, envp);
 
        pr_debug("execve() returns: %ld\n", ret);
 
        return ret;
}

/************************************************************************************************/
/************************************** Hooked Functions ****************************************/
/************************************************************************************************/
 
static struct ftrace_hook hooked_functions[] = {
        HOOK("sys_execve",  fh_sys_execve,  &real_sys_execve),
};

/************************************************************************************************/
/*************************************** Kallsyms Lookup ****************************************/
/************************************************************************************************/

static int resolve_hook_address(struct ftrace_hook *hook)
{
        hook->address = kallsyms_lookup_name(hook->name);
 
        if (!hook->address) {
                pr_debug("unresolved symbol: %s\n", hook->name);
                return -ENOENT;
        }
 
        *((unsigned long*) hook->original) = hook->address;
 
        return 0;
}

/************************************************************************************************/
/************************************** Callback Function ***************************************/
/************************************************************************************************/

static void notrace fh_ftrace_thunk(unsigned long ip, unsigned long parent_ip,
                struct ftrace_ops *ops, struct pt_regs *regs)
{
        struct ftrace_hook *hook = container_of(ops, struct ftrace_hook, ops);
         /* Skip the function calls from the current module. */
        if (!within_module(parent_ip, THIS_MODULE))
                regs->ip = (unsigned long) hook->function;
}

/************************************************************************************************/
/********************************** Install and Remove Hooks ************************************/
/************************************************************************************************/

int fh_install_hook(struct ftrace_hook *hook)
{
        int err;
 
        err = resolve_hook_address(hook);
        if (err)
                return err;
 
        hook->ops.func = (void (*)(long unsigned int,  long unsigned int,  struct ftrace_ops *, struct ftrace_regs *)) fh_ftrace_thunk;
        hook->ops.flags = FTRACE_OPS_FL_SAVE_REGS
                        | FTRACE_OPS_FL_IPMODIFY;
 
        err = ftrace_set_filter_ip(&hook->ops, hook->address, 0, 0);
        if (err) {
                pr_debug("ftrace_set_filter_ip() failed: %d\n", err);
                return err;
        }
 
        err = register_ftrace_function(&hook->ops);
        if (err) {
                pr_debug("register_ftrace_function() failed: %d\n", err);
 
                /* Don’t forget to turn off ftrace in case of an error. */
                ftrace_set_filter_ip(&hook->ops, hook->address, 1, 0);
 
                return err;
        }
 
        return 0;
}

void fh_remove_hook(struct ftrace_hook *hook)
{
        int err;
 
        err = unregister_ftrace_function(&hook->ops);
        if (err) {
                pr_debug("unregister_ftrace_function() failed: %d\n", err);
        }
 
        err = ftrace_set_filter_ip(&hook->ops, hook->address, 1, 0);
        if (err) {
                pr_debug("ftrace_set_filter_ip() failed: %d\n", err);
        }
}

#endif // __TESTC_H_