//
//  Adapted from $LINUX_SRC_TREE/samples/kobject/kobject-example.c
//
//  Copyright (C) 2004-2007 Greg Kroah-Hartman <greg@kroah.com>
//  Copyright (C) 2007 Novell Inc.
// Released under the GPL version 2 only.
//

#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>

//
// Create the subdirectory 
//     /sys/kernel/tq84_kobj
// in the sysfs.
//
// The directory will expose three files to store values
// in kernel space:
//   filename_1, filename_2 and filename_3
//
//

static int val_1, val_2, val_3;

static ssize_t store_val_1(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
 //
 // Something was written into /sys/kernel/tq84_kobj/filename_1
 // Store it into the variable val_1
 //
    int ret;

    ret = kstrtoint(buf, 10, &val_1);

    if (ret < 0) return ret;

    return count;
}

static ssize_t get_val_1(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
 //
 // It was requested to read the value of /sys/kernel/tq84_kobj/filename_1
 //
 // Return the value of val_1 as a string.
 //
    return sprintf(buf, "%d\n", val_1);
}

static ssize_t store_dynam(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
 //
 // A more dynamic approach.
 //
 // Determine the name of the file by attr->attr.name
 // and set respective variable (val_2 or val_3) respectively
 //
    int val, ret;

    ret = kstrtoint(buf, 10, &val);

    if (ret < 0) return ret;

    if (strcmp(attr->attr.name, "filename_2") == 0) val_2 = val;
    else                                            val_3 = val;

    return count;
}


static ssize_t get_dynam(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
 //
 // Same idea for a more dynamic approach.
 //
    int val;

    if (strcmp(attr->attr.name, "filename_2") == 0) val = val_2;
    else                                            val = val_3;

    return sprintf(buf, "%d\n", val);
}


//
//   Specify the file names for the attributes. The filename
//   is given as first argument to the __ATTR() macro.
// 
//   Sysfs attributes cannot be world-writable.
//
static struct kobj_attribute attribute_1 = __ATTR(filename_1, 0664, get_val_1, store_val_1);
static struct kobj_attribute attribute_2 = __ATTR(filename_2, 0664, get_dynam, store_dynam);
static struct kobj_attribute attribute_3 = __ATTR(filename_3, 0664, get_dynam, store_dynam);



/*
 * Create a group of attributes so that we can create and destroy them all
 * at once.
 */
static struct attribute *attrs[] = {
    &attribute_1.attr,
    &attribute_2.attr,
    &attribute_3.attr,
    NULL,    /* need to NULL terminate the list of attributes */
};

static struct attribute_group attr_group = {


 //
 //    The name is not explicitely specified. This creates an unnamed
 //    group, directly under /sys/kernel/tq84_kobj
 //
 // .name  = "subdirectory-name",
 //
 //    If we were to specify the name, the attributes would be
 //    places below this name, eg
 //        /sys/kernel/tq84_kobj/subdirectory-name
 //
 

    .attrs = attrs,
};

static struct kobject *example_kobj;

static int __init example_init(void) {
    int retval;

 //
 // Create the kobject under /sys/kernel (kernel_kobj).
 // Note, it's only an example, so no uevent is sent to userspace.
 //
    example_kobj = kobject_create_and_add("tq84_kobj", kernel_kobj);

 //
 // Evertything ok?
 //
    if (!example_kobj) return -ENOMEM;

    /* Create the files associated with this kobject */
    retval = sysfs_create_group(example_kobj, &attr_group);
    if (retval)
        kobject_put(example_kobj);

    return retval;
}

static void __exit example_exit(void) {
    kobject_put(example_kobj);
}

module_init(example_init);
module_exit(example_exit);

MODULE_LICENSE("GPL v2");
