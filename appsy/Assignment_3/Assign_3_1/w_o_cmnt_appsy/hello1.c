/*                                                     
 * $Id: hello.c,v 1.5 2004/10/26 03:32:21 corbet Exp $ 
 */                                                    

//kernel headers as per the requirements !!
#include <linux/init.h>
#include <linux/module.h>

extern int new_symbol ; //this will allocated in the data region of the module 
char *str1 = NULL;
int *var1 = 0x0000000a;
//int *var1 = 0x0000000c;

static int hello_init(void)
{
        new_symbol = 10;
        int local_var1 =0; //this will be allocated on the system stack 
        dump_stack();      //very useful for diagnostics and debugging !!

        //*var1 = 0xffffffff;
        printk(KERN_ALERT "Hello, system world\n");
    	printk(KERN_ALERT "Inside hello init\n");
        //printk(KERN_EMERG "var1 is %d\n", *var1); 
        //printk(KERN_ALERT "string is %s\n", str1); 

        return 0;
}

static void hello_exit(void)
{
        dump_stack();
        printk(KERN_ALERT "Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);

//EXPORT_SYMBOL_GPL(new_symbol);
//EXPORT_SYMBOL(new_symbol);

//MODULE_LICENSE("Dual BSD/GPL");
MODULE_LICENSE("GPL");
