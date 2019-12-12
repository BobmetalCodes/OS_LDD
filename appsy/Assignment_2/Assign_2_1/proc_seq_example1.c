//include appropriate kernel header files

#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/proc_fs.h>
#include<linux/slab.h>
#include<linux/seq_file.h>

// Every kernel module object file creates a struct module{} in the system space, when loaded
// This struct module{} is used to manage the loaded kernel module object file, in the system !!! 

// refer to ch11 of LDD/3 for the data-structures and related macros, used in this example !!!
// refer to chapter 3 of ULK/3.
//
// intrestingly, similar setup is used for pds, in process management !!!

/* Private Data structure */
struct _mydrv_struct 
{
   /* ... */

   /* Link to the next node */ 
   // This is fixed for any type of struct
   struct list_head list;
                           
   /* Info to pass via the procfs file */ 
   // These elements differ based on your module/driver
   char info[13];
   char info1[10];  

   /* ... */
};

//refer to ch11 of LDD/3 for LIST_HEAD
/* List Head */
static LIST_HEAD(mydrv_list);
static struct proc_dir_entry *entry = NULL ; 
static struct proc_dir_entry *entry1 = NULL ; 

/* start() method */

// Whatever index is requested, we send back the pointer of that object.
// *pos gives the index of the requested element in your list.
// Typically, it is 0 when navigation is started by a read() (first read).
// During run-time, *pos tells us where are we in the list,
// access specific object's index in the list - the specific node in the list  
//
// parameter1 is the object created for this active file in the 
// sequence file layer.
// It may be used in our operations' code 
//
// parameter2 is pointer to the offset field in the open file object of this active file.
// However, in this context it does not represent file logical byte number,
// Instead, it represents index of the system object,
// in the system's data structure that is navigated, when 
// this procfs file is accessed using an active file !!!
// In fact, when an active file of this procfs file is accessed/navigated/read, 
// data from system objects maintained in the corresponding system,
// data structure is retrieved and passed to user-space !!!
//
// System passes a *pos (offset/index of an object),
// as per the current state of navigation/access !!!
// To start with the value will be 0 as one or more objects are navigated/accessed, this value will be incremented
// we must interpret this value, validate the value and take appropriate actions, as per rules !!!
//
// as per rules, we must do the following, in xxx_seq_start() :
// - using the *pos(index value), navigate related system data-structure
//   and extract ptr to corresponding object 
// - return ptr to the extracted object, if there is such object for the provided index !!!
// - otherwise return NULL 
//
// All we need to code is the above rules and implement the same !!!

static void *mydrv_seq_start(struct seq_file *seq, loff_t *pos)
{
  struct _mydrv_struct *p;
  loff_t off = 0;

  /* The iterator at the requested offset */ 
  dump_stack(); 

  //system macro used to navigate our private ds !!!
  list_for_each_entry(p, &mydrv_list, list) 
  {
    if (*pos == off++) 
    {
	    printk("in start : success %d\n",*pos);
            return p;
        //if my position is reached return the ptr 
    }
  }
  printk("In seq_start : over\n");
  return NULL;
}

/* next() method */

// Whenever seq_read() needs more data from more objects,
// it will invoke xxx_seq_next ???
// -> Advance to the next position and return the pointer to that object
//
// As per the rules, we must do the following in this method :
//
// - Using ptr (known as iterator by the kernel document) to the current object,
//   get the ptr to the next object in the system data-structure !!!
// - In addition, increment the offset (index) using *pos !!!
// - Eventually, return ptr to the next object in the data structure
// - However, if we have navigated the list completely, return NULL !!!

static void *mydrv_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
  // Here, our real objective is to increment the *pos 
  // and return ptr to the next object based on what is 
  // passed to us by system.
  // We still must check if the next element is the first element
  // and if so, we have reached end of list and we must return NULL  

  dump_stack();

  /*
   * 'v' is a pointer to the iterator returned by start() or
   *  by the previous invocation of next() 
  */

  struct list_head *n = ((struct _mydrv_struct *)v)->list.next;
  ++*pos;
  /* Advance position */
  /* Return the next iterator, which is the next node in the list */
  
  printk("in seq_next :%d\n",*pos);
  return(n != &mydrv_list) ?
          list_entry(n, struct _mydrv_struct, list) : NULL;
}

