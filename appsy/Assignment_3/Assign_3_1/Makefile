### This file is written , as per the kbuild system, for external modules only 
### We may change some of the Makefile variables, if needed during run-time ???

### The following is to build 3 dynamic kernel module object files ending with *.ko, 
### hello_n3.ko, hello.ko, and hello1.ko
### In this context, the kbuild system will generate intermediate object files,
### hello_n3.o, hello.o, hello1.o 

#obj-m := hello.o hello1.o 
#obj-m := hello_n3.o

obj-m := hello.o hello1.o
#obj-m :=   kset-example.o

#ccflags-y += -DDEBUG 
ccflags-y += -O1
ccflags-y += -g
#ccflags-y += -E
