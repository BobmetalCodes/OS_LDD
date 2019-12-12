#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>

static char foo[30];
static char baz[32];
static char bar[30];

static ssize_t foo_show(struct kobject *kobj,struct kobj_attribute *attr,char *buf)
{
	/*char var;
        dump_stack();
        var= foo;
	//sprintf(buf, "%d", foo);
        printk("data string is %s\n", buf);
	return sprintf(buf, "%c\n", var);*/



	char var[10];
        dump_stack();
        strcpy(var,foo);
        printk("data string is %s\n", buf);
        return sprintf(buf, "%s\n", var);

}


static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{

        /*dump_stack();
	sscanf(buf, "%cu", &foo);
	printk("data  is %d\n", foo);
	return count;*/



	char var[10];
        dump_stack();
        sscanf(buf, "%su", var);
        sprintf(foo, "%s", var);
        printk("data(foo)is %s\n", foo);
   // lseek(fd,0,SEEK_SET);
        return count;
}

static struct kobj_attribute foo_attribute =__ATTR(foo, 0644, foo_show, foo_store);

static ssize_t bar_show(struct kobject *kobj,struct kobj_attribute *attr,char *buf)
{
	/*char var;
        dump_stack();
	var = bar;
        printk("data string is %s\n", buf);
	return sprintf(buf, "%c\n", var);*/




	char var[10];
        dump_stack();
        strcpy(var,bar);
        printk("data string is %s\n", buf);
        return sprintf(buf, "%s\n", var);

}

static ssize_t bar_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	/*char var;
        dump_stack();
        sscanf(buf, "%cu", &var);
	bar = var;
        printk("data(bar)is %c\n", bar);
        return count;*/


	char var[10];
        dump_stack();
        sscanf(buf, "%su", var);
        sprintf(bar, "%s", var);
        printk("data(bar)is %s\n", bar);
   // lseek(fd,0,SEEK_SET);
        return count;
}

static struct kobj_attribute bar_attribute =
        __ATTR(bar, 0644, bar_show, bar_store);

static ssize_t baz_show(struct kobject *kobj,struct kobj_attribute *attr,char *buf)
{
	char var[10];
        dump_stack();
	strcpy(var,baz);
        printk("data string is %s\n", buf);
	return sprintf(buf, "%s\n", var);

}

static ssize_t baz_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{

	char var[10];
        dump_stack();
        sscanf(buf, "%su", var);
	sprintf(baz, "%s", var); 
        printk("data(baz)is %s\n", baz);
    //lseek(fd,0,SEEK_SET);
        return count;
}

static struct kobj_attribute baz_attribute =__ATTR(baz, 0644, baz_show, baz_store);

static struct attribute *attrs[] = {
	&foo_attribute.attr,
	&baz_attribute.attr,
	&bar_attribute.attr,
	NULL,	/* need to NULL terminate the list of attributes */
};

static struct attribute_group attr_group = {
	.attrs = attrs,
};

static struct kobject *example_kobj;

static int __init example_init(void)
{
	int retval=0;
	example_kobj = kobject_create_and_add("kobject_example", kernel_kobj);
	if (!example_kobj)
		return -ENOMEM; //kobject is typically allocated from 
        retval = sysfs_create_group(example_kobj, &attr_group);
	return retval;
}

static void __exit example_exit(void)
{
	kobject_put(example_kobj);
}

module_init(example_init);
module_exit(example_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Greg Kroah-Hartman <greg@kroah.com>");