/* show() method */

// Invariably, show() method is invoked just after start() method or after next() method !!!
// In addition, show() method is passed ptr to the object that was returned by start()/next() to seq_read() !!!
// In addition, we must copy data from the object pointed to by 
// 'v' into the kernel buffer maintained in struct seq_file object pointed to by seq.
// There are helper functions to do this.
// Do not do by hand. If you do so, it will corrupt any existing data in the buffer !!!
// These actions are taken as per rules. 
// If you need more information, refer to document that contains the rules !!!

static int mydrv_seq_show(struct seq_file *seq, void *v)
{
   int ret;
   struct _mydrv_struct  *p = v;

   dump_stack();

   /* Interpret the iterator, 'v' */
   printk("in seq_show \n");
   
   // what ever information that you wish to generate on the fly,
   // it must be generated and passed as below
   //
   // using seq_printf() we can pass info. to user space, via
   // sequence file layer's kernel buffer !!!
   // first data is copied into kernel buffer from the object of a subsystem or component !!
   //
   // Kernel buffer is used with seq_printf(provided by sequence file layer)
   // we are copying data from a current object of navigation into kernel buffer of *seq !!!

   // ret = seq_printf(seq,"%s\n",p->info);
   seq_printf(seq,"%s\n",p->info);
   //printk(KERN_INFO "the return value of seq_printf is %d\n", ret); 

   return 0;
}

// typically invoked by seq_read(), when required data is copied or
// no more data is present for copying !!!
//
// this is suppposed to be used to free any resources allocated
// during start method !!!


/* stop() method */
static void mydrv_seq_stop(struct seq_file *seq, void *v)
{
   //stop() is normally used for freeing any resource allocated during start() !!!
   /* No cleanup needed in this example */
   dump_stack();
   printk("in seq_stop:\n");
}

// if you have understood the rules for coding, in the below methods,
// you can code and provide a set of methods to the sequence file layer,
// which inturn serves the procfs layer 
//
// here, procfs, sequence file layer and our module are connected-
// these methods are provided by our module to copy data from 
// appropriate objects of system data structure to sequence file
// layer's buffer maintained in seq_file { } object for our active file !!!
//
// From the sequence file layer, data is copied to users-space buffer !!!
//
// Whenever, our procfs file is accessed, associated data structure
// is navigated and information extracted from objects maintained
// in the data structure  are copied to users-space buffer.
// However, the whole frame-work works, if you have written these methods as per 
// rules and validated their working !!!

/* Define iterator operations */
static struct seq_operations mydrv_seq_ops = {
   .start  = mydrv_seq_start,
   .next   = mydrv_seq_next,
   .stop   = mydrv_seq_stop,
   .show   = mydrv_seq_show,
};

// This method is invoked, when active file is created, when 
// our pseudofs file is opened using open(" ", ) system call API !!!

static int mydrv_seq_open(struct inode *inode, struct file *file)
{
   // Printk writes the message in the kernel's log-buffer 
   // Using dmesg, you can read from the kernel's log-buffer 

   printk("we are in mydrv_seq_open\n");   //1
   
   /* Register the operations */ 
   // used for diagnostic messages
   dump_stack();

   // With the help of dump_stack(), we understand that our
   // open method is invoked by procfs layer, which is connected
   // to logical file system manager 

   // further, our open method also registers another table
   // of methods/operations using seq_open() !!! this system API
   // enables us to register another set of operations with 
   // sequence file layer of the system !!! this must be done 
   // as per the rules of pseudo files of procfs !!!
  
   // as per the rules and what is available from 
   // <ksrc>/include/linux/seq_file.h & <ksrc>/fs/seq_file.c(is the
   // sequence file layer)
   // 
   // following is done during seq_open()
   //
   // internally, struct seq_file {} object is created and 
   // filled with another operations table ptr and in addition, 
   // the new struct seq_file {} object is linked to active file
   // object of the current active file(via open file object 
   // of this active file - private_data field of the open file object 
   // of this active file is used to store the ptr to
   // struct seq_file { } object instance ) !!!this is all done with 
   // respect to sequence file layer, not procfs layer - however,
   // procfs and sequence file layer are connected !!!
   // we are registering our table with sequence file layer - 
   // as part of it, certain objects are connected and may be
   // used during run-time !!!

   return seq_open(file, &mydrv_seq_ops);
}

