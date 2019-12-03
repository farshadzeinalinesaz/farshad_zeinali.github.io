/*
===============================================================================
Driver Name		 :		ldd_proj_3_chdr_init_initdata_exit
Author			 :		FARSHAD
License			 :		GPL
Description		 :		LINUX DEVICE DRIVER PROJECT
Useful References:		https://www.kernel.org/doc/htmldocs/kernel-hacking/routines-init.html
						https://www.tldp.org/LDP/lkmpg/2.4/html/x281.htm
===============================================================================
*/

#include"ldd_proj_3_chdr_init_initdata_exit.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("FARSHAD");

static __initdata int device_flag=1;

static int major;

static void __init device_function1(void);
static void device_function2(void);

int device_open (struct inode *inp, struct file *);
ssize_t device_read (struct file *, char __user *buffer, size_t len, loff_t *offset);
ssize_t device_write (struct file *filp, const char __user *buffer, size_t len, loff_t *offset);
int device_flush (struct file *filp, fl_owner_t id);
int device_release (struct inode *inp, struct file *filp);


static const struct file_operations fops={
		.owner=THIS_MODULE,
		.open=device_open,
		.read=device_read,
		.write=device_write,
		.flush=device_flush,
		.release=device_release
};

static int __init device_driver_init(void)
{
	major=register_chrdev(0,DRIVER_NAME,&fops);
	if(major<0)
	{
		printk(KERN_ALERT"%s has been invoked that the device register failed and errno is %d\n",__FUNCTION__,major);
		return major;
	}
	printk(KERN_ALERT"%s has been invoked and the flag status is %d\n",__FUNCTION__,device_flag);
	device_function1();
	device_function2();
	return 0;
}

static void __exit device_driver_exit(void)
{	
	unregister_chrdev(major,DRIVER_NAME);
	device_function2();
}

static void __init device_function1(void)
{
	printk(KERN_ALERT,"%s has been invoked\n",__FUNCTION__);
}
static void device_function2(void)
{
	printk(KERN_ALERT,"%s has been invoked\n",__FUNCTION__);
}

int device_open (struct inode *inp, struct file *filp)
{
	return 0;
}
ssize_t device_read (struct file *filp, char __user *buffer, size_t len, loff_t *offset)
{
	return 0;
}
ssize_t device_write (struct file *filp, const char __user *buffer, size_t len, loff_t *offset)
{
	return 0;
}
int device_flush (struct file *filp, fl_owner_t id)
{
	return 0;
}
int device_release (struct inode *inp, struct file *filp)
{
	return 0;
}

module_init(device_driver_init);
module_exit(device_driver_exit);

