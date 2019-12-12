#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>

struct foo_obj {
	struct kobject kobj;
	int foo;
	int baz;
	int bar;
};

#define to_foo_obj(x) container_of(x, struct foo_obj, kobj)

struct foo_attribute {
	struct attribute attr;
	ssize_t (*show)(struct foo_obj *foo, struct foo_attribute *attr, char *buf);
	ssize_t (*store)(struct foo_obj *foo, struct foo_attribute *attr, const char *buf, size_t count);
};

#define to_foo_attr(x) container_of(x, struct foo_attribute, attr)


            /* foo_attr_show */
static ssize_t foo_attr_show(struct kobject *kobj, struct attribute *attr, char *buf) {
	struct foo_attribute *attribute;
	struct foo_obj *foo;
    //dump_stack();
	attribute = to_foo_attr(attr);
	foo = to_foo_obj(kobj);
	if (!attribute->show)
		return -EIO;

	return attribute->show(foo, attribute, buf);
}


            /* foo_attr_store */
static ssize_t foo_attr_store(struct kobject *kobj, struct attribute *attr, const char *buf, size_t len) {
	struct foo_attribute *attribute;
	struct foo_obj *foo;
    //dump_stack();
	attribute = to_foo_attr(attr);
	foo = to_foo_obj(kobj);
	if (!attribute->store)
		return -EIO;

	return attribute->store(foo, attribute, buf, len);
}

           /* sysfs_ops and foo_sysfs_ops */
static struct sysfs_ops foo_sysfs_ops = {
	.show = foo_attr_show,
	.store = foo_attr_store,
};

         /* foo_release */
static void foo_release(struct kobject *kobj) {
    dump_stack();
	struct foo_obj *foo;
	foo = to_foo_obj(kobj);
	kfree(foo);
}

            /* foo_show */
static ssize_t foo_show(struct foo_obj *foo_obj, struct foo_attribute *attr, char *buf){
    dump_stack(); 
	return sprintf(buf, "%d\n", foo_obj->foo);
}

            /* foo_store */
static ssize_t foo_store(struct foo_obj *foo_obj, struct foo_attribute *attr, const char *buf, size_t count) {
    dump_stack(); 
	sscanf(buf, "%du", &foo_obj->foo);
	return count;
}

            /* b_show */
static ssize_t b_show(struct foo_obj *foo_obj, struct foo_attribute *attr, char *buf) {
	int var;
    dump_stack();
    if (strcmp(attr->attr.name, "dev_param2") == 0)
		var = foo_obj->baz;
	else
		var = foo_obj->bar;

	return sprintf(buf, "%d\n", var);
}

            /* b_store */
static ssize_t b_store(struct foo_obj *foo_obj, struct foo_attribute *attr, const char *buf, size_t count) {
	int var;
    dump_stack();
	sscanf(buf, "%du", &var);
	if (strcmp(attr->attr.name, "dev_param2") == 0)
		foo_obj->baz = var;
	else
		foo_obj->bar = var;
	return count;
}

static struct foo_attribute foo_attribute =
	__ATTR(dev_param1, 0600, foo_show, foo_store);
static struct foo_attribute baz_attribute =
	__ATTR(dev_param2, 0600, b_show, b_store);
static struct foo_attribute bar_attribute =
	__ATTR(dev_param3, 0600, b_show, b_store);


static struct attribute *foo_default_attrs[] = {
	&foo_attribute.attr,
	&baz_attribute.attr,
	&bar_attribute.attr,
	NULL,	/* need to NULL terminate the list of attributes */
};


           /* kobj_type and foo_ktype */
static struct kobj_type foo_ktype = {
	.sysfs_ops = &foo_sysfs_ops,
	.release = foo_release,
	.default_attrs = foo_default_attrs,
};

static struct kset *example_kset;
static struct foo_obj *foo_obj;
static struct foo_obj *bar_obj;
static struct foo_obj *baz_obj;


                    /* create_foo_obj */
static struct foo_obj *create_foo_obj(const char *name) {
	struct foo_obj *foo;
	int retval;
        foo = kzalloc(sizeof(*foo), GFP_KERNEL);
	if (!foo)
		return NULL;

	foo->kobj.kset = example_kset;
	retval = kobject_init_and_add(&foo->kobj, &foo_ktype, NULL, "%s", name);
	if (retval) {
		kfree(foo);
		return NULL;
	}
	return foo;
}
         /* destroy_foo_obj */
static void destroy_foo_obj(struct foo_obj *foo) {
	kobject_put(&foo->kobj);
}


              /* __init */
static int example_init(void) {
	example_kset = kset_create_and_add("kset_devices_typeA", NULL, kernel_kobj);
	if (!example_kset)
		return -ENOMEM;

	foo_obj = create_foo_obj("device0");
	if (!foo_obj)
		goto foo_error;

	bar_obj = create_foo_obj("device1");
	if (!bar_obj)
		goto bar_error;

	baz_obj = create_foo_obj("device2");
	if (!baz_obj)
		goto baz_error;

	return 0;

baz_error:
	destroy_foo_obj(bar_obj);
bar_error:
	destroy_foo_obj(foo_obj);
foo_error:
	return -EINVAL;
}

                /* _exit */
static void example_exit(void) {
	destroy_foo_obj(baz_obj);
	destroy_foo_obj(bar_obj);
	destroy_foo_obj(foo_obj);
	kset_unregister(example_kset);
}

module_init(example_init);
module_exit(example_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Greg Kroah-Hartman <greg@kroah.com>");
