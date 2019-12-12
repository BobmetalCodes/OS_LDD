#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/proc_fs.h>
#include<linux/slab.h>
#include<linux/seq_file.h>


/* Private Data structure */
struct _mydrv_struct 
{
   struct list_head list;  /* Link to the next node */
   char info[13];
   char info1[10];         /* Info to pass via the procfs file */
};


static LIST_HEAD(mydrv_list);  
/* List Head */
static struct proc_dir_entry *entry = NULL ; 


/* start() method */
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
	    printk("In start : success %d\n",*pos);
            return p;
    }
  }
  printk("In seq_start : over\n");
  return NULL;
}


/* next() method */
static void *mydrv_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
  dump_stack();
  /*
   * 'v' is a pointer to the iterator returned by start() or
   * by the previous invocation of next() 
  */
  struct list_head *n = ((struct _mydrv_struct *)v)->list.next;
  ++*pos;
  /* Advance position */

  /* Return the next iterator, which is the next node in the list */  
  printk("in seq_next :%d\n",*pos);
  return(n != &mydrv_list) ? list_entry(n, struct _mydrv_struct, list) : NULL;
}


/* show() method */
static int mydrv_seq_show(struct seq_file *seq, void *v)
{
   int ret;
   struct _mydrv_struct  *p =v;
   dump_stack();

   /* Interpret the iterator, 'v' */
   printk("in seq_show \n");

   ret = seq_printf(seq,"%s\n",p->info);
   printk(KERN_INFO "the return value of seq_printf is %d\n", ret); 
   return 0;
}


/* stop() method */
static void mydrv_seq_stop(struct seq_file *seq, void *v)
{
   /* No cleanup needed in this example */
   dump_stack();
   printk("in seq_stop:\n");
}


/* Define iterator operations */
static struct seq_operations mydrv_seq_ops = {
   .start = mydrv_seq_start,
   .next   = mydrv_seq_next,
   .stop   = mydrv_seq_stop,
   .show   = mydrv_seq_show,
};


static int mydrv_seq_open(struct inode *inode, struct file *file)
{
   printk("we are in mydrv_seq_open\n");   				//1
   
   /* Register the operations */
   dump_stack();  //used for diagnostic messages
  
   return seq_open(file, &mydrv_seq_ops);
}

static struct file_operations mydrv_proc_fops = {
   .owner    = THIS_MODULE,       // this macro will provide the ptr to our module object
   .open     = mydrv_seq_open,    /* User supplied */
   .read     = seq_read,          // Built-in helper functions taken from fs/seq_file.c 
   .llseek   = seq_lseek,         // fs/seq_file.c provides sequence file layer - sequence file layer provides
   .release  = seq_release,       // support to procfs layer 
};

static int __init mydrv_init(void)
{
   int i;
   struct _mydrv_struct *mydrv_new;
   dump_stack();

   /* Create /proc/readme */
   entry1 = proc_mkdir("proc_test",  NULL);//a file is created 
   entry = create_proc_entry("readme", S_IRUSR, entry1);//a file is created 

   /* Attach it to readme_proc() */
   if (entry) 
   {
    	entry->proc_fops = &mydrv_proc_fops;
   }  
   else 
   {
	return -EINVAL; //init method has failed !!!
   }
  
   for (i=0;i<100;i++) 
   {
        mydrv_new = kmalloc(sizeof(struct _mydrv_struct), GFP_KERNEL);
        sprintf(mydrv_new->info, "Node No: %d\n", i);
        list_add_tail(&mydrv_new->list, &mydrv_list);
   }
   printk("we are in init function of the module\n");  			//2
   return 0;
}

static void mydrv_exit(void)
{
   dump_stack();
   //incomplete

   struct _mydrv_struct *p,*n;
   //list_for_each_entry_safe(p,n, &mydrv_list, list) 
   //kfree(p);

   list_for_each_entry_safe(p,n, &mydrv_list, list)
   { 
     kfree(p);
   }
   //remove_proc_entry("readme", NULL);
   remove_proc_entry("readme", entry1);
   remove_proc_entry("proc_test", NULL);
   printk("mydrv_exit just executed\n");    				//3
}

module_init(mydrv_init);
module_exit(mydrv_exit);

MODULE_LICENSE("GPL v2");
