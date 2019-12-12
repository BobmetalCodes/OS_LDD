/*                                                     
 * $Id: hello.c,v 1.5 2004/10/26 03:32:21 corbet Exp $ 
 */                                                    

//these headers are from the kernel-source tree, whose path was passed
//during   make  -C  <ksrcpath>  M=`pwd`  modules

//kernel headers as per the requirements !!

#include <linux/init.h>
#include <linux/module.h>

//a kernel module object file contains code/data/other sections !!!
//when insmod is invoked to load our kernel module,
//insmod invokes a system call API, which inturn requests memory
//from system's physical memory manager
//for code/data and other sections of the kernel module !!!
//most of the activities related to loading a kernel module,
//are done by this special system call API !!!

//you may declare the global variables for a module
//the memory is allocated using non-contiguous memory allocator
//of the kernel, which interacts with core physical mem manager of 
//the kernel  and the code/data is located in kernel - space !!

 int new_symbol = 5; //this will allocated in the data region of the module 
 char *str1 = NULL;
 int *var1 = 0x0000000a;
 //int *var1 = 0x0000000c;

//a module component typically uses an init method and 
//an exit method - in addition, a module may also use
//other methods for its exported functionalities !!!
//certain methods may be registered with the system,
//but not exported (all these activities are subject to rules !!!

//init routine  will be executed once when the module is added
//to the kernel space-during module loading
//typically, used to allocate resources and initialise
//hw/sw entities !!! some system level registrations using 
//system APIs may be done !!! this is to dock our module to another
//component/subsystem of the system !!!
//still, other rules of the system apply, in relevant contexts  !!!

//what is the context of execution of init method ??
//process context of process that executes insmod and in addition,
//our init method is invoked as part of the special system call API !!
//insmod is the utility used to load a kernel module, into 
//kernel space !!!
//process context defines a set of flexible rules that may 
//be applicable for init() method(other module methods, if 
//applicable - there is more freedom 
//for coding inside process contexts - this allows invoking 
//most of the system APIs   !!!

static int hello_init(void)
{
	int local_var1 =0; //this will be allocated on the system stack 

	//dump_stack();    //very useful for diagnostics and debugging !!

 	// *var1 = 0xffffffff;

        //here, trivial printks are used
        //ideally, some tangible coding will be added 
	
	printk(KERN_ALERT "Hello, system world\n");
      
        //printk(KERN_EMERG "var1 is %d\n", *var1); 
        //printk(KERN_ALERT "string is %s\n", str1); 
	
        //when our init() method returns to system call API, 
        //system call API returns to user-space and our 
        //module is passively resident in system space !!!
        //system uses struct module {} object to manage 
        //a loaded module - this is not a pd !!!
        
	return 0;  //0 means, success !!!
                   //-ve error code means failure !!!
                   //if there is a failure notification, 
                   //module will not be loaded and resources
                   //will be freed !!!
}

//rmmod is the utility used to unload a module - there are
//other utilities - we will see them in appropriate context !!!
//this routine is called only once when the module is unloaded 

//used to free resources allocated during init or other methods
//and may also be used to shutdown hw controllers and destroy
//lists/objects - may undo registrations with kernel subsystems !!!

//undo whatever was done during init() method !!!

static void hello_exit(void)
{
        //any local variable will allocated on the kernel stack 

        //this may be used to free some of the dynamically allocated
        //resources during the init() function above

        dump_stack();
	printk(KERN_ALERT "Goodbye, cruel world\n");
}

//refer to page nos 31-32 LDD/3 

//module_xxx() is mandatory 
//these macros add special info. in a special section of the module's
//object file
//once this information is added to such a special section, 
//loader can track the init and exit methods - otherwise, not !!!

module_init(hello_init);
module_exit(hello_exit);

EXPORT_SYMBOL_GPL(new_symbol);
//EXPORT_SYMBOL(new_symbol);

//MODULE_LICENSE("Dual BSD/GPL");
MODULE_LICENSE("GPL");

//there are standard methods that must be provided by 
//a given module/dynamic object file and in addition, 
//we may provide additional methods, which are dependent 
//on our project !!!
