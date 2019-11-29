/*
===============================================================================
Driver Name		:		ldd_proj_2_chdr_reg_unreg_dev
Author			:		FARSHAD
License			:		GPL
Description		:		LINUX DEVICE DRIVER PROJECT
Useful resource :		https://lwn.net/Kernel/LDD3/
						https://www.fsl.cs.sunysb.edu/kernel-api/re941.html
						https://www.tldp.org/LDP/lkmpg/2.4/html/c43.html
===============================================================================
*/

#include"ldd_proj_2_chdr_reg_unreg_dev.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("FARSHAD");
MODULE_DESCRIPTION("Put device description here");
MODULE_VERSION("1");
MODULE_ALIAS("LDD PROJ 2");

int major=0;

int device_open (struct inode *inode, struct file *filp)
{
	return 0;
}
//__user means that is from user space
ssize_t device_read (struct file *filp, char __user *buffer, size_t length, loff_t *offset)
{
	return 0;
}
ssize_t device_write (struct file *filp, const char __user *buffer, size_t length, loff_t *offset)
{
	return 0;
}
int device_flush (struct file *filp, fl_owner_t id)
{
	return 0;
}
int device_release (struct inode *inode, struct file *filp)
{
	return 0;
}

static const struct file_operations fops= {
	.owner=   THIS_MODULE,
	.read=    device_read,
	.write=	  device_write,
	.flush=   device_flush,
	.release= device_release
};

static int __init ldd_proj_2_chdr_reg_unreg_dev_init(void)
{
	//status will be zero on success
	//int status=register_chrdev(239,DRIVER_NAME,&fops);

	//In this case, register_chrdev will dynamically allocate and return a major number
	//The minor number will start from 0 to range of 256 numbers
	//if fails will return a negative error number as result
	major=register_chrdev(0,DRIVER_NAME,&fops);
	if(major<0)
	{
		printk(KERN_ALERT"%s: %s has failed and returned error number is %d\n",DRIVER_NAME,__FUNCTION__,major);
		return major;
	}
	printk(KERN_ALERT"%s: %s has invoked successfully and returned major number is %d\n",DRIVER_NAME,__FUNCTION__,major);
	return 0;
}

static void __exit ldd_proj_2_chdr_reg_unreg_dev_exit(void)
{	
	unregister_chrdev(major,DRIVER_NAME);
	printk(KERN_ALERT"%s: %s has invoked successfully\n",DRIVER_NAME,__FUNCTION__);
}

module_init(ldd_proj_2_chdr_reg_unreg_dev_init);
module_exit(ldd_proj_2_chdr_reg_unreg_dev_exit);

