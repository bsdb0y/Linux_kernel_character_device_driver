#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/version.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Neeraj Pal <neerajpal09@gmail.com>");
MODULE_DESCRIPTION("Working Sample  Character Driver");


static dev_t first;
static struct device *dev_ret;
static struct cdev c_dev;
static struct class *cl;


static int dev_open(struct inode *,struct file *);
static int dev_release(struct inode *,struct file *);
static ssize_t dev_read(struct file *, char __user *,size_t count, loff_t *);
static ssize_t dev_write(struct file *, const char __user *,size_t count, loff_t *);

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = dev_open,
	.read = dev_read,
	.write = dev_write,
	.release = dev_release
};

static int __init chardev_init(void)
{
	int ret;
	printk(KERN_ALERT "char device registered\n");
	if ((ret = alloc_chrdev_region(&first,0,1,"Namastey")) < 0)
	{
		return ret;
	}
	if (IS_ERR(cl = class_create(THIS_MODULE,"hello")))
	{
		unregister_chrdev_region(first,1);
		return PTR_ERR(cl);
	}
	if (IS_ERR(dev_ret = device_create(cl,NULL,first,NULL,"mychar")))
	{
		class_destroy(cl);
		unregister_chrdev_region(first,1);
		return PTR_ERR(dev_ret);
	}

	cdev_init(&c_dev,&fops);
	
	if ((ret = cdev_add(&c_dev, first, 1)) < 0)
	{
		device_destroy(cl, first);
		class_destroy(cl);
		unregister_chrdev_region(first, 1);
		return ret;
	}
	return 0;
}

static void __exit chardev_exit(void)
{
	cdev_del(&c_dev);
	device_destroy(cl, first);
	class_destroy(cl);
	unregister_chrdev_region(first, 1);
	printk(KERN_INFO " Unloaded \n");
}


static int dev_open(struct inode *i, struct file *f)
{
	printk(KERN_INFO "Driver: open()\n");
	return 0;
}
static int dev_release(struct inode *i, struct file *f)
{
	printk(KERN_INFO "Driver: close()\n");
	return 0;
}
static ssize_t dev_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
	printk(KERN_INFO "Driver: read()\n");
	return 0;
}
static ssize_t dev_write(struct file *f, const char __user *buf, size_t len,
	loff_t *off)
{
	printk(KERN_INFO "Driver: write()\n");
	return len;
}


module_init(chardev_init);
module_exit(chardev_exit);


