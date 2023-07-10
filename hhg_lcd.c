/***************************************************************************
 * 
 * Hi Happy Garden LCD Driver
 * Copyright (C) 2023  Antonio Salsi <passy.linux@zresa.it>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 * 
 ***************************************************************************/

#include "hhg_lcd.h"

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <linux/cdev.h>

#ifdef pr_fmt
#undef pr_fmt
#define pr_fmt(fmt) "hhg_lcd: " fmt
#endif

#define HHG_IS_VALID(gpio, msg) \
if (!gpio_is_valid(gpio)) \
{ \
    pr_err("GPIO " msg " wrong pin:%d", gpio); \
    return -EINVAL; \
} 

#define READ_BUFF_LEN (256)

//LCD MANAGEMENT

static bool data_mode_8_bit = false;
static bool enable_read_mode = false;

static short gpio_rs  = -1;
static short gpio_rw  = -1;
static short gpio_en  = -1;
static short gpio_db0  = -1;
static short gpio_db1  = -1;
static short gpio_db2  = -1;
static short gpio_db3  = -1;
static short gpio_db4  = -1;
static short gpio_db5  = -1;
static short gpio_db6  = -1;
static short gpio_db7  = -1;
static short gpio_db[]  = {
    [0] = -1,
    [1] = -1,
    [2] = -1,
    [3] = -1,
    [4] = -1,
    [5] = -1,
    [6] = -1,
    [7] = -1,
    NULL
};


// static decl
/**
 * @brief Initializes the LCD in 8-bit mode.
 *
 * This function initializes the LCD in 8-bit mode.
 *
 * @return `true` if the initialization was successful, `false` otherwise.
 */
static bool hhg_lcd_init_8_bit(void);

/**
 * @brief Initializes the LCD in 4-bit mode.
 *
 * This function initializes the LCD in 4-bit mode.
 *
 * @return `true` if the initialization was successful, `false` otherwise.
 */
static bool hhg_lcd_init_4_bit(void);

/**
 * @brief Sends one byte of data to the LCD.
 *
 * This function sends one byte of data to the LCD.
 *
 * @param byte The byte of data to be sent.
 * @param mode The mode of operation (data or command).
 */
static void hhg_lcd_send_byte(__u8 byte, __u8 mode);

/**
 * @brief Sends a command to the LCD.
 *
 * This function sends a command to the LCD.
 *
 * @param command The command to be sent.
 */
static void hhg_lcd_send_command(__u8 command);

bool hhg_lcd_init_8_bit(void)
{
    pr_info("init 8 bit data mode done"
    " gpio_rs:%d"
    " gpio_rw:%d"
    " gpio_en:%d"
    " gpio_db0:%d"
    " gpio_db1:%d"
    " gpio_db2:%d"
    " gpio_db3:%d"
    " gpio_db4:%d"
    " gpio_db5:%d"
    " gpio_db6:%d"
    " gpio_db7:%d"
    , gpio_rs
    , gpio_rw
    , gpio_en
    , gpio_db0
    , gpio_db1
    , gpio_db2
    , gpio_db3
    , gpio_db4
    , gpio_db5
    , gpio_db6
    , gpio_db7
    );

    return false;
}

