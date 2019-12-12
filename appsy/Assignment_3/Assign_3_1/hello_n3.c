/*                                                     
 * $Id: hello.c,v 1.5 2004/10/26 03:32:21 corbet Exp $ 
 */                                                    
//these headers are from the kernel - source tree,whose path was passed
//during make -C <ksrcpath>  M=`pwd`  modules  ??
//
//kernel headers as per the requirements !!

//#define DEBUG 
#include <linux/init.h>
#include <linux/module.h>
#include <linux/debugfs.h>
#include <linux/sched.h>


//a loadable kernel module object file contains code / data / other
//sections !!!

//when insmod is invoked to load our kernel module, insmod 
//invokes a system call API, which inturn requests memory 
//from system's physical memory manager(non-contiguous memory
// manager)  - for code / data and
//other sections of the kernel module !!!
//most of the activities related to loading a kernel module, 
//are done by this special system call API !!!

//you may declare the global variables for a module -  
//the memory is allocated using non-contiguous kernel memory allocator
//of the kernel, which interacts with core physical mem manager of 
//the kernel  and the code/data is located in kernel - space !!
 int new_symbol = 5; //this will allocated in the data region of the module 
 int new_symbol_2 = 10; //this will allocated in the data region of the module 
static  char *str1 = NULL;
static  int *var1 = 0x0000000a;
 //int *var1 = 0x0000000c;

//a special filesystem, known as debugfs - this is used for kernel debugging 
//and diagnoistics ??

struct dentry * dbg_dir1 = NULL;
struct dentry * dbg_file1 = NULL;



//a module component typically uses an init method and 
//an exit method - in addition, a module may also use
//other methods for its exported functionalities !!!
////certain methods may be registered with the system,
//but not exported (all these activities are subject to rules !!!

//init  routine  will be executed once when the module is added/loaded
//to the kernel space - during module loading
//typically, used to allocate dynamic resources and initialise
//hw / sw entities !!! some system level registrations using 
//system APIs may be done, for certain frame-works  !!! 
//this is to dock/register our module to another
//component/subsystem of the system !!!

//still, other rules of the system apply, in relevant contexts  !!!
//
//what is the context of execution of this init method ??
//process(current thread/main thread of the process) context of process that 
//executes insmod utility and in addition,
//our init method is invoked as part of a  special system call API !!
//insmod is the utility used to load a kernel module, into 
//kernel space !!!
//process context(current thread of execution) defines a set of 
//flexible rules that may 
//be applicable for init() method(other module methods, if 
//applicable - there is more freedom 
//for coding inside process contexts - this allows invoking 
//most of the system APIs - can invoke blocking system APIs   !!!
int  hello_init(void)
{

        int local_var1 =0,local_var2=0;//this will be allocated on 
                          //the system stack of the current 
                          //process / thread, which invokes
                          //the system call API  

        dump_stack(); //very useful for diagnostics and debugging !!


       //*var1 = 0xffffffff;

        //here, trivial printks are used
        //ideally, some tangible coding will be added 
       

	printk(KERN_ALERT "Hello, system world\n");
	printk( "Hello, system world with default log level \n");
	printk( "Hello, address of a local variable is %x \n", &local_var1);
	printk( "Hello, address of a local variable is %x \n", &local_var2);
	printk( "Hello, address of a global variable is %x \n", &new_symbol);
	printk( "Hello, address of a global variable is %x \n", &new_symbol_2);
	pr_debug("Hello, system world\n");
      
        //printk(KERN_EMERG "var1 is %d\n", *var1); 
        //printk(KERN_ALERT "string is %s\n", str1); 
	
        //when our init() method returns to system call API, 
        //system call API returns to user-space and our 
        //module is passively resident in system space !!!
        //system uses struct module {} object to manage 
        // a loaded module - this is not a pd !!!

        printk("kernel stack size is %lu\n", THREAD_SIZE); 
        printk("page size is %lu\n", PAGE_SIZE); 
 

       dbg_dir1 = debugfs_create_dir("hello_n2_node", NULL);

       if(dbg_dir1==NULL) { pr_alert("error in debugfs dir creation\n"); }

       if(dbg_dir1!=NULL) {
                             
                            dbg_file1 = debugfs_create_u32("new_symbol", 0600, dbg_dir1,&new_symbol);
                            if(dbg_file1==NULL) { pr_alert("error in debugfs new_symbol file creat\n"); }
       } 



        return 0;  //0 means, success !!!
                   //-ve error code means failure !!!
                   //if there is a failure notification, 
                   //module will not be loaded and resources
                   //will be freed !!!
}

//rmmod is the utility used to unload a module - there are
//other utilities - we will see them in appropriate context !!!
//this routine is called only once when the module is unloaded 
//
//
//used to free resources allocated during init or other methods
//and may also be used to shutdown hw controllers and destroy
//lists/objects - may undo registrations with kernel subsystems !!!

//undo whatever was done during init() method !!!
void  hello_exit(void)
{
        //any local variable will allocated on the kernel stack 

        //this may be used to free some of the dynamically allocated
        //resources during the init() function above

        dump_stack();

	pr_debug("Goodbye, cruel world\n");

        //debugfs_remove(dbg_file1);
        //debugfs_remove(dbg_dir1);
        debugfs_remove_recursive(dbg_dir1); 

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









