
#define DRIVER_NAME "ldd_proj_6_chdr_fops_1"
#define PDEBUG(fmt,args...) printk(KERN_ALERT" "fmt, ##args)
#define PINFO(fmt,args...) printk(KERN_ALERT" "fmt, ##args)
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/uaccess.h>
#include <linux/capability.h>
#include <linux/sched.h>

#include "device_ioctl.h"
