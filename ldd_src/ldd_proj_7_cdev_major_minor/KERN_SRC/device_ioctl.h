/*
 * device_ioctl.h
 * Author: farshad
 */

#ifndef DEVICE_IOCTL_H_
#define DEVICE_IOCTL_H_

#include <linux/ioctl.h>

typedef struct
{
	int device_param1;
	int device_param2;
	int device_param3;
}dev_ctl;

#define IOC_MAGIC 'q'
#define GET_DEV_CTL _IOR(IOC_MAGIC,1,dev_ctl *)
#define SET_DEV_CTL _IOW(IOC_MAGIC,2,dev_ctl *)
#define CLR_DEV_CTL _IO(IOC_MAGIC,3)

#endif /* DEVICE_IOCTL_H_ */
