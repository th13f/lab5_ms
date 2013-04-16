#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/fs.h>


MODULE_AUTHOR("Serge Staravoitau");
MODULE_DESCRIPTION("Calculator character device driver");
MODULE_LICENSE("GPL");

static int device_open(struct inode *inode, struct file *file);
static int device_release(struct inode *inode, struct file *file);
static ssize_t device_read(	struct file *filp, char *buffer, size_t length, loff_t * offset);
static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t * off);

/* Char devices files names. */
#define CALC_FIRST "calc_first"
#define CALC_SECOND "calc_second"
#define CALC_OPERAND "calc_operand"
#define CALC_RESULT "calc_result"

/* Char devices maximum file size. */
#define FILE_MAX_SIZE 16

/* Char devices files names. */
static char names[][16] = {
        CALC_FIRST,
        CALC_SECOND,
        CALC_OPERAND,
        CALC_RESULT
};

/* Device opened counter. */
static int device_opened = 0;

/* Char devices files buffers. */
static char** devices_buffer;

/* User message. */
static char message[32];

/* Device operations struct. */
static struct file_operations fops = {
        .owner = THIS_MODULE,
        .read = device_read,
        .write = device_write,
        .open = device_open,
        .release = device_release
};

/* Devices numbers. */
static dev_t numbers[4];

/* Global var for the character device struct */
static struct cdev* c_dev;

/* Global var for the device class */
static struct class** classes;


static int device_open(struct inode *inode, struct file *file)
{
        if (device_opened)
                return -EBUSY;

        device_opened++;
        try_module_get(THIS_MODULE);

        return 0;
}


static int device_release(struct inode *inode, struct file *file)
{
        device_opened--;

        module_put(THIS_MODULE);

        return 0;
}


module_init(calc_init); /* Register module entry point */
module_exit(calc_exit); /* Register module cleaning up */
