/*
===============================================================================
Driver Name			:		ldd_proj_6_chdr_fops_1
Author				:		FARSHAD
License				:		GPL
Description			:		LINUX DEVICE DRIVER PROJECT
Useful References	:		http://www.tldp.org/LDP/lkmpg/2.4/html/x856.html
							https://opensourceforu.com/2011/08/io-control-in-linux/
===============================================================================
*/

#include"ldd_proj_6_chdr_fops_1.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("FARSHAD");

#define SUCCESS 0
#define BUFFER_LEN 100

static int major=0;
static int deviceOpenCount=0;
static char message[BUFFER_LEN];
static char *messagePointer;

int device_param1=0;
int device_param2=0;
int device_param3=0;


int device_open (struct inode *inp, struct file *filp);
ssize_t device_read (struct file *filp, char __user *buffer, size_t len, loff_t *offset);
ssize_t device_write (struct file *filp, const char __user *buffer, size_t len, loff_t *offset);
long device_ioctl (struct file *filp, unsigned int ioctl_num, unsigned long ioctl_param);
int device_release (struct inode *inp, struct file *filp);


static void print_device_params(void);
static int is_user_capable(int permission);

module_param(major,int,0644);


static const struct file_operations fops= {
	.owner=				THIS_MODULE,
	.open=				device_open,
	.read=				device_read,
	.write=				device_write,
	.unlocked_ioctl=	device_ioctl,
	.release=			device_release
};

static int __init device_init(void)
{
	int result=SUCCESS;
		if(major<0)
		{
			result=major;
			PDEBUG("%s %s: The major number must be equal or greater than 0. Major curr val is %d\n",DRIVER_NAME,__FUNCTION__,major);
			goto OUTPUT;
		}
		int status=register_chrdev(major,DRIVER_NAME,&fops);
		if(status<0)
		{
			result=status;
			PDEBUG("%s %s: The device initialization has failed with errno=%d\n",DRIVER_NAME,__FUNCTION__,status);
			goto OUTPUT;
		}
		major=(major==0)?status:major;
		PDEBUG("%s %s, The device initialization done successfully with major=%d\n",DRIVER_NAME,__FUNCTION__,major);
		OUTPUT:
		return result;
}

static void __exit device_exit(void)
{
	PDEBUG("%s %s: The device exit operation done successfully\n",DRIVER_NAME,__FUNCTION__);
	unregister_chrdev(major,DRIVER_NAME);
}

int device_open (struct inode *inp, struct file *filp)
{
	if(deviceOpenCount>0)
	{
		return -EBUSY;
	}
	++deviceOpenCount;
	messagePointer=message;
	return SUCCESS;
}
ssize_t device_read (struct file *filp, char __user *buffer, size_t len, loff_t *offset)
{
	int temp_read_count=0;
	if(*messagePointer==0)
	{
		return 0;
	}
	while(len && *messagePointer)
	{
		put_user(*(messagePointer++),buffer++);
		--len;
		++temp_read_count;
	}
	return temp_read_count;
}
ssize_t device_write (struct file *filp, const char __user *buffer, size_t len, loff_t *offset)
{
	int write_count;
	for(write_count=0;write_count<len && write_count<BUFFER_LEN;write_count++)
	{
		get_user(message[write_count],buffer+write_count);
	}
	messagePointer=message;
	return write_count;
}
long device_ioctl (struct file *filp, unsigned int ioctl_num, unsigned long ioctl_param)
{
	dev_ctl my_dev_ctl;
	switch (ioctl_num) {
		case GET_DEV_CTL:
		{
			my_dev_ctl.device_param1=device_param1;
			my_dev_ctl.device_param2=device_param2;
			my_dev_ctl.device_param3=device_param3;

			/*It is better to use put_user fun because data size is small,but here we are using
			copy_to_user just as an example.*/
			if (copy_to_user((dev_ctl *)ioctl_param, &my_dev_ctl, sizeof(dev_ctl)))
			{
			return -EACCES;
			}
			print_device_params();
			break;
		}
		case SET_DEV_CTL:
		{
			if(!is_user_capable(CAP_SYS_ADMIN))
			{
				return -EPERM;
			}
			if(copy_from_user(&my_dev_ctl,(dev_ctl *)ioctl_param,sizeof(dev_ctl)))
			{
				return -EACCES;
			}
			device_param1=my_dev_ctl.device_param1;
			device_param2=my_dev_ctl.device_param2;
			device_param3=my_dev_ctl.device_param3;
			print_device_params();
			break;
		}
		case CLR_DEV_CTL:
		{
			device_param1=0;
			device_param2=0;
			device_param3=0;
			print_device_params();
			break;
		}
		default:
		{
			return -EINVAL;
			break;
		}
	}
	return 0;
}
int device_release (struct inode *inp, struct file *filp)
{
	--deviceOpenCount;
	return SUCCESS;
}

static void print_device_params(void)
{
	PDEBUG("%s %s: The device param1=%d\tparam2=%d\tparam3=%d\n",DRIVER_NAME,__FUNCTION__,device_param1,device_param2,device_param3);
}

static int is_user_capable(int permission)
{
	return capable(permission);
}

module_init(device_init);
module_exit(device_exit);

