/*
===============================================================================
Driver Name		:		ldd_proj_7_cdev_major_minor
Author			:		FARSHAD
License			:		GPL
Description		:		LINUX DEVICE DRIVER PROJECT
===============================================================================
*/

#include"ldd_proj_7_cdev_major_minor.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("FARSHAD");

#define SUCCESS 0
#define FIRST_MINOR 0
#define N_MINORS 1
#define BUFFER_LEN 80

int device_param1 = 0;
int device_param2 = 0;
int device_param3 = 0;

static int major = 0;
static dev_t device_num;
typedef struct private_data
{
	struct cdev dev_cdev;
	int minor;
	int deviceOpenCount;
	char message[BUFFER_LEN];
	char *messagePointer;

}dev_private_data;
dev_private_data devices[N_MINORS];

int device_open(struct inode *inp, struct file *filp);
ssize_t device_read(struct file *filp, char __user *buffer, size_t size,loff_t *offset);
ssize_t device_write(struct file *filp, const char __user *buffer, size_t size,loff_t *offset);
long device_ioctl(struct file *filp, unsigned int ioctl_num,unsigned long ioctl_param);
int device_release(struct inode *inp, struct file *filp);

static void print_device_params(void);
static int is_user_capable(int permission);

module_param(device_param1,int,0644);
module_param(device_param2,int,0644);
module_param(device_param3,int,0644);

static const struct file_operations fops = { .owner = THIS_MODULE, .open =
		device_open, .read = device_read, .write = device_write,
		.unlocked_ioctl = device_ioctl, .release = device_release };

static int __init device_init(void) {
	int result = SUCCESS;
	int reg_status=alloc_chrdev_region(&device_num,FIRST_MINOR,N_MINORS,DRIVER_NAME);
	if(reg_status)
	{
		result = reg_status;
		PDEBUG("%s %s: The char device minor allocation has failed and the errno is %d\n",DRIVER_NAME,__FUNCTION__,reg_status);
		goto OUTPUT;
	}
	major=MAJOR(device_num);
	int i;
	for(i=0;i<N_MINORS;i++)
	{
		device_num=MKDEV(major,FIRST_MINOR+i);
		cdev_init(&devices[i].dev_cdev,&fops);
		cdev_add(&devices[i].dev_cdev,device_num,1);
		devices[i].minor=FIRST_MINOR+i;
		PDEBUG("%s %s, The device(major=%d,minor=%d) is added successfully.\n",DRIVER_NAME,__FUNCTION__,major,devices[i].minor);
	}
	OUTPUT:
	return result;
}

static void __exit device_exit(void) {
	int i;
	for(i=0;i<N_MINORS;i++)
	{
		cdev_del(&devices[i].dev_cdev);
	}
	unregister_chrdev_region(device_num,N_MINORS);
	PDEBUG("%s %s: The device exit operation done successfully\n",DRIVER_NAME,__FUNCTION__);
}

int device_open(struct inode *inp, struct file *filp) {
	dev_private_data *device_private_data;
	device_private_data=container_of(inp->i_cdev,dev_private_data,dev_cdev);
	filp->private_data=device_private_data;
	if (device_private_data->deviceOpenCount > 0) {
		return -EBUSY;
	}
	++device_private_data->deviceOpenCount;
	device_private_data->messagePointer = device_private_data->message;
	return SUCCESS;
}
ssize_t device_read(struct file *filp, char __user *buffer, size_t size,loff_t *offset) {
	dev_private_data *device_private_data=filp->private_data;
	int length = min(BUFFER_LEN- *offset,size);
	if (length <= 0) {
		return 0;
	}
	if (copy_to_user(buffer, device_private_data->messagePointer + *offset, length)) {
		return -EFAULT;
	} else {
		*offset += length;
		return length;
	}
}
ssize_t device_write(struct file *filp, const char __user *buffer, size_t size,loff_t *offset) {
	dev_private_data *device_private_data=filp->private_data;
	int length = min(BUFFER_LEN- *offset,size);
	if (length <= 0) {
		return 0;
	}
	if (copy_from_user(device_private_data->messagePointer + *offset, buffer, length)) {
		return -EFAULT;
	} else {
		*offset += length;
		return length;
	}
}
long device_ioctl(struct file *filp, unsigned int ioctl_num,unsigned long ioctl_param) {
	dev_ctl my_dev_ctl;
	switch (ioctl_num) {
	case GET_DEV_CTL: {
		my_dev_ctl.device_param1 = device_param1;
		my_dev_ctl.device_param2 = device_param2;
		my_dev_ctl.device_param3 = device_param3;

		/*It is better to use put_user fun because data size is small,but here we are using
		 copy_to_user just as an example.*/
		if (copy_to_user((dev_ctl *) ioctl_param, &my_dev_ctl, sizeof(dev_ctl))) {
			return -EACCES;
		}
		print_device_params();
		break;
	}
	case SET_DEV_CTL: {
		if (!is_user_capable(CAP_SYS_ADMIN)) {
			return -EPERM;
		}
		if (copy_from_user(&my_dev_ctl, (dev_ctl *) ioctl_param,
				sizeof(dev_ctl))) {
			return -EACCES;
		}
		device_param1 = my_dev_ctl.device_param1;
		device_param2 = my_dev_ctl.device_param2;
		device_param3 = my_dev_ctl.device_param3;
		print_device_params();
		break;
	}
	case CLR_DEV_CTL: {
		device_param1 = 0;
		device_param2 = 0;
		device_param3 = 0;
		print_device_params();
		break;
	}
	default: {
		return -EINVAL;
		break;
	}
	}
	return 0;
}
int device_release(struct inode *inp, struct file *filp) {
	dev_private_data *device_private_data=filp->private_data;
	--device_private_data->deviceOpenCount;
	filp->private_data=NULL;
	return SUCCESS;
}

static void print_device_params(void) {
	PDEBUG("%s %s: The device param1=%d\tparam2=%d\tparam3=%d \n",DRIVER_NAME,__FUNCTION__,device_param1,device_param2,device_param3);
}

static int is_user_capable(int permission) {
	return capable(permission);
}

module_init(device_init);
module_exit(device_exit);
