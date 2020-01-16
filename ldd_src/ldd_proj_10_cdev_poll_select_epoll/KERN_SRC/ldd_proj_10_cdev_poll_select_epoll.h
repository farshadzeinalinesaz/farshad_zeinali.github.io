#define DRIVER_NAME "ldd_proj_10_cdev_poll_select_epoll"

#define SUCCESS 1
#define FALSE 0
#define TRUE 1

#define DEVICE_DEBUG_MODE
#ifdef DEVICE_DEBUG_MODE
#define PDEBUG(fmt,args...) printk(KERN_DEBUG"%s:"fmt,DRIVER_NAME, ##args)
#else PDEBUG(fmt,args...) /*do nothing*/
#endif

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kern_levels.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/capability.h>
#include <linux/semaphore.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/poll.h>

#include "device_ioctl.h"
