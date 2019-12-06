/*
===============================================================================
Driver Name		:		ldd_proj_5_chdr_goto_error_handling
Author			:		FARSHAD
License			:		GPL
Description		:		LINUX DEVICE DRIVER PROJECT
===============================================================================
*/

#include"ldd_proj_5_chdr_goto_error_handling.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("FARSHAD");

static int major=0;

ssize_t device_read (struct file *filp, char __user *buffer, size_t len, loff_t *offset);
ssize_t device_write (struct file *filp, const char __user *buffer, size_t len, loff_t *offset);
int device_open (struct inode *inp, struct file *filp);
int device_flush (struct file *filp, fl_owner_t id);
int device_release (struct inode *inp, struct file *filp);

module_param(major,int,0644);


static const struct file_operations fops= {
	.owner=		THIS_MODULE,
	.open=		device_open,
	.read=		device_read,
	.write=		device_write,
	.flush=		device_flush,
	.release=	device_release
};

static int __init device_init(void)
{
	int result=0;
	if(major<0)
	{
		result=major;
		PDBMSG("%s %s: The major number must be equal or  greater than 0. Major curr val is %d\n",DRIVER_NAME,__FUNCTION__,major);
		goto OUTPUT;
	}
	int status=register_chrdev(major,DRIVER_NAME,&fops);
	if(status<0)
	{
		result=status;
		PDBMSG("%s %s: The device initialization has failed with errno=%d\n",DRIVER_NAME,__FUNCTION__,status);
		goto OUTPUT;
	}
	major=(major==0)?status:major;
	PINFOMSG("%s %s, The device initialization done successfully with major=%d\n",DRIVER_NAME,__FUNCTION__,major);
	OUTPUT:
	return result;
}

static void __exit device_exit(void)
{	
	PINFOMSG("%s %s: The device exit operation done successfully\n",DRIVER_NAME,__FUNCTION__);
	unregister_chrdev(major,DRIVER_NAME);
}

ssize_t device_read (struct file *filp, char __user *buffer, size_t len, loff_t *offset)
{
	return 0;
}
ssize_t device_write (struct file *filp, const char __user *buffer, size_t len, loff_t *offset)
{
	return 0;
}
int device_open (struct inode *inp, struct file *filp)
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

module_init(device_init);
module_exit(device_exit);

