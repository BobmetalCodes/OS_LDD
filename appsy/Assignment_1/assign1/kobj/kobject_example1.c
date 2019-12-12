#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>
 
static int foo;
static int baz;
//static int bar;
//static char baz;
static char bar;

static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
        dump_stack();
        int ret;
    	//printk("data string is %s\n", buf);
        ret=sprintf(buf, "%d", foo);
        printk("data string is %s\n", buf);
        return ret;
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
        dump_stack();
	    sscanf(buf, "%d", &foo);
	    printk("Data is %d\n", foo);
	    return count;
}

static ssize_t b_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    	int var;
        char cvar;
        dump_stack();
    	if (strcmp(attr->attr.name, "baz") == 0)
        {
    	    	var = baz;
                printk("Data var (from baz) is %d\n", var);	    
          	    return sprintf(buf, "%d\n", var);
        }
        else
        {
    	    	cvar = bar;
                printk("Data var (from bar) is %c\n", cvar);
          	    return sprintf(buf, "%c\n", cvar);
        }
}

static ssize_t b_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
        int var;
        char cvar;
        dump_stack();
	    if (strcmp(attr->attr.name, "baz") == 0)
	    {
                sscanf(buf, "%d", &var);        
    	        baz = var;
                //baz = cvar1;
                printk("Data baz is %d\n", baz);
        }
	    else
        {
                sscanf(buf, "%c", &cvar);
        		bar = cvar;
        		//bar = cvar1;
                printk("Data bar is %c\n", bar);
        }
	    return count;
}

static struct kobj_attribute foo_attribute =
	//__ATTR(foo, 0666, foo_show, foo_store);
    //__ATTR(foo, 0600, foo_show, foo_store);
	__ATTR(foo, 0664, foo_show, foo_store);
static struct kobj_attribute baz_attribute =
	__ATTR(baz, 0664, b_show, b_store);
	//__ATTR(baz, 0666, b_show, b_store);
static struct kobj_attribute bar_attribute =
	__ATTR(bar, 0664, b_show, b_store);

static struct attribute *attrs[] = {
	&foo_attribute.attr,
	&baz_attribute.attr,
	&bar_attribute.attr,
	NULL,	
};


static struct attribute_group attr_group = {
	.attrs = attrs,
};

static struct kobject *example_kobj;

static int __init example_init(void) {
	int retval=0;

	example_kobj = kobject_create_and_add("kobject_example", kernel_kobj);
	if (!example_kobj)
		return -ENOMEM; 

    retval = sysfs_create_group(example_kobj, &attr_group);
	//if (retval)
	//  kobject_put(example_kobj);
    //sysfs_create_file(example_kobj, &new_attribute1.attr);
    //sysfs_create_file(example_kobj, &new_attribute2.attr);
	return retval;
}

static void __exit example_exit(void) {
	kobject_put(example_kobj);
}

module_init(example_init);
module_exit(example_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Greg Kroah-Hartman <greg@kroah.com>");
