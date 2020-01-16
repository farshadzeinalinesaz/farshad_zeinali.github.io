/*
===============================================================================
Driver Name		:		ldd_proj_10_cdev_poll_select_epoll
Author			:		FARSHAD
License			:		GPL
Description		:		LINUX DEVICE DRIVER PROJECT
REFERENCES		:		https://lwn.net/Kernel/LDD3/
					https://www.youtube.com/watch?v=UP6B324Qh5k&list=PLypxmOPCOkHXbJhUgjRaV2pD9MJkIArhg&index=85
===============================================================================
*/

#include"ldd_proj_10_cdev_poll_select_epoll.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("FARSHAD");

#define FIRST_MINOR 0
#define N_MINORS 1
#define BUFFER_LEN 100

static int device_param1=0,device_param2=0,device_param3=0;

static int major=0;
static dev_t device_number;
static wait_queue_head_t device_wait_queue;

static int is_data_available=FALSE;


typedef struct
{
	int minor;
	//int device_open_count;
	char buffer[BUFFER_LEN];
	char *buff_read_p,*buff_write_p;
	struct cdev dev_cdev;
	struct semaphore dev_sem;

}dev_private_data;

dev_private_data devices[N_MINORS];

int device_open(struct inode *inp, struct file *filp);
ssize_t device_read(struct file *filp, char __user *buffer, size_t size,loff_t *offset);
ssize_t device_write(struct file *filp, const char __user *buffer, size_t size,loff_t *offset);
__poll_t device_poll (struct file *filp, struct poll_table_struct *ptablep);
long device_ioctl(struct file *filp, unsigned int ioctl_num,unsigned long ioctl_param);
int device_release(struct inode *inp, struct file *filp);

static void print_device_params(void);
static int is_user_capable(int permission);


module_param(device_param1,int,0644);
module_param(device_param2,int,0644);
module_param(device_param3,int,0644);

static const struct file_operations fops = {
		.owner 			=THIS_MODULE,
		.open 			=device_open,
		.read 			=device_read,
		.write 			=device_write,
		.poll			=device_poll,
		.unlocked_ioctl =device_ioctl,
		.release 		=device_release
};


static int __init device_init(void)
{
	int result=SUCCESS;
	int reg_status=alloc_chrdev_region(&device_number,FIRST_MINOR,N_MINORS,DRIVER_NAME);
	if(reg_status)
	{
		result=reg_status;
		PDEBUG("%s %s: The char device minor allocation has failed and the errno is %d\n",DRIVER_NAME,__FUNCTION__,reg_status);
		goto OUTPUT;
	}
	major=MAJOR(device_number);
	int i;
	for(i=0;i<N_MINORS;i++)
	{
		devices[i].minor=FIRST_MINOR+i;
		device_number=MKDEV(major,devices[i].minor);
		cdev_init(&devices[i].dev_cdev,&fops);
		cdev_add(&devices[i].dev_cdev,device_number,1);
		sema_init(&devices[i].dev_sem, 1);//val=1 mean unlock(up) and val=0 mean lock(down)
		PDEBUG("%s %s, The device(major=%d,minor=%d) is added successfully.\n",DRIVER_NAME,__FUNCTION__,major,devices[i].minor);
	}
	init_waitqueue_head(&device_wait_queue);
	OUTPUT:
	return result;
}

static void __exit device_exit(void)
{
	int i;
	for(i=0;i<N_MINORS;i++)
	{
		cdev_del(&devices[i].dev_cdev);
	}
	unregister_chrdev_region(MKDEV(major,FIRST_MINOR),N_MINORS);
	PDEBUG("%s %s: The device exit operation done successfully\n",DRIVER_NAME,__FUNCTION__);
}

