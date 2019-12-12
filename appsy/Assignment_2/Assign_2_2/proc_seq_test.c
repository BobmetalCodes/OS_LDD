#include<unistd.h>
#include<sys/types.h>
#include<linux/fcntl.h>

int main()
{
   int fd,ret,buf[1024];

   // This will setup an active file of /proc/readme, for the current process
   // This active file will be different from a regular active file of a regular file !!
   // Active file is represented, using another handle, in user-space
   // open() system call returns this handle to user-space 
   // 
   // This is handle/index to an active file is used to 
   // access the actual system data exported by the procfs file/kernel module 

   fd = open("/proc/proc_test/readme",O_RDONLY);
   if(fd<0)
   {
           perror("error in opening");
           exit(1);
   }

   //printf("value of fd is %d\n", fd); 

   //exit(0); 
   //just for testing open() 
   
   /*
   lseek(fd,22,SEEK_SET);
   ret = read(fd,buf,30);
   printf("1..the no. characters returned is %d\n", ret);
   if(ret>0)
   { 
   	write(STDOUT_FILENO,buf,ret);
   }
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

 
// ret = read(p1, p2, p3);
// p1 ---> Active file handle/index of the respective file.
//         In this case, active file handle/index of a procfs file is provided
// p2 ---> Pointer to an user-space buffer,
//         for copying data from system-space to user-space 
// p3 ---> size of p2's buffer - in this case, we are 
//         passing 4096 - We can change this reasonably  
// ret value ---> typically, ret returns a +ve value, 
//    which is the number of bytes successfully copied, 
//    as part of the read() system call.
//    ret/return value can be less than p3.
//
//    If read() returns 0, no data was copied.
//    Since there was no data in this scenario, there is no blocking, 
//    as per the rules of the frame-works  
//
//    If there is an error -1 is returned
//    we need to analyze the scenario
//    There may parameter problems/permission related issues 
//
//    read() system call API returns +ve value, 
//    if there are bytes read into the user-space buffer 
//    if we pass a buffer size value to p3, read may 
//    copy lesser number of bytes and return a 
//    different size value, in ret
//    we must check this value, before further processing
//
// In addition, you may need to check for 
// return value of 0, if there are no bytes
// read() by the read() system call API.
// This normally happens, if there is no 
// more data in the active file, which can 
// be a normal file or a procfs file 
//
// we may see a return value of -1, if there
// is error, in access of the active - in the 
// case of invalid fd or invalid access permissions 

   //while(1)
   //{

     // we are reading certain number of bytes from a procfs file 
     // finally, the no.of bytes read depends on the data retrieved from system objects
     // of system list of our custom module.
     // param1 represents handle to active file of our module's procfs file 
   
	   ret = read(fd,buf,4096);
	   if(ret<0) { perror("error in read"); exit(6); }
	   if(ret==0) break ;  //end of file - no more data
	   if(ret>0) { write(STDOUT_FILENO,buf,ret); } 
   
	//printf("the no. characters returned is %d\n", ret);
    
     	// In this case, we are not using printf.
     	// Rather, we are using handle to active file
     	// of output of current terminal of this session 
     	//
     	// First parameter is the handle to output 
     	// of current terminal's active file.
     	// this handle is for writing to the terminal   

   //}
  
   //ret = read(fd,buf,11);
   //ret = read(fd,buf,11);
   //ret = read(fd,buf,22);
   
   //printf("the no. characters returned is %d\n", ret);

   //if(ret>0) { write(STDOUT_FILENO,buf,ret); }

   //pause();
   exit(0);
}
