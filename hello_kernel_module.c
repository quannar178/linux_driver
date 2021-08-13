#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/device.h>
#include <linux/cdev.h>
  
static dev_t first;         // Global variable for the first device number
static struct cdev c_dev;     // Global variable for the character device structure
static struct class *cl;     // Global variable for the device class
static int value_GPIO = 0;
static int init_result;
 
static ssize_t gpio_read( struct file* F, char *buf, size_t count, loff_t *f_pos )
{
    char buffer[10];
 
    int temp = value_GPIO;
 
    sprintf( buffer, "%1d" , temp );
 
    count = sizeof( buffer );
 
    if( copy_to_user( buf, buffer, count ) )
    {
        return -EFAULT;
    }
 
    if( *f_pos == 0 )
    {
        *f_pos += 1;
        return 1;
    }
    else
    {
        return 0;
    }
}
 
static ssize_t gpio_write( struct file* F, const char *buf, size_t count, loff_t *f_pos )
{
 
    printk(KERN_INFO "Executing WRITE.\n");
 
    switch( buf[0] )
    {
        case '0':
            printk("Set value : 0\n");
            value_GPIO = 0;
            break;
 
        case '1':
            printk("Set value : 1\n");
            value_GPIO = 1;
            break;
 
        default:
            printk("Wrong option.\n");
            break;
    }
    return count;
}
 
static int gpio_open( struct inode *inode, struct file *file )
{
    return 0;
}
 
static int gpio_close( struct inode *inode, struct file *file )
{
    return 0;
}
 
static struct file_operations FileOps =
{
    .owner        = THIS_MODULE,
    .open         = gpio_open,
    .read         = gpio_read,
    .write        = gpio_write,
    .release      = gpio_close,
};
 
static int init_gpio(void)
{
     
    init_result = alloc_chrdev_region( &first, 0, 1, "gpio_drv" );
 
    if( 0 > init_result )
    {
        printk( KERN_ALERT "Device Registration failed\n" );
        return -1;
    }
 
    if ( (cl = class_create( THIS_MODULE, "chardev" ) ) == NULL )
    {
        printk( KERN_ALERT "Class creation failed\n" );
        unregister_chrdev_region( first, 1 );
        return -1;
    }
 
    if( device_create( cl, NULL, first, NULL, "gpio_drv" ) == NULL )
    {
        printk( KERN_ALERT "Device creation failed\n" );
        class_destroy(cl);
        unregister_chrdev_region( first, 1 );
        return -1;
    }
 
    cdev_init( &c_dev, &FileOps );
 
    if( cdev_add( &c_dev, first, 1 ) == -1)
    {
        printk( KERN_ALERT "Device addition failed\n" );
        device_destroy( cl, first );
        class_destroy( cl );
        unregister_chrdev_region( first, 1 );
        return -1;
    }

    printk( KERN_INFO "Device addition success\n" );
 
    return 0;
}
 
void cleanup_gpio(void)
{
    cdev_del( &c_dev );
    device_destroy( cl, first );
    class_destroy( cl );
    unregister_chrdev_region( first, 1 );
 
    printk(KERN_ALERT "Device unregistered\n");
}
 
module_init(init_gpio);
module_exit(cleanup_gpio);
 
MODULE_AUTHOR("BaQuan");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Demo driver");