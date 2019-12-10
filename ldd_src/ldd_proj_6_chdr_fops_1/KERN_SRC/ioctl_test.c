#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "device_ioctl.h"


int main()
{
    int fd;
    char *file_path="/dev/ldd_proj_6_chdr_fops_1";
    fd=open(file_path,O_RDWR);
    if(fd<0)
    {
        perror("The device open process has failed:");
        return -1;
    }
    dev_ctl *my_dev_ctl;
    my_dev_ctl=malloc(sizeof(dev_ctl));
    my_dev_ctl->device_param1=10;
    my_dev_ctl->device_param2=15;
    my_dev_ctl->device_param3=20;
    if(ioctl(fd,SET_DEV_CTL,my_dev_ctl)<0)
    {
        perror("The device ioctl set process has failed:");
    }
    if(ioctl(fd,GET_DEV_CTL,my_dev_ctl)<0)
    {
        perror("The device ioctl get process has failed:");
        return -1;
    }
    printf("The device param1 is=%d\n",my_dev_ctl->device_param1);
    printf("The device param2 is=%d\n",my_dev_ctl->device_param2);
    printf("The device param3 is=%d\n",my_dev_ctl->device_param3);
    return 0;
}
