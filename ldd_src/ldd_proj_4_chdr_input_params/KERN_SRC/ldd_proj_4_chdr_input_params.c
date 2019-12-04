/*
===============================================================================
Driver Name		:		ldd_proj_4_chdr_input_params
Author			:		FARSHAD
License			:		GPL
Description		:		LINUX DEVICE DRIVER PROJECT
===============================================================================
*/

#include"ldd_proj_4_chdr_input_params.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("FARSHAD");

static int major=0;
static char *usr_name="farshad";
static int usr_params_len=2;
static int usr_params[2]={0,0};

ssize_t device_read (struct file *filp, char __user *buffer, size_t len, loff_t *offset);
ssize_t device_write (struct file *filp, const char __user *buffer, size_t len, loff_t *offset);
int device_open (struct inode *inp, struct file *filp);
int device_flush (struct file *filp, fl_owner_t id);
int device_release (struct inode *inp, struct file *filp);


//the first param is variable name
//the second param is type of variable(bool,invbool,charp,int,uint,long,ulong,short,ushort)
//the third param is permission.(0=no permission,1=execute,2=write,4=read)
//If the permission is zero, means that no body cannot change it later and is following linux permission rule
//the permission 0644 means that the module owner can read and write on this param, but groups and other people can just read it
module_param(major,int,0644);//module_param(major,int,S_IRWXU);
module_param(usr_name,charp,0644);
module_param_array(usr_params,int,&usr_params_len,0644);//module_param_array(usr_params,char,NULL,0644);


static const struct file_operations fops= {
	.owner		= THIS_MODULE,
	.open=		device_open,
	.read=		device_read,
	.write=		device_write,
	.flush=		device_flush,
	.release=	device_release
};

static int __init device_init(void)
{
	printk(KERN_ALERT"%s: %s has been invoked.\n",DRIVER_NAME,__FUNCTION__);
	printk(KERN_ALERT"-------------------------------------\n");
	printk(KERN_ALERT"%s: user passed major number as %d\n",DRIVER_NAME,major);
	printk(KERN_ALERT"%s: user passed usr_name as %s\n",DRIVER_NAME,usr_name);
	//int usr_params_len=sizeof(usr_params)/sizeof(usr_params[0]);
	int i;
	for(i=0;i<usr_params_len;i++)
	{
		printk(KERN_ALERT"%s: user passed usr_params[%d]=%d\n",DRIVER_NAME,i,usr_params[i]);
	}
	printk(KERN_ALERT"-------------------------------------\n");
	major=register_chrdev(0,DRIVER_NAME,&fops);
	if(major<0)
	{
		printk(KERN_ALERT"%s: %s failed with the errno %d.\n",DRIVER_NAME,__FUNCTION__,major);
		return major;
	}
	printk(KERN_ALERT"%s: %s done successfully with the major number %d.\n",DRIVER_NAME,__FUNCTION__,major);
	return 0;
}

static void __exit device_exit(void)
{	
	printk(KERN_ALERT"%s: %s has been invoked.\n",DRIVER_NAME,__FUNCTION__);
	unregister_chrdev(major,DRIVER_NAME);
}


ssize_t device_read (struct file *filp, char __user *buffer, size_t len, loff_t *offset)
{
	printk(KERN_ALERT"%s: %s has been invoked.\n",DRIVER_NAME,__FUNCTION__);
	return 0;
}
ssize_t device_write (struct file *filp, const char __user *buffer, size_t len, loff_t *offset)
{
	printk(KERN_ALERT"%s: %s has been invoked.\n",DRIVER_NAME,__FUNCTION__);
	return 0;
}
int device_open (struct inode *inp, struct file *filp)
{
	printk(KERN_ALERT"%s: %s has been invoked.\n",DRIVER_NAME,__FUNCTION__);
	return 0;
}
int device_flush (struct file *filp, fl_owner_t id)
{
	printk(KERN_ALERT"%s: %s has been invoked.\n",DRIVER_NAME,__FUNCTION__);
	return 0;
}
int device_release (struct inode *inp, struct file *filp)
{
	printk(KERN_ALERT"%s: %s has been invoked.\n",DRIVER_NAME,__FUNCTION__);
	return 0;
}

module_init(device_init);
module_exit(device_exit);

