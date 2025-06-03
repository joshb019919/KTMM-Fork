#include "testc.h"

static int __init start(void) {
    pr_info("Hello from the kernel!\n");
    for (int i = 0; i < NUM_HOOKED; ++i) {
        fh_install_hook(&hooked_functions[i]);
    }
    return 0;
}

static void __exit stop(void) {
    pr_info("Goodbye from the kernel!\n");
}

module_init(start);
module_exit(stop);