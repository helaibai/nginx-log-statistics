/* hello.c */

#include <linux/module.h>       /* Needed by all modules */

#include <linux/config.h>       /* Needed for KERN_ALERT */

#include <linux/init.h>         /* Needed for the module-macros */

 
static int __init hello_init(void)    // Module entry function specified by module_init()

{

        printk(KERN_ALERT "Hello,world!/n");

        return 0;

}

 
static void __exit hello_exit(void)  //Module exit function specified by module_exit()

{

        printk(KERN_ALERT "Goodbye,cruel world!/n");

}

 
module_init(hello_init);

module_exit(hello_exit);

 
MODULE_LICENSE("Dual BSD/GPL");  //should always exist or you’ll get a warning

MODULE_AUTHOR("BENSON"); //optional

MODULE_DESCRIPTION("STUDY_MODULE"); //optional

