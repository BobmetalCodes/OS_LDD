/*                                                     
 * $Id: hello.c,v 1.5 2004/10/26 03:32:21 corbet Exp $ 
 */                                                    

//kernel headers as per the requirements !!
//#define DEBUG 
#include <linux/init.h>
#include <linux/module.h>
#include <linux/debugfs.h>
#include <linux/sched.h>

int new_symbol = 5;    //this will allocated in the data region of the module 
int new_symbol_2 = 10; //this will allocated in the data region of the module 
static  char *str1 = NULL;
static  int *var1 = 0x0000000a;
//int *var1 = 0x0000000c;

struct dentry * dbg_dir1 = NULL;
struct dentry * dbg_file1 = NULL;

int hello_init(void)
{
        int local_var1 =0,local_var2=0;

        dump_stack();                  //very useful for diagnostics and debugging !!
        //*var1 = 0xffffffff;

    	printk(KERN_ALERT "Hello, system world\n");
	    printk( "Hello, system world with default log level \n");
	    printk( "Hello, address of a local variable is %x \n", &local_var1);
    	printk( "Hello, address of a local variable is %x \n", &local_var2);
	    printk( "Hello, address of a global variable is %x \n", &new_symbol);
    	printk( "Hello, address of a global variable is %x \n", &new_symbol_2);
	    pr_debug("Hello, system world\n");
      
        //printk(KERN_EMERG "var1 is %d\n", *var1); 
        //printk(KERN_ALERT "string is %s\n", str1); 
	
        printk("kernel stack size is %lu\n", THREAD_SIZE); 
        printk("page size is %lu\n", PAGE_SIZE); 
 
        dbg_dir1 = debugfs_create_dir("hello_n2_node", NULL);
        if(dbg_dir1==NULL)
        { 
            pr_alert("error in debugfs dir creation\n"); 
        }
        if(dbg_dir1!=NULL)
        {                     
            dbg_file1 = debugfs_create_u32("new_symbol", 0600, dbg_dir1,&new_symbol);
            if(dbg_file1==NULL) { pr_alert("error in debugfs new_symbol file creat\n"); }
        } 
        return 0;
}

void hello_exit(void)
{
        dump_stack();
    	pr_debug("Goodbye, cruel world\n");
        //debugfs_remove(dbg_file1);
        //debugfs_remove(dbg_dir1);
        debugfs_remove_recursive(dbg_dir1); 
}

module_init(hello_init);
module_exit(hello_exit);

EXPORT_SYMBOL_GPL(new_symbol);
//EXPORT_SYMBOL(new_symbol);

//MODULE_LICENSE("Dual BSD/GPL");
MODULE_LICENSE("GPL");

