#ifndef __TESTHOOK_C_
#define __TESTHOOK_C_

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

#endif // __TESTHOOK_C