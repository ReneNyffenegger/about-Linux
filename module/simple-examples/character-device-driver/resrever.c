/*
 *    derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include "resrever.h"


 //
 // The class name specifies the directory
 // under /sys/class/ where it appears:
 //   /sys/class/tq84_class
 //
 // Apparently,is somehow specifies the 'subsystem'.
 //
#define TQ84_CLASS_NAME  "tq84_class"


MODULE_LICENSE("GPL");
MODULE_AUTHOR("René Nyffenegger");
MODULE_DESCRIPTION("TQ84 character device");
MODULE_VERSION("1.0");

static int    majNr;

  //
  //          reversed_text contains the last text that was written into
  //          the device with write(...)
  //
static char   reversed_text[256] = {0};
static int    reversed_text_len = 0;
static struct class*  tq84CharClass  = NULL;
static struct device* tq84CharDevice = NULL;

static int     open_device      (struct inode*, struct file*);
static int     release_device   (struct inode*, struct file*);
static ssize_t read_from_device (struct file* , char*, size_t, loff_t*);
static ssize_t write_to_device  (struct file* , const char*, size_t, loff_t*);

static struct file_operations fops = {
  .open    = open_device,
  .read    = read_from_device,
  .write   = write_to_device,
  .release = release_device,
};


static int __init tq84_dev_init(void) {

    majNr = register_chrdev(0, TQ84_DEVICE_NAME, &fops);
    if (majNr < 0) {
      printk(KERN_ALERT TQ84_DEVICE_NAME ": could not register a new major number\n");
      return majNr;
    }
  
 //
 // The type of THIS_MODULE is struct module*.
 //    
    tq84CharClass = class_create(THIS_MODULE, TQ84_CLASS_NAME);

    if (IS_ERR(tq84CharClass)) {
      unregister_chrdev(majNr, TQ84_DEVICE_NAME);
      printk(KERN_ALERT TQ84_DEVICE_NAME ": Failed to register device class\n");
      return PTR_ERR(tq84CharClass);
    }
  
    tq84CharDevice = device_create(tq84CharClass, NULL, MKDEV(majNr, 0), NULL, TQ84_DEVICE_NAME);

    if (IS_ERR(tq84CharDevice)) {
      class_destroy(tq84CharClass);
      unregister_chrdev(majNr, TQ84_DEVICE_NAME);
      printk(KERN_ALERT TQ84_DEVICE_NAME ": Could not create the device\n");
      return PTR_ERR(tq84CharDevice);
    }
  
  
    printk(KERN_INFO TQ84_DEVICE_NAME ": allocated major number: %d\n", majNr);
    return 0;
}

static void __exit tq84_dev_exit(void) {
    device_destroy(tq84CharClass, MKDEV(majNr, 0));
    class_unregister(tq84CharClass);
    class_destroy(tq84CharClass);
    unregister_chrdev(majNr, TQ84_DEVICE_NAME);
    printk(KERN_INFO TQ84_DEVICE_NAME ": device driver unloaded\n");
}

static int open_device(struct inode* inodep, struct file* filep) {
    printk(KERN_INFO TQ84_DEVICE_NAME ": opened\n"); 
    return 0;
}

static ssize_t read_from_device(struct file* filep, char* buffer, size_t len, loff_t* offset) {
    int error_count = 0;
    error_count = copy_to_user(buffer, reversed_text, reversed_text_len);
  
    if (error_count == 0) {
      return 0;
    }
  
    printk(KERN_ALERT TQ84_DEVICE_NAME ": Failed to send %d characters\n", error_count);
    return -EFAULT;
}

static ssize_t write_to_device(struct file *filep, const char *buffer, size_t len, loff_t *offset) {

    unsigned int left  = 0;
    unsigned int right = len-1;

    memcpy(reversed_text, buffer, len);
    reversed_text_len = len;

    while (left < right) {
	char c = reversed_text[left];
	reversed_text[left ] = reversed_text[right];
	reversed_text[right] = c;

	left ++;
	right--;
    }
    reversed_text[reversed_text_len] = 0;

    printk(KERN_INFO TQ84_DEVICE_NAME ": reversed_text=%s\n", reversed_text);
    return len;
}

static int release_device(struct inode *inodep, struct file* filep) {
    printk(KERN_INFO TQ84_DEVICE_NAME ": device susscessfully closed\n");
  
    return 0;
}

module_init(tq84_dev_init);
module_exit(tq84_dev_exit);