bool hhg_lcd_init_4_bit(void)
{

    pr_info("init 4 bit data mode done"
    " gpio_rs:%d"
    " gpio_rw:%d"
    " gpio_en:%d"
    " gpio_db4:%d"
    " gpio_db5:%d"
    " gpio_db6:%d"
    " gpio_db7:%d"
    , gpio_rs
    , gpio_rw
    , gpio_en
    , gpio_db4
    , gpio_db5
    , gpio_db6
    , gpio_db7
    );

    HHG_IS_VALID(gpio_rs, "RS")
    if(gpio_rw > -1)
    {
        HHG_IS_VALID(gpio_rw, "RW")
    }
    HHG_IS_VALID(gpio_en, "EN")
    HHG_IS_VALID(gpio_db4, "DB4")
    HHG_IS_VALID(gpio_db5, "DB5")
    HHG_IS_VALID(gpio_db6, "DB6")
    HHG_IS_VALID(gpio_db7, "DB7")
    
    gpio_request_one(gpio_rs, GPIOF_OUT_INIT_LOW, "RS");
    if(gpio_rw > -1)
    {
        gpio_request_one(gpio_rw, GPIOF_OUT_INIT_LOW, "RW");
    }
    gpio_request_one(gpio_en, GPIOF_OUT_INIT_LOW, "EN");
    gpio_request_one(gpio_db4, GPIOF_OUT_INIT_LOW, "DB4");
    gpio_request_one(gpio_db5, GPIOF_OUT_INIT_LOW, "DB5");
    gpio_request_one(gpio_db6, GPIOF_OUT_INIT_LOW, "DB6");
    gpio_request_one(gpio_db7, GPIOF_OUT_INIT_LOW, "DB7");

    //TODO: init procedure

    return 0;
}



void hhg_lcd_send_byte(__u8 byte, __u8 mode)
{

    //TODO: send byte
    msleep(1);
}

void hhg_lcd_send_command(__u8 command)
{
    //TODO: send command
}


void hhg_lcd_send_data(__u8 data)
{
    //TODO: send data
}
EXPORT_SYMBOL(hhg_lcd_send_data);

void hhg_lcd_send_str(const char* str, __u16 len)
{
    //TODO: send string
}
EXPORT_SYMBOL(hhg_lcd_send_str);


//MODULE



// data
static dev_t hhg_dev = 0;  ///< Device number for the HHG device.
static struct class* hhg_class;  ///< Pointer to the device class for the HHG device.
static struct cdev hhg_cdev;  ///< Character device structure for the HHG device.
static atomic_t device_busy = ATOMIC_INIT(0);  ///< Atomic variable to track device availability.

/**
 * @brief Uevent callback function for HHG LCD device.
 *
 * This function is called when a uevent (user event) occurs for the HHG LCD device.
 * It handles changing the access permissions in user space.
 *
 * @param dev Pointer to the device structure.
 * @param env Pointer to the kobj_uevent_env structure containing the uevent environment.
 * @return The return value indicates the success or failure of the uevent handling.
 */
static int hhg_lcd_uevent(struct device* dev, struct kobj_uevent_env* env);

/**
 * @brief File operations open function for HHG LCD device.
 *
 * This function is called when the HHG LCD device file is opened.
 *
 * @param inode Pointer to the inode structure.
 * @param file Pointer to the file structure.
 * @return The return value indicates the success or failure of the file open operation.
 */
static int hhg_lcd_fops_open(struct inode* inode, struct file* file);

/**
 * @brief File operations release function for HHG LCD device.
 *
 * This function is called when the HHG LCD device file is closed.
 *
 * @param inode Pointer to the inode structure.
 * @param file Pointer to the file structure.
 * @return The return value indicates the success or failure of the file release operation.
 */
static int hhg_lcd_fops_release(struct inode* inode, struct file* file);

/**
 * @brief File operations read function for HHG LCD device.
 *
 * This function is called when a read operation is performed on the HHG LCD device file.
 *
 * @param filp Pointer to the file structure.
 * @param buff User buffer to store the read data.
 * @param len Number of bytes to read.
 * @param off Pointer to the file offset.
 * @return The number of bytes read, or an error code indicating the failure of the read operation.
 */
static ssize_t hhg_lcd_fops_read(struct file* filp, char __user* buff, size_t len, loff_t* off);

/**
 * @brief File operations write function for HHG LCD device.
 *
 * This function is called when a write operation is performed on the HHG LCD device file.
 *
 * @param filp Pointer to the file structure.
 * @param buff User buffer containing the data to be written.
 * @param len Number of bytes to write.
 * @param off Pointer to the file offset.
 * @return The number of bytes written, or an error code indicating the failure of the write operation.
 */
static ssize_t hhg_lcd_fops_write(struct file* filp, const char* buff, size_t len, loff_t* off);


