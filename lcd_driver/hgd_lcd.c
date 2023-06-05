/***************************************************************************
 * 
 * Happy GardenPI Driver
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

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <linux/cdev.h>

#ifdef pr_fmt
#undef pr_fmt
#define pr_fmt(fmt) "hgd_lcd: " fmt
#endif

#define HGD_IS_VALID(gpio, msg) \
if (!gpio_is_valid(gpio)) \
{ \
    pr_err("GPIO " msg " wrong pin:%d", gpio); \
    return -EINVAL; \
} 

#define HGD_DRIVER_NAME "hgd_lcd"

#define READ_BUFF_LEN (256)

// data
static dev_t hgd_dev = 0;
static struct class *hgd_class;
static struct cdev hgd_cdev;
static atomic_t device_busy = ATOMIC_INIT(0);

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


// static decl
/**
 * Init LCD 8 bit
*/
static bool hgd_lcd_init_8_bit(void);

/**
 * Init LCD 4 bit
*/
static bool hgd_lcd_init_4_bit(void);

/**
 * Write 4 bit
*/
static void hgd_lcd_write_nibble(__u8 nibble);

/**
 * Send 1 byte to lcd
*/
static void hgd_lcd_send_byte(__u8 byte, __u8 mode);

/**
 * Send command to lcd
*/
static void hgd_lcd_send_command(__u8 command);


static int hgd_lcd_open(struct inode *inode, struct file *file);
static int hgd_lcd_release(struct inode *inode, struct file *file);
static ssize_t hgd_lcd_read(struct file *filp, char __user *buff, size_t len, loff_t *off);
static ssize_t hgd_lcd_write(struct file *filp, const char *buff, size_t len, loff_t *off);

/**
 * @brief Change access permission in user space
 */
static int hgd_lcd_uevent(struct device *dev, struct kobj_uevent_env *env);

// File operation structure
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = hgd_lcd_read,
    .write = hgd_lcd_write,
    .open = hgd_lcd_open,
    // .unlocked_ioctl = hgd_button_ioctl,
    .release = hgd_lcd_release
};

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


void hgd_lcd_send_data(__u8 data)
{
    hgd_lcd_send_byte(data, 1);
}
EXPORT_SYMBOL(hgd_lcd_send_data);

void hgd_lcd_send_str(const char* str, __u16 len)
{
    for(__u16 i = 0; i < len; i++)
    {
        hgd_lcd_send_byte(str[i], 1);
    }
}
EXPORT_SYMBOL(hgd_lcd_send_str);

/*
* Module Init function
*/
static int __init hgd_lcd_init(void)
{
    /*Allocating Major number*/
    if ((alloc_chrdev_region(&hgd_dev, 0, 1, "hgd_Dev")) < 0)
    {
        pr_err("cannot allocate major number\n");
        goto r_unreg;
    }
    pr_info("Major = %d Minor = %d \n", MAJOR(hgd_dev), MINOR(hgd_dev));

    /*Creating cdev structure*/
    cdev_init(&hgd_cdev, &fops);

    /*Adding character device to the system*/
    if ((cdev_add(&hgd_cdev, hgd_dev, 1)) < 0)
    {
        pr_err("cannot add the device to the system\n");
        goto r_dev;
    }

    /*Creating struct class*/
    if ((hgd_class = class_create(THIS_MODULE, "hgd_class")) == NULL)
    {
        pr_err("cannot create the struct class\n");
        goto r_class;
    }
    hgd_class->dev_uevent = hgd_lcd_uevent;

    /*Creating device*/
    if ((device_create(hgd_class, NULL, hgd_dev, NULL, HGD_DRIVER_NAME)) == NULL)
    {
        pr_err("cannot create the Device \n");
        goto r_device;
    }

    if(gpio_rw > -1)
    {
        pr_info("read mode enabled");
        enable_read_mode = true;
    }

    if(
        gpio_db0 > -1
        && gpio_db1 > -1
        && gpio_db2 > -1
        && gpio_db3 > -1
        && gpio_db4 > -1
        && gpio_db5 > -1
        && gpio_db6 > -1
        && gpio_db7 > -1
      )
    {
        data_mode_8_bit = true;
    }
    else  if(
        gpio_db0 == -1
        && gpio_db1 == -1
        && gpio_db2 == -1
        && gpio_db3 == -1
        && gpio_db4 > -1
        && gpio_db5 > -1
        && gpio_db6 > -1
        && gpio_db7 > -1
      )
    {
        data_mode_8_bit = false;
    }
    else
    {
        pr_err("number of configured pins not suitable for 4-bit or 8-bit configuration");
        return -EINVAL;
    }

    if(gpio_rs == -1)
    {
        pr_err("GPIO RS mandatory");
        return -EINVAL;
    }

    if(gpio_en == -1)
    {
        pr_err("GPIO RS mandatory");
        return -EINVAL;
    }

    if(data_mode_8_bit)
    {
        //init 8 bit
        if(hgd_lcd_init_8_bit())
        {
            pr_err("Init 4 bit data error");
            return -ENOEXEC;
        }
    }
    else
    {
        //init 4 bit
        if(hgd_lcd_init_4_bit())
        {
            pr_err("Init 4 bit data error");
            return -ENOEXEC;
        }
    }


    hgd_lcd_send_data('C');
    hgd_lcd_send_data('i');
    hgd_lcd_send_data('a');
    hgd_lcd_send_data('o');
    return 0;

r_device:
    device_destroy(hgd_class, hgd_dev);
r_class:
    class_destroy(hgd_class);
r_dev:
    cdev_del(&hgd_cdev);
r_unreg:
    unregister_chrdev_region(hgd_dev, 1);
    return -ENXIO;
}
module_init(hgd_lcd_init);

