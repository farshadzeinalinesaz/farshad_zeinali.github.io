#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include<poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFF_LEN 100

void read_from_user()
{
    char message_buff[BUFF_LEN];
    int fd=open("/dev/ldd10",O_RDWR);
    printf("The fd=%d\n",fd);
    int ret, poll_ret;
    int timeout;

    struct pollfd read_pollfd;
    timeout=5000;

    while(1)
    {
        memset(message_buff,0,BUFF_LEN);
        read_pollfd.fd=fd;
        read_pollfd.events|=POLLIN;
        read_pollfd.revents=0;

        poll_ret=poll(&read_pollfd,1,timeout);
        printf("The poll_ret=%d\n",poll_ret);
        if(poll_ret==0)
        {
            printf("\nTimeout reached!\n");
        }
        else
        {
            ret=read(fd,message_buff,BUFF_LEN);
            if(ret<0)
            {
                perror("read has an error:");
                return ret;
            }


            printf("The user entered: %s\n",message_buff);
        }


    }
}



int main()
{
    printf("Hello %s\n","Farshad, START!!");
    read_from_user();
    return 0;
}



