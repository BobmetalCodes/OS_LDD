#include<unistd.h>
#include<sys/types.h>
#include<linux/fcntl.h>
#include<stdio.h>
#include<stdlib.h>


int main()
{
   int fd,ret,buf[1024];

   fd = open("/proc/proc_test/readme",O_RDONLY);
   if(fd<0) { perror("error in opening"); exit(1); }

   //printf("value of fd is %d\n", fd); 
   //exit(0); //just for testing open()  

   /*
        lseek(fd,22,SEEK_SET);
        ret = read(fd,buf,30);
        printf("1..the no. characters returned is %d\n", ret);
        if(ret>0)
        { write(STDOUT_FILENO,buf,ret); }
   */
   /*
        ret = read(fd,buf,15);
        printf("1..the no. characters returned is %d\n", ret);
        if(ret>0) { write(STDOUT_FILENO,buf,ret); }
   */
   /*
        ret = read(fd,buf,15);
        printf("2..the no. characters returned is %d\n", ret);
        if(ret>0) { write(STDOUT_FILENO,buf,ret); } 
   */
   /*
        ret = read(fd,buf,4096);
        printf("1..the no. characters returned is %d\n", ret);
        if(ret>0) { write(STDOUT_FILENO,buf,ret); }
   */
   /*
        ret = read(fd,buf,2048);
        printf("1..the no. characters returned is %d\n", ret);
        if(ret>0) { write(STDOUT_FILENO,buf,ret); }
   */
   /*
        ret = read(fd,buf,15);
        printf("the no. characters returned is %d\n", ret);
        if(ret>0) { write(STDOUT_FILENO,buf,ret); }
   */
 
   // Remove the following 'while loop' while tracing...

   while(1)
   {

	   ret = read(fd,buf,4096);
	   if(ret<0) { perror("error in read"); exit(6); }
	   if(ret==0) ;
	   if(ret>0) { write(STDOUT_FILENO,buf,ret); } 

	   //end of file - no more data
	   //printf("the no. characters returned is %d\n", ret);

   }
  
   //ret = read(fd,buf,11);
   //ret = read(fd,buf,11);
   //ret = read(fd,buf,22);
   //printf("the no. characters returned is %d\n", ret);
   //if(ret>0) { write(STDOUT_FILENO,buf,ret); }
 
   //pause();
   exit(0);
}