/*
* Module exit function
*/
static void __exit hgd_lcd_exit(void)
{

    gpio_free(gpio_rs);
    if(gpio_rw > -1)
    {
        gpio_free(gpio_rw);
    }
    gpio_free(gpio_en);
    if(data_mode_8_bit)
    {
        gpio_free(gpio_db0);
        gpio_free(gpio_db1);
        gpio_free(gpio_db2);
        gpio_free(gpio_db3);
    }
    gpio_free(gpio_db4);
    gpio_free(gpio_db5);
    gpio_free(gpio_db6);
    gpio_free(gpio_db7);
    pr_info("exit");
}
module_exit(hgd_lcd_exit);

bool hgd_lcd_init_8_bit(void)
{
    pr_info("init 8 bit data mode done gpio_rs:%d gpio_rw:%d gpio_en:%d gpio_db0:%d gpio_db1:%d gpio_db2:%d gpio_db3:%d gpio_db4:%d gpio_db5:%d gpio_db6:%d gpio_db7:%d"
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

bool hgd_lcd_init_4_bit(void)
{

    pr_info("init 4 bit data mode done gpio_rs:%d gpio_rw:%d gpio_en:%d gpio_db4:%d gpio_db5:%d gpio_db6:%d gpio_db7:%d"
        , gpio_rs
        , gpio_rw
        , gpio_en
        , gpio_db4
        , gpio_db5
        , gpio_db6
        , gpio_db7
    );

    HGD_IS_VALID(gpio_rs, "RS")
    if(gpio_rw > -1)
    {
        HGD_IS_VALID(gpio_rw, "RW")
    }
    HGD_IS_VALID(gpio_en, "EN")
    HGD_IS_VALID(gpio_db4, "DB4")
    HGD_IS_VALID(gpio_db5, "DB5")
    HGD_IS_VALID(gpio_db6, "DB6")
    HGD_IS_VALID(gpio_db7, "DB7")
    
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

    msleep(15);
    hgd_lcd_write_nibble(0x03);
    msleep(5);
    hgd_lcd_write_nibble(0x03);
    msleep(1);
    hgd_lcd_write_nibble(0x03);
    msleep(1);
    hgd_lcd_write_nibble(0x02);

    hgd_lcd_send_command(0x28); // 4-bit mode, 2 lines, 5x8 font
    hgd_lcd_send_command(0x0C); // Display on, cursor off
    hgd_lcd_send_command(0x06); // Entry mode: increment cursor, no shift
    hgd_lcd_send_command(0x01); // Clear display

    return 0;
}


void hgd_lcd_write_nibble(__u8 nibble)
{
    gpio_set_value(gpio_db4, nibble & 0x01);
    gpio_set_value(gpio_db5, nibble & 0x02);
    gpio_set_value(gpio_db6, nibble & 0x04);
    gpio_set_value(gpio_db7, nibble & 0x08);
    msleep(1);

    gpio_set_value(gpio_en, 1);
    msleep(1);
    gpio_set_value(gpio_en, 0);
    msleep(1);
}


static void hgd_lcd_send_byte(__u8 byte, __u8 mode)
{
    gpio_set_value(gpio_rs, mode);

    hgd_lcd_write_nibble(byte >> 4);
    hgd_lcd_write_nibble(byte & 0x0F);

    msleep(1);
}

static void hgd_lcd_send_command(__u8 command)
{
    hgd_lcd_send_byte(command, 0);
}

int hgd_lcd_open(struct inode *inode, struct file *file)
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

int hgd_lcd_release(struct inode *inode, struct file *file)
{
    atomic_sub(1, &device_busy);

    pr_info("Device release:%u\n", atomic_read(&device_busy));
    return 0;
}

ssize_t hgd_lcd_read(struct file *filp, char __user *buff, size_t len, loff_t *off)
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

ssize_t hgd_lcd_write(struct file *filp, const char *buff, size_t len, loff_t *off)
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

    hgd_lcd_send_str(data, len);

    kfree(data);
    return 0;
}

int hgd_lcd_uevent(struct device *dev, struct kobj_uevent_env *env)
{
    add_uevent_var(env, "DEVMODE=%#o", 0666);
    return 0;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Antonio Salsi <passy.linux@zresa.it>");
MODULE_DESCRIPTION("Happy GardenPi driver to get access to hardware resources");
MODULE_INFO(intree, "Y");
MODULE_VERSION("0.80.0");