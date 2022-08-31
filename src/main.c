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
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/uaccess.h> //copy_to/from_user()

#include "constants.h"
#include "error.h"
#include "parser.h"
#include "pin_config.h"
#include "led.h"
#include "relay.h"

#ifdef pr_fmt
#undef pr_fmt
#define pr_fmt(fmt) HGD_NAME ": " fmt
#endif

#define READ_BUF_LEN (256)
#define NOT_DEF (3)

// data
static dev_t hgd_dev = 0;
static struct class *hgd_class;
static struct cdev hgd_cdev;
static struct cdev hgd_cdev;
static atomic_t device_busy = ATOMIC_INIT(0);

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
        .release = hgd_release
};

/*
** This function will be called when we open the Device file
*/
int hgd_open(struct inode *inode, struct file *file)
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

/*
** This function will be called when we close the Device file
*/
int hgd_release(struct inode *inode, struct file *file)
{
    atomic_sub(1, &device_busy);

    pr_info("Device release:%u\n", atomic_read(&device_busy));
    return 0;
}

/*
** This function will be called when we read the Device file
*/
ssize_t hgd_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    char msg[READ_BUF_LEN];
    memset(msg, '\0', READ_BUF_LEN);

    sprintf(msg, "\nHGD_LED:\t\t\t%u\n"
                 "HGD_BUTTON:\t\t%u\n"
                 "HGD_LCD:\t\t\t%u\n"
                 "HGD_RELAY_1:\t\t%u\n"
                 "HGD_RELAY_2:\t\t%u\n"
                 "HGD_RELAY_3:\t\t%u\n"
                 "HGD_RELAY_4:\t\t%u\n",
            hgd_led_get_state(), NOT_DEF, NOT_DEF, hgd_relay_get_state(HGD_RELAY_1), hgd_relay_get_state(HGD_RELAY_2), hgd_relay_get_state(HGD_RELAY_3), hgd_relay_get_state(HGD_RELAY_4));

    pr_info("%s", msg);

    return copy_to_user(buf, msg, strlen(msg));
}

/*
** This function will be called when we write the Device file
*/
ssize_t hgd_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
    len--;
    if (len > HDG_PARSER_BUF_MAX)
    {
        return -EINVAL;
    }

    char *params = (char *)kmalloc(len, GFP_KERNEL);
    if (params == NULL)
    {
        return -ENOMEM;
    }
    memset(params, '\0', len);

    if (copy_from_user(params, buf, len) != 0)
    {
        kfree(params);
        return -EINVAL;
    }

    kfree(params);

    hgd_parser_t parsed;
    if (!hgd_parser_params(params, len, &parsed))
    {
        return 1;
    }

    switch (parsed.type)
    {
    case HGD_LED:
        pr_info("HGD_LED: %u\n", parsed.status);
        hgd_led_set_state(parsed.status);
        return len;
    case HGD_BUTTON:
        return len;
    case HGD_LCD:
        return strlen(parsed.buff);
    case HGD_RELAY_1:
    case HGD_RELAY_2:
    case HGD_RELAY_3:
    case HGD_RELAY_4:
        pr_info("HGD_RELAY_%u: %u\n", parsed.type, parsed.status);
        hgd_relay_set_state(parsed.type, parsed.status);
        return len;
    }

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
    if (!hgd_pin_config_init(&error))
    {
        hgd_error_print(error, "Cannot init gpio config", true);
        goto r_pin_config;
    }

    hgd_led_init();
    hgd_relay_init();

    pr_info("Happy GarderPI driver insert...Done\n");
    return 0;

r_pin_config:
    hgd_pin_config_free();
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

    device_destroy(hgd_class, hgd_dev);
    class_destroy(hgd_class);
    cdev_del(&hgd_cdev);
    unregister_chrdev_region(hgd_dev, 1);
    pr_info("Happy GarderPI driver remove... done");
}

module_init(hgd_driver_init);
module_exit(hgd_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Antonio Salsi <passy.linux@zresa.it>");
MODULE_DESCRIPTION("Happy GardenPi driver to get access to hardware resources");
MODULE_INFO(intree, "Y");