module_param(gpio_rs, short, 0660);
MODULE_PARM_DESC(gpio_rs, "GPIO RS - Select registers");

module_param(gpio_rw, short, 0660);
MODULE_PARM_DESC(gpio_rw, "GPIO RW - Select read or write");

module_param(gpio_en, short, 0660);
MODULE_PARM_DESC(gpio_en, "GPIO EN - Start data read/write");

module_param(gpio_db0, short, 0660);
MODULE_PARM_DESC(gpio_db0, "GPIO DB0 - bit 0");

module_param(gpio_db1, short, 0660);
MODULE_PARM_DESC(gpio_db1, "GPIO DB1 - bit 1");

module_param(gpio_db2, short, 0660);
MODULE_PARM_DESC(gpio_db2, "GPIO DB2 - bit 2");

module_param(gpio_db3, short, 0660);
MODULE_PARM_DESC(gpio_db3, "GPIO DB3 - bit 3");

module_param(gpio_db4, short, 0660);
MODULE_PARM_DESC(gpio_db4, "GPIO DB4 - bit 4");

module_param(gpio_db5, short, 0660);
MODULE_PARM_DESC(gpio_db5, "GPIO DB5 - bit 5");

module_param(gpio_db6, short, 0660);
MODULE_PARM_DESC(gpio_db6, "GPIO DB6 - bit 6");

module_param(gpio_db7, short, 0660);
MODULE_PARM_DESC(gpio_db7, "GPIO DB7 - bit 7");


// File operation structure
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = hhg_lcd_fops_read,
    .write = hhg_lcd_fops_write,
    .open = hhg_lcd_fops_open,
    .release = hhg_lcd_fops_release
};


/*
* Module Init function
*/
static int __init hhg_lcd_init(void)
{
    /*Allocating Major number*/
    if ((alloc_chrdev_region(&hhg_dev, HHG_MAJOR_NUM_START, HHG_MINOR_NUM_COUNT, HHG_DRIVER_NAME)) < 0)
    {
        pr_err("cannot allocate major number\n");
        goto r_unreg;
    }
    pr_info("Major = %d Minor = %d \n", MAJOR(hhg_dev), MINOR(hhg_dev));

    /*Creating cdev structure*/
    cdev_init(&hhg_cdev, &fops);

    /*Adding character device to the system*/
    if ((cdev_add(&hhg_cdev, hhg_dev, 1)) < 0)
    {
        pr_err("cannot add the device to the system\n");
        goto r_dev;
    }

    /*Creating struct class*/
    if ((hhg_class = class_create(THIS_MODULE, HHG_CLASS_NAME)) == NULL)
    {
        pr_err("cannot create the struct class\n");
        goto r_class;
    }
    hhg_class->dev_uevent = hhg_lcd_uevent;

    /*Creating device*/
    if ((device_create(hhg_class, NULL, hhg_dev, NULL, HHG_DRIVER_NAME)) == NULL)
    {
        pr_err("cannot create the Device \n");
        goto r_device;
    }

    // if(gpio_rw > -1)
    // {
    //     pr_info("read mode enabled");
    //     enable_read_mode = true;
    // }

    // if(
    //     gpio_db0 > -1
    //     && gpio_db1 > -1
    //     && gpio_db2 > -1
    //     && gpio_db3 > -1
    //     && gpio_db4 > -1
    //     && gpio_db5 > -1
    //     && gpio_db6 > -1
    //     && gpio_db7 > -1
    //   )
    // {
    //     data_mode_8_bit = true;
    // }
    // else  if(
    //     gpio_db0 == -1
    //     && gpio_db1 == -1
    //     && gpio_db2 == -1
    //     && gpio_db3 == -1
    //     && gpio_db4 > -1
    //     && gpio_db5 > -1
    //     && gpio_db6 > -1
    //     && gpio_db7 > -1
    //   )
    // {
    //     data_mode_8_bit = false;
    // }
    // else
    // {
    //     pr_err("number of configured pins not suitable for 4-bit or 8-bit configuration");
    //     return -EINVAL;
    // }

    // if(gpio_rs == -1)
    // {
    //     pr_err("GPIO RS mandatory");
    //     return -EINVAL;
    // }

    // if(gpio_en == -1)
    // {
    //     pr_err("GPIO RS mandatory");
    //     return -EINVAL;
    // }

    // if(data_mode_8_bit)
    // {
    //     //init 8 bit
    //     if(hhg_lcd_init_8_bit())
    //     {
    //         pr_err("Init 4 bit data error");
    //         return -ENOEXEC;
    //     }
    // }
    // else
    // {
    //     //init 4 bit
    //     if(hhg_lcd_init_4_bit())
    //     {
    //         pr_err("Init 4 bit data error");
    //         return -ENOEXEC;
    //     }
    // }

    return 0;

r_device:
    device_destroy(hhg_class, hhg_dev);
r_class:
    class_destroy(hhg_class);
r_dev:
    cdev_del(&hhg_cdev);
r_unreg:
    unregister_chrdev_region(hhg_dev, 1);
    return -ENXIO;
}
module_init(hhg_lcd_init);