// For the current context, a file_operations object is created
// and initialized as per rules of the system - these rules are
// documented under <ksrc>/Documentation/filesystems/seq_file.txt 

static struct file_operations mydrv_proc_fops = {
   // This macro will provide the ptr to our module object
   .owner    = THIS_MODULE,
   /* User supplied */
   // passing addresses of functions to function pointers
   .open     = mydrv_seq_open,
   .read     = seq_read,
   // Built-in helper functions taken from  fs/seq_file.c
   // fs/seq_file.c  provides sequence file layer.
   // Sequence file layer provides support to procfs layer.
   .llseek   = seq_lseek,
   .release  = seq_release,       
};

// Assuming this module is compiled and loaded, what will be the consequences ??
// what are the actions taken by the system ??

// A new proc file will be created and visible from the
// procfs to the user-space !!! this procfs pseudo file 
// provides interface to our module, in this context.
// when this procfs file is accessed, several call backs 
// of the kernel space are invoked, including our modules call backs ???

// what next ???

// we need to access the procfs file, using file related system 
// call APIs !!! in this context, we may use open(), read(), ioctl(),
// and close() system call APIs 

// assuming that we are opening a procfs file, using open -
// fd = open("/proc/readme", O_RDONLY) - what will be the
// actions taken by the system ???

// in this context , the first system call API will be 
// open() system call API, which will be the first 
// system call API invoked, by an application code or 
// an utility ?? what happens, when this open system call API is invoked
// refer to the call backs and descriptions above ??

// what next ??

// let us understand the set-up of a procfs file, based on the 
// code of the init method 

