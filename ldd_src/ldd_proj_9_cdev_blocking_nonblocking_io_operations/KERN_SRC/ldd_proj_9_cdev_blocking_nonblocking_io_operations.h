#define DRIVER_NAME "ldd_proj_9_cdev_blocking_nonblocking_io_operations"
#define PDEBUG(fmt,args...) printk(KERN_ALERT" "fmt, ##args)
#define PINFO(fmt,args...) printk(KERN_ALERT" "fmt, ##args)
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/capability.h>
#include <linux/sched.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/semaphore.h>
#include <linux/wait.h>

#include "device_ioctl.h"