int device_open(struct inode *inp, struct file *filp)
{
	dev_private_data *private_data;
	private_data = container_of(inp->i_cdev,dev_private_data,dev_cdev);
	filp->private_data=private_data;
	/*if (private_data->device_open_count > 0) {
		 return -EBUSY;
		 }*/
	//++private_data->device_open_count;
	return SUCCESS;
}
ssize_t device_read(struct file *filp, char __user *buffer, size_t size,loff_t *offset)
{
	dev_private_data *private_data=filp->private_data;
	int length=min(size,(size_t)(private_data->buff_write_p-private_data->buff_read_p));
	if(length<0)
	{
		//O_NONBLOCK==O_NDELAY
		if(filp->f_flags & O_NONBLOCK)
		{
			return -EAGAIN;
		}
		if(is_data_available)
		{
			is_data_available=FALSE;
			return 0;
		}
		//return non-zero val if the sleep process(current process) interrupt by some signals
		if(wait_event_interruptible(device_wait_queue,is_data_available))
		{
			return -ERESTARTSYS;
		}
		length=min(size,(size_t)(private_data->buff_write_p-private_data->buff_read_p));
	}
	if(copy_to_user(buffer,private_data->buff_read_p,length))
	{
		return -EFAULT;
	}
	else
	{
		*offset+=length;
		private_data->buff_read_p+=length;
	}
	return length;
}
ssize_t device_write(struct file *filp, const char __user *buffer, size_t size,loff_t *offset)
{
	int ret_val;
	dev_private_data *private_data=filp->private_data;
	if(!private_data->buff_write_p)
	{
		private_data->buff_read_p=private_data->buffer;
		private_data->buff_write_p=private_data->buffer;
	}
	//if the return value of down_interruptible() is non-zero it means that it was interrupted.
	if(down_interruptible(&private_data->dev_sem))
	{
		return -ERESTARTSYS;
	}
	int length=min(size,(size_t)(private_data->buff_write_p-private_data->buff_read_p));
	if(length<=0)
	{
		ret_val=0;
		goto OUTPUT;
	}
	if(copy_from_user(private_data->buff_write_p,buffer,length))
	{
		ret_val=-EFAULT;
		goto OUTPUT;
	}
	else
	{
		*offset+=length;
		private_data->buff_write_p+=length;
		ret_val=length;
	}
	OUTPUT:
	//cleanup semaphore
	up(&private_data->dev_sem);
	is_data_available=TRUE;
	//Note: We must not call this macro when we are inside semaphore(I mean call it before up semaphore)
	wake_up_interruptible(&device_wait_queue);
	return ret_val;
}
__poll_t device_poll (struct file *filp, struct poll_table_struct *ptablep)
{
	int mask=0;
	dev_private_data *private_data=filp->private_data;
	down(&private_data->dev_sem);
	poll_wait(filp,&device_wait_queue,ptablep);
	if(private_data->buff_read_p!=private_data->buff_write_p)
	{
		mask|= POLLIN | POLLRDNORM;
	}
	if(private_data->buff_write_p < &private_data->buffer[BUFFER_LEN-1])
	{
		mask|= POLLOUT | POLLWRNORM;
	}
	up(&private_data->dev_sem);
	return mask;
}
long device_ioctl(struct file *filp, unsigned int ioctl_num,unsigned long ioctl_param)
{
	dev_ctl my_dev_ctl;
	switch (ioctl_num)
	{
		case GET_DEV_CTL:
		{
			my_dev_ctl.device_param1=device_param1;
			my_dev_ctl.device_param2=device_param2;
			my_dev_ctl.device_param3=device_param3;
			/*It is better to use put_user fun because data size is small,but here we are using
			copy_to_user just as an example.*/
			if(copy_to_user((dev_ctl *)ioctl_param,&my_dev_ctl,sizeof(dev_ctl)))
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
		}
	}
	return 0;
}
int device_release(struct inode *inp, struct file *filp)
{
	dev_private_data *private_data=filp->private_data;
	//--device_private_data->device_open_count;
	private_data=NULL;
	return SUCCESS;
}

static void print_device_params(void) {
	PDEBUG("%s: The device_param1=%d\tparam2=%d\tparam3=%d \n",__FUNCTION__,device_param1,device_param2,device_param3);
}
static int is_user_capable(int permission)
{
	return capable(permission);
}

module_init(device_init);
module_exit(device_exit);

