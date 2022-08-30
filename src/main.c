/*
 * This file is part of the Happy GardenPI distribution (https://github.com/HappyGardenPI/happy-gardenpi-driver).
 * Copyright (c) 2022 Antonio Salsi.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/uaccess.h> //copy_to/from_user()

#include "constants.h"
#include "error.h"
#include "log.h"
#include "parser.h"
#include "pin_config.h"
#include "sys_info.h"
#include "led.h"

#define READ_BUF_LEN (256) 
#define NOT_DEF (3) 

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Antonio Salsi <passy.linux@zresa.it>");
MODULE_DESCRIPTION("Happy GardenPi driver to get access to hardware resources");
MODULE_INFO(intree, "Y");

// data
static dev_t hgd_dev = 0;
static struct class *hgd_class;
static struct cdev hgd_cdev;
static struct cdev hgd_cdev;

// static atomic_t device_busy = ATOMIC_INIT(0);;

// static decl
static int __init hgd_driver_init(void);
static void __exit hgd_driver_exit(void);

// static decl
static int hgd_open(struct inode *inode, struct file *file);
static int hgd_release(struct inode *inode, struct file *file);
static ssize_t hgd_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t hgd_write(struct file *filp, const char *buf, size_t len, loff_t *off);

// File operation structure
static struct file_operations fops =
    {
        .owner = THIS_MODULE,
        .read = hgd_read,
        .write = hgd_write,
        .open = hgd_open,
        .release = hgd_release,
};

/*
** This function will be called when we open the Device file
*/
int hgd_open(struct inode *inode, struct file *file)
{
    pr_info("Device File Opened...\n");

    // if(atomic_read(&device_busy) > 0)
    // {
    //     pr_err("device busy");
    //     return -EBUSY;
    // }

    // atomic_inc(&device_busy);
    return 0;
}

/*
** This function will be called when we close the Device file
*/
int hgd_release(struct inode *inode, struct file *file)
{
    pr_info("Device File Closed...\n");
    // atomic_dec(&device_busy);
    return 0;
}

/*
** This function will be called when we read the Device file
*/
ssize_t hgd_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{ 
    char msg[READ_BUF_LEN];
    memset(msg, '\0', READ_BUF_LEN);

    sprintf(msg, "HGD_LED:\t\t%u\n" \
     "HGD_BUTTON:\t\t%u\n" \
     "HGD_LCD:\t\t\t%u\n" \
     "HGD_RELAY_1:\t\t%u\n" \
     "HGD_RELAY_2:\t\t%u\n" \
     "HGD_RELAY_3:\t\t%u\n" \
     "HGD_RELAY_4:\t\t%u\n" 
     , hgd_led_get_state()
     , NOT_DEF
     , NOT_DEF
     , NOT_DEF
     , NOT_DEF
     , NOT_DEF
     , NOT_DEF
     );
    
    pr_info("%s", msg);

    return copy_to_user( buf, msg, strlen(msg));
}

/*
** This function will be called when we write the Device file
*/
ssize_t hgd_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{

    char* params = (char*)kmalloc(len, GFP_KERNEL);
    if (params == NULL) {
        return -ENOMEM;
    }
    memset(params, '\0', len);
    if(len > HDG_PARSER_BUF_MAX)
    {
        kfree(params);
        return -EINVAL;
    }
    if(copy_from_user(params, buf, len) != 0)
    {
        kfree(params);
        return -EINVAL;
    }

    hgd_parser_t parsed;
    if (!hgd_parser_params(params, len, &parsed))
    {
        kfree(params);
        return -EINVAL;
    }

    switch (parsed.type)
    {
    case HGD_LED:
        /* code */
        hgd_led_set_state(parsed.status);
        return 1;
    case HGD_BUTTON:
        /* code */

        return 1;
    case HGD_LCD:
        /* code */

        return strlen(parsed.buff);
    case HGD_RELAY_1:
        /* code */

        return 1;
    case HGD_RELAY_2:
        /* code */

        return 1;
    case HGD_RELAY_3:
        /* code */

        return 1;
    case HGD_RELAY_4:
        /* code */

        return 1;
    default:
        pr_info("--1.5");
        kfree(params);
        return -EINVAL;
    }

    kfree(params);
    return -EINVAL;
}

/*
** Module Init function
*/
int __init hgd_driver_init(void)
{

    /*Allocating Major number*/
    if ((alloc_chrdev_region(&hgd_dev, 0, 1, "hgd_Dev")) < 0)
    {
        pr_err("Cannot allocate major number\n");
        goto r_unreg;
    }
    pr_info("Major = %d Minor = %d \n", MAJOR(hgd_dev), MINOR(hgd_dev));

    /*Creating cdev structure*/
    cdev_init(&hgd_cdev, &fops);

    /*Adding character device to the system*/
    if ((cdev_add(&hgd_cdev, hgd_dev, 1)) < 0)
    {
        pr_err("Cannot add the device to the system\n");
        goto r_del;
    }

    /*Creating struct class*/
    if ((hgd_class = class_create(THIS_MODULE, "hgd_class")) == NULL)
    {
        pr_err("Cannot create the struct class\n");
        goto r_class;
    }

    /*Creating device*/
    if ((device_create(hgd_class, NULL, hgd_dev, NULL, HGD_NAME)) == NULL)
    {
        pr_err("Cannot create the Device \n");
        goto r_device;
    }

    // load pin config
    hgd_error_t *error = NULL;

    // if(!hgd_sys_info_init(&error))
    // {
    //     hgd_error_print(error, "Cannot init sysfs", true);
    //     goto r_sys_info;
    // }

    if (!hgd_pin_config_init(&error))
    {
        hgd_error_print(error, "Cannot init gpio config", true);
        goto r_pin_config;
    }

    pr_info("Happy GarderPI driver insert...Done\n");
    return 0;

r_pin_config:
    hgd_pin_config_free();
// r_sys_info:
//     hgd_sys_info_free();
r_device:
    device_destroy(hgd_class, hgd_dev);
r_class:
    class_destroy(hgd_class);
r_del:
    cdev_del(&hgd_cdev);
r_unreg:
    unregister_chrdev_region(hgd_dev, 1);

    return -1;
}

/*
** Module exit function
*/
static void __exit hgd_driver_exit(void)
{
    hgd_pin_config_unexport();
    hgd_pin_config_free();

    hgd_sys_info_free();

    device_destroy(hgd_class, hgd_dev);
    class_destroy(hgd_class);
    cdev_del(&hgd_cdev);
    unregister_chrdev_region(hgd_dev, 1);
    pr_info("Happy GarderPI driver remove... done");
}

module_init(hgd_driver_init);
module_exit(hgd_driver_exit);
