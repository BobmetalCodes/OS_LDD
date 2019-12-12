#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<malloc.h>
#include<unistd.h>
#include<string.h>
#include<semaphore.h>
#include<unistd.h>
#include<sys/types.h>
#include<linux/fcntl.h>

pthread_t t1,t2,t3;

void *th1(void *arg)
{
	int fd,ret,buf[1024],ret1;
	//char buf1[10]="45";
	printf("Inside function of thread_1\n");
	fd = open("/sys/kernel/kobject_example/foo",O_RDWR);
	//fd = open("/proc/readme",O_RDONLY);
	if(fd<0)
	{
           perror("error in opening");
           exit(1);
   	}
	else
	{
		printf("file (foo) open successfully (value of fd is %d)\n", fd);
	}
	while(1)
	{
		
		ret1 = write(fd,"AKSHAY",6);
		lseek(fd,0,SEEK_SET);
		//ret = read(fd,buf,4096);
                if(ret1<0)
                {
                        perror("error in write");
                        exit(6);
                }

                printf("the no. characters write  (file foo)ret1 =%d\n\n\n", ret1);
                if(ret1>0)
                {
			printf("write sucessfully ret1 =%d\n",ret1);
                         ret = read(fd,buf,4096);
			write(STDOUT_FILENO,buf,ret);
                         //ssize_t write(int fd, const void *buf, size_t count);
                }
                if(ret1==6)
                        break ;  //end of file - no more data


   	}
	//while(1);
}

void *th2(void *arg)
{
        int fd,ret,buf[1024],ret1;
        printf("Inside function of thread_2\n");
        fd = open("/sys/kernel/kobject_example/baz",O_RDWR);
        //fd = open("/proc/readme",O_RDONLY);
        if(fd<0)
        {
           perror("error in opening");
           exit(1);
        }
        else
        {
                printf("file baz open successfully (value of fd is %d)\n", fd);
        }
        while(1)
        {
		ret1 = write(fd,"Swapnil",7);
		lseek(fd,0,SEEK_SET);
                //ret = read(fd,buf,4096);
                if(ret1<0)
                {
                        perror("error in write");
                        exit(6);
                }

                printf("the no. characters write  (file baz)ret1 =%d\n\n\n", ret1);
                if(ret1>0)
                {
                        printf("write sucessfully ret1 =%d\n",ret1);
                         ret = read(fd,buf,4096);
                        write(STDOUT_FILENO,buf,ret);
                         //ssize_t write(int fd, const void *buf, size_t count);
                }
                if(ret1==7)
                        break ;  //end of file - no more data
        }

       // while(1);
}

void *th3(void *arg)
{
        int fd,ret,buf[1024],ret1;
        printf("Inside function of thread_3\n");
        fd = open("/sys/kernel/kobject_example/bar",O_RDWR);
        //fd = open("/proc/readme",O_RDONLY);
        if(fd<0)
        {
           perror("error in opening");
           exit(1);
        }
        else
        {
                printf("file bar open successfully (value of fd is %d)\n", fd);
        }
        while(1)
        {
		ret1 = write(fd,"ANKIT",5);
		    lseek(fd,0,SEEK_SET);
                //ret = read(fd,buf,4096);
                if(ret1<0)
                {
                        perror("error in write");
                        exit(6);
                }

                printf("the no. characters write  (file bar)ret1 =%d\n\n\n", ret1);
                if(ret1>0)
                {
                        printf("write sucessfully ret1 =%d\n",ret1);
                         ret = read(fd,buf,4096);
                        write(STDOUT_FILENO,buf,ret);
                         //ssize_t write(int fd, const void *buf, size_t count);
                }
                if(ret1==5)
                        break ;  //end of file - no more data

        }
       // while(1);
}


int main()
{
	int t_stat = pthread_create(&t1, NULL, th1, NULL);
	if(t_stat){
		perror("Thread creation failure!");
		exit(1);
	}
	else
	{
		printf("Thread_1 created successfully\n\n");
	}
	
	t_stat = pthread_create(&t2, NULL, th2, NULL);
        if(t_stat){
                perror("Thread creation failure!");
                exit(1);
        }
        else
        {
                printf("Thread_2 created successfully\n\n");
        }

	t_stat = pthread_create(&t3, NULL, th3, NULL);
        if(t_stat){
                perror("Thread creation failure!");
                exit(1);
        }
        else
        {
                printf("Thread_3 created successfully\n\n");
        }

	void *stat;
	pthread_join(t1, stat);
	pthread_join(t2, stat);
	pthread_join(t3, stat);
}