/*
* Module exit function
*/
static void __exit hhg_lcd_exit(void)
{

    // gpio_free(gpio_rs);
    // if(gpio_rw > -1)
    // {
    //     gpio_free(gpio_rw);
    // }
    // gpio_free(gpio_en);
    // if(data_mode_8_bit)
    // {
    //     gpio_free(gpio_db0);
    //     gpio_free(gpio_db1);
    //     gpio_free(gpio_db2);
    //     gpio_free(gpio_db3);
    // }
    // gpio_free(gpio_db4);
    // gpio_free(gpio_db5);
    // gpio_free(gpio_db6);
    // gpio_free(gpio_db7);

    device_destroy(hhg_class, hhg_dev);
    class_destroy(hhg_class);
    cdev_del(&hhg_cdev);
    unregister_chrdev_region(hhg_dev, 1);
    pr_info("exit");
}
module_exit(hhg_lcd_exit);

int hhg_lcd_fops_open(struct inode *inode, struct file *file)
{
    if (atomic_read(&device_busy) > 0)
    {
        pr_err("device busy");
        return -EBUSY;
    }

    atomic_inc(&device_busy);

    pr_info("Device open:%u\n", atomic_read(&device_busy));
    return 0;
}

int hhg_lcd_fops_release(struct inode *inode, struct file *file)
{
    atomic_sub(1, &device_busy);

    pr_info("Device release:%u\n", atomic_read(&device_busy));
    return 0;
}

ssize_t hhg_lcd_fops_read(struct file *filp, char __user *buff, size_t len, loff_t *off)
{
    char msg[READ_BUFF_LEN] = { [ 0 ... READ_BUFF_LEN - 1] = '\0'};

    __u32 msg_len = sprintf(msg,
                            "ciao");

    pr_info("%s", msg);

    if (len > msg_len)
    {
        len = msg_len;
    }

    return simple_read_from_buffer(buff, len, off, msg, msg_len);
}

ssize_t hhg_lcd_fops_write(struct file *filp, const char *buff, size_t len, loff_t *off)
{
    char *data = (char *)kmalloc(len, GFP_KERNEL);
    if (data == NULL)
    {
        return -ENOMEM;
    }
    memset(data, '\0', len);

    if (copy_from_user(data, buff, len) != 0)
    {
        kfree(data);
        return -EINVAL;
    }

    pr_devel("user data:%.*s", (int)len, data);

    hhg_lcd_send_str(data, len);

    kfree(data);
    return 0;
}

int hhg_lcd_uevent(struct device *dev, struct kobj_uevent_env *env)
{
    add_uevent_var(env, "DEVMODE=%#o", 0666);
    return 0;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Antonio Salsi <passy.linux@zresa.it>");
MODULE_DESCRIPTION("Driver for LCD 16x2 management with chip HITACHI HD44780U compatible.");
MODULE_INFO(intree, "Y");
MODULE_VERSION("0.80.0");