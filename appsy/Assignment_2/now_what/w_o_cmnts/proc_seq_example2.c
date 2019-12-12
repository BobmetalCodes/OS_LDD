#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/proc_fs.h>
#include<linux/slab.h>
#include<linux/seq_file.h>
#include<linux/sched.h>
#include<linux/sched/signal.h>

static LIST_HEAD(mydrv_list);
static struct proc_dir_entry *entry = NULL ; 
static struct proc_dir_entry *entry1 = NULL ; 

struct _mydrv_struct {
    struct list_head list;
    char info[12];
};

/*
static void *mydrv_seq_start(struct seq_file *seq, loff_t *pos) {
    struct _mydrv_struct *p;
    loff_t off = 0;
 
    dump_stack(); 

    list_for_each_entry(p, &mydrv_list, list) {
       if (*pos == off++) {
          printk("in start : success %d\n",*pos);
          return p;
       }
    }
    printk("in seq_start : over\n");
    return NULL;
}
*/

static void *mydrv_seq_start(struct seq_file *seq, loff_t *pos) {
    struct task_struct *p;
    loff_t off = 0;

    dump_stack();
    for_each_process(p){
        if (*pos == off++) {
            printk("in start : SUCCESS %d\n",*pos);
            return p;
        }
    }
    printk("in seq_start : OVER\n");
    return NULL;
}

static void *mydrv_seq_next(struct seq_file *seq, void *v, loff_t *pos) {
    struct list_head *n = (((struct task_struct *)v)->tasks.next);
    ++*pos;
    printk("in seq_next :%lld\n",*pos);

    return(n != &mydrv_list) ? list_entry(n, struct task_struct, tasks) : NULL ;
}

static int mydrv_seq_show(struct seq_file *seq, void *v) {
   const struct task_struct *p = v;

   char *info_str = kmalloc(256, GFP_KERNEL);
   printk("in seq_show \n");

   sprintf(info_str, "pid is %d tgid is %d, cmd=%s, stack = %x\n", p->pid, p->tgid, p->comm, p-> stack);

   seq_printf(seq,info_str);
   kfree(info_str);

   return 0;

   //ret=seq_printf(seq,info_str);
   //printk(KERN_INFO "the return value of seq_printf is %d\n", ret); 
}

static void mydrv_seq_stop(struct seq_file *seq, void *v) { 
    printk("In mydrv_seq_stop :\n"); 
}

static struct seq_operations mydrv_seq_ops = {
   .start  = mydrv_seq_start,
   .next   = mydrv_seq_next,
   .stop   = mydrv_seq_stop,
   .show   = mydrv_seq_show,
};

static int mydrv_seq_open(struct inode *inode, struct file *file) {
   printk("In mydrv_seq_open\n");
   dump_stack();
   return seq_open(file, &mydrv_seq_ops);
}

static struct file_operations mydrv_proc_fops = {
   .owner    = THIS_MODULE,    
   .open     = mydrv_seq_open, 
   .read     = seq_read,
   .llseek   = seq_lseek,
   .release  = seq_release,
};

static int __init mydrv_init(void) {

    //struct _mydrv_struct *mydrv_new;

    entry1 = proc_mkdir("proc_test",  NULL);
    entry = proc_create("readme", S_IRUSR, entry1, &mydrv_proc_fops);
    if(!entry)  { return -EINVAL; }

    printk("In mydrv_init function of the module (initialization) \n");
    return 0;
}

static void mydrv_exit(void) {
    
    //struct _mydrv_struct *p,*n;
    //list_for_each_entry_safe(p,n, &mydrv_list, list)
    //    kfree(p);

    remove_proc_entry("readme", entry1);
    remove_proc_entry("proc_test", NULL);

    printk("mydrv_exit just executed\n");
}

MODULE_LICENSE("GPL v2");
module_init(mydrv_init);
module_exit(mydrv_exit);
