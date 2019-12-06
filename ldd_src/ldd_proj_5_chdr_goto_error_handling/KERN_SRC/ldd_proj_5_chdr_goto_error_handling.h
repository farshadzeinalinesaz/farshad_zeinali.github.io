
#define DRIVER_NAME "ldd_proj_5_chdr_goto_error_handling"
#define PDBMSG(fmt,args...) printk(KERN_ALERT fmt,##args)
#define PINFOMSG(fmt,args...) printk(KERN_INFO fmt,##args)

#include<linux/fs.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
