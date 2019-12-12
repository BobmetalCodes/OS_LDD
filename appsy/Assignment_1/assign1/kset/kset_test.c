#include<unistd.h>
#include<sys/types.h>
#include<linux/fcntl.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/stat.h>

int main()
{
/*------------------------------------OPEN----------------------------------------------*/

   int fd, fd1, ret, ret1, val=22;
   char cval = 'a';
   char buf[1024];

   fd = open("/sys/kernel/kset_devices_typeA/device0/dev_param1",O_RDWR);
       if(fd<0) { perror("error in opening"); exit(1);  }
   printf("value of fd is %d\n", fd); 

 /*  fd1 = open("/sys/kernel/kobject_example/bar",O_RDWR);
       if(fd1<0) { perror("error in opening"); exit(1);  }
   printf("value of fd1 is %d\n", fd1);      */
/*------------------------------------READ----------------------------------------------*/

	ret = read(fd,buf,4096);
    	if(ret<0) { perror("error in read"); exit(6); }
	printf("READ 1 executed. Ret value = %d\n", ret);
    if(ret>0) { write(STDOUT_FILENO,buf,ret); } 

	/*ret1 = read(fd1,buf,4096);
    	if(ret1<0) { perror("error in read"); exit(6); }
	printf("READ 1 executed. Ret value = %d\n", ret1);
    if(ret1>0) { write(STDOUT_FILENO,buf,ret1); } 
    */

/*------------------------------------WRITE----------------------------------------------*/

    ret = sprintf(buf, "%d", val);
    ret = write(fd, buf, ret);
        if(ret<0){ perror("error in write");  exit(6); }
	printf("WRITE 1 executed. Ret value = %d\n", ret);

/*
    ret1 = sprintf(buf, "%c", cval);
    ret1 = write(fd1, buf, ret1);
        if(ret1<0){ perror("error in write");  exit(6); }
	printf("WRITE 1 executed. Ret value = %d\n", ret1);
    */
/*-------------------------------------READ2----------------------------------------------*/
 
    lseek(fd,0,SEEK_SET);
 //   lseek(fd1,0,SEEK_SET);

	ret = read(fd,buf,4096);
    	if(ret<0) { perror("error in read"); exit(6); }
	printf("READ 2 executed. Ret value = %d\n", ret);
    if(ret>0) { write(STDOUT_FILENO,buf,ret); } 

/*	ret1 = read(fd1,buf,4096);
    	if(ret1<0) { perror("error in read"); exit(6); }
	printf("READ 2 executed. Ret value = %d\n", ret1);
    if(ret1>0) { write(STDOUT_FILENO,buf,ret1); } 
*/
    exit(0);
}
