
###(4)add the following line to  <KSRC>/drivers/char/Makefile 
###and remove the comments
### do not delete/remove/overwrite the existing Makefile of 
### the parent directory  
     
###obj-$(CONFIG_CUSTOM_MODULE) += custom/


add the following line to <KSRC>/drivers/char/custom/Makefile

###this will be added to our project's/modules'  Makefile script, 
###this is a simple case -  a single source file only
###in this simple context, a single source file is used
#### to build the final kernel module object file 
###obj-$(CONFIG_CUSTOM_MODULE) += custom.o

###following can be used to build multiple, independent 
#### kernel modules  
obj-$(CONFIG_CUSTOM_MODULE) += hello.o hello1.o 


................................................
(or)
###this set-up is for a project, which uses multiple source files
####for a final module object file 
####in this context, custom-mod1.c will be a source file and 
####custom-mod2.c will be another source file
###final object file will custom.ko 
##obj-$(CONFIG_CUSTOM_MODULE) += custom.o

###custom-objs := custom-mod1.o custom-mod2.o 



###custom-objs := hello.o hello1.o hello_n3.o









