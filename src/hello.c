/// --------------------------------------------------------
///  A simple `Hello, $(name)!` linux driver module.
///  
///  This kernel module implements a pseudo-device driver,
///  which offers the user two essential operations on the 
///  device: he can write to it specifying a name for greeting,
///  then he can read from it to get a greeting message in the
///  form like Hello, `name`!
///  
///  This device driver module does no real useful thing other
///  than illustrating some of the working mechanisms of linux
///  kernel drivers, and offering some enjoyable activity for
///  linux enthusiasts.
///  
///  Written by: Abdelbari BOUGOFFA
///  On: Oct 15th, 2025
/// --------------------------------------------------------


#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>

#define HELLO_MAJOR   300
#define NAME_OFFSET   7
#define MAX_NAME_LEN  100
#define MAX_MSG_LEN   NAME_OFFSET + MAX_NAME_LEN + 1

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

static struct cdev *my_cdev;
static char greet_msg[MAX_MSG_LEN + 1] = "Hello, World!";
static int greet_len = 13;

// FILE OPERATIONS -----------------------------------------------
static int hello_open(
  struct inode *inode, struct file *file)
{
  return 0;
}

static int hello_release(
  struct inode *inode, struct file *file)
{
  return 0;
}

static ssize_t hello_read(
  struct file *filp, char __user *buf,
  size_t count, loff_t *fpos)
{
  size_t readb, remb;

  if (filp->f_pos >= greet_len)
    return 0;
  
  count = MIN(count, greet_len);
  remb = copy_to_user(buf, greet_msg + filp->f_pos, count);
  
  readb = count - remb;
  *fpos += readb;

  return readb;
}

static ssize_t hello_write(
  struct file *file, const char __user *buffer,
  size_t count, loff_t *fpos)
{
  int i;
  size_t writb, remb;

  count = MIN(count, MAX_NAME_LEN);

  for (i = NAME_OFFSET; i < MAX_MSG_LEN; i++)
    greet_msg[i] = '\0';

  remb = copy_from_user(greet_msg + NAME_OFFSET, buffer, count);
  writb = count - remb;
  greet_msg[NAME_OFFSET + count] = '!';

  greet_len = NAME_OFFSET + writb + 1;

  return writb;
}

static struct file_operations hello_fops = {
  .owner   = THIS_MODULE,
  .open    = hello_open,
  .release = hello_release,
  .read    = hello_read,
  .write   = hello_write,
};

// ---------------------------------------------------------------

/*
  This function defines the entry point to the module.
  Any data structure initialization, memory allocation,
  and function registration must be done here.
*/
static int __init hello_init(void)
{
  int err;

  printk(KERN_INFO "Hello, world!\n");

  /* allocating a device number that we use to
    identify our interface with user-space. */
  dev_t devn = MKDEV(HELLO_MAJOR, 0);
  err = register_chrdev_region(devn, 1, "hello");
  if (err)  return 1;

  my_cdev = cdev_alloc();
  if (my_cdev == NULL)  return 1;

  /* specifying file operations for our char device */
  my_cdev->ops = &hello_fops;

  /* after the following step, driver functions
    are active, and may be invoked at any time. */
  err = cdev_add(my_cdev, devn, 1);
  if (err)  return 1;

  return 0;
}

/*
  This function defines the exit point of the module.
  Here, initialized data structures are destroyed,
  and allocated memory is freed. This does the rewinding
  of the module activity.
*/
static void __exit hello_exit(void)
{
  if (my_cdev != NULL)  cdev_del(my_cdev);
  unregister_chrdev_region(MKDEV(HELLO_MAJOR, 0), 1);

  printk(KERN_INFO "Goodbye, world!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