static int __init mydrv_init(void)
{
   /* ... */

   int i;
   struct _mydrv_struct *mydrv_new;

   dump_stack();         // used more for diagnostics and debugging 

   /* ... */
   /* Create /proc/readme */

   // this will create a new proc file with following attributes !!!
   // name 
   // permissions
   // ptr to the parent directory's object (proc_dir_entry{})
   // last parameter may be NULL, if our file is to be created
   // under /proc directory as the parent directory - otherwise,
   // it must not be NULL !! 
   // return value provides pointer to struct proc_dir_entry{} of the
   // new proc file that has been created !! a newly created procfile 
   // object is added to a list under the object of the corresponding 
   // parent directory !!!
   // refer to include/linux/proc_fs.h 

   // refer to fs/proc/generic.c

   // the following system API will set-up 
   // /proc/proc_test/
   // in the below system API, p1 is the procfs directory 
   // name and p2 will be the /proc, since we pass NULL
   // in this context, NULL is the default value for p2
   // if proc_mkdir() is succcessful, it will return 
   // pointer to struct proc_dir_entry{} system object of the 
   // newly created procfs directory 
   // it will be used, as handle to this new procfs dir
   // if we pass NULL, /proc is used , as parent directory 
   // if we pass non-NULL, another parent directory, 
   // in procfs will be used 

   entry1 = proc_mkdir("proc_test",  NULL);//a file is created 

   // the following line will set-up 
   // /proc/proc_test/readme
   // p1 is the procfs file name and p3 is the parent directory's handle
   // p2 defines the access permissions to different users, including 
   // owner of this file - in this case, the owner will be
   // root/uid=0
   // S_IRUSR means RO access to the owner/user of this procfs file 
   
   //entry = create_proc_entry("readme", S_IRUSR, entry1);//a file is created 
  entry = proc_create("readme", S_IRUSR, entry1, &mydrv_proc_fops);//a file is created 
   // we cannot use create_proc_entry(), in the 
   // newer kernel versions - so, we will be 
   // using proc_create() ??
   // try to find the parameters ??
   // set the parameters correctly and test ??
   //
   // check  <ksrctreepath>/include/linux/proc_fs.h, for 
   // declaration of proc_create() system API  
   //
   // check  <ksrctreepath>/include/linux/proc_fs.h 
   // in the below system API, p1 is the procfs file name
   // and p3 is the parent directory's ptr to struct proc_dir_entry{}  
   // entry = create_proc_entry("readme", S_IRUSR, NULL);//a file is created 

   /* Attach it to readme_proc() */

   // check for error - NULL 
   if (!entry) 
   {
 	  return -EINVAL; //init method has failed !!!
   } 

    //{

    /*
      Replace the assignment to entry->read_proc in proc_example1.c
      with a more fundamental assignment to entry->proc_fops. So
      instead of doing "entry->read_proc = readme_proc;", do the following:
    */

    // we are replacing an entry in the proc_dir_entry to our convenience
    // this is a form of registration, where a table of our methods
    // are added to the system via a system object's field - in this case,
    // we are using proc_fops field of proc_dir_entry {} - this is done
    // as per the rules !!! by doing this, we are providing custom 
    // operations(call backs) to our procfs file - whenever, 
    // our active file(??) of our procfs file is 
    // accessed, these custom methods will be accessed and accordingly,
    // data will be processed/retrieved !!!
    //
    // for a regular file of a regular filesystem, we need not 
    // provide custom operations and system's apppropriate 
    // file system manager will provide the operations table 1!


    //entry->proc_fops = &mydrv_proc_fops;
    // Done as per rules of the frame-works of procfs and sequence file layer

  //}                                       

  //else 
  
  
  /* Handcraft mydrv_list for testing purpose.
     In the real world, device driver logic or kernel's  sub-systems 
     maintain the list of objects and populates the fields */
    // we are creating our own dummy(private) objects and 
    // maintaining them in a 
    // system defined dummy data-structure/list - all rules/apis are provided
    // by the system !!! for a basic demo and illustration !!!
    // refer to chapter 11 of LDD/3 for details of objects and 
    // data structure !!!
    // kmalloc() is a system API that allocates physical memory 
    // and returns appropriate starting virtual/logical address of 
    // physical memory - there are several rules - for the time being,
    // let us use the default calling conventions !!!
    // you need not create objects and maintain a list in assignment 2
    // you will be using system objects(pd objects) and 
    // system data structures in assignment 2
    // in assignment 5, you will be accessing private objects of 
    // your devices and drivers !!!
    for (i=0;i<5;i++) 
    {
       mydrv_new = kmalloc(sizeof(struct _mydrv_struct), GFP_KERNEL);
       //check errors
       sprintf(mydrv_new->info, "Node No: %d\n", i);
       list_add_tail(&mydrv_new->list, &mydrv_list);
    }

    printk("we are in init function of the module\n");  //2
    return 0;
}

static void mydrv_exit(void)
{
   dump_stack();
   // incomplete

   struct _mydrv_struct *p,*n;
   //list_for_each_entry_safe(p,n, &mydrv_list, list) 
   //kfree(p);

   list_for_each_entry_safe(p,n, &mydrv_list, list)
   { 
     kfree(p);
     // additional steps are added 
   }
   //remove_proc_entry("readme", NULL);
   remove_proc_entry("readme", entry1);
   remove_proc_entry("proc_test", NULL);
   printk("mydrv_exit just executed\n");    //3

}

module_init(mydrv_init);
module_exit(mydrv_exit);

// add other macros as needed 

MODULE_LICENSE("GPL v2");
