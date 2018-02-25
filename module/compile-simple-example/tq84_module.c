#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

 //
 // The following values will be shown by executing
 //    modinfo tq84_module
 //
MODULE_LICENSE("GPL");
MODULE_AUTHOR("René Nyffenegger");
MODULE_DESCRIPTION("Just another boring module.");
MODULE_VERSION("1.0");

 //
 // When the module is inserted into the kernel, it can be given
 // two optional arguments. The variables for these arguments
 // are declared here. They are also initialized with default
 // values that can be overwritten when inserting the module.
 //
 //    insmod tq84_module
 //    insmod tq84_module arg_one=ValueChanged
 //    insmod tq84_module arg_one=ValueChanged arg_two=99
 //
 //  After the module is insarted, the values of these parameters
 //  can be queried with
 //
 //    cat /sys/module/tq84_module/parameters/arg_one
 //    cat /sys/module/tq84_module/parameters/arg_two
 //
static char *arg_one="Default Value";
static int   arg_two=            42 ;

 //
 // We also need to explicitely state which variables can be used for
 // passing parameters and what the type of these variables is.
 // charp stands for char*
 //
module_param(arg_one, charp, S_IRUGO);
module_param(arg_two,   int, S_IRUGO);

 //
 // The following information will be shown by executing
 //    modinfo tq84_module
 //
MODULE_PARM_DESC(arg_one, "A string");
MODULE_PARM_DESC(arg_two, "An integer");


static int __init tq84_module_init(void) {
 //
 // A kernel module does not have a main() function. But it has a function that is marked
 // with __init (and further below again with module_init(...).
 // This function will be called when the module is inserted (with insmod).
 //

 //
 // The printk() function writes into the kernel-log.
 // The kernel log can be examined with the shell command dmesg or possibly at looking
 // at /var/log/kern.log
 //
    printk(KERN_INFO "TQ84 module, init: arg_one=%s, arg_two=%d\n",arg_one, arg_two);

 // The return value 0 signals that the initialization went ok:
    return 0;
}

static void __exit tq84_module_exit(void) {
 //
 // Similarly to the init function above, this function will be called
 // when the module is removed from the kernel with rmmod.
 //
    printk(KERN_INFO "TQ84 module: exiting\n");
}

module_init(tq84_module_init);
module_exit(tq84_module_exit);
