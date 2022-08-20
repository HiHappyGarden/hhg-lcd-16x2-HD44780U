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

#include "pin_config.h"
#include "constants.h"
#include "error.h"
#include "log.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Antonio Salsi <passy.linux@zresa.it>");
MODULE_DESCRIPTION("Happy GardenPi driver to get access to hardware resources");

// data
static dev_t hgd_dev = 0;
static struct class *hgd_class;
static struct cdev hgd_cdev;

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
    pr_info("Device File Opened...!!!\n");

    return 0;
}

/*
** This function will be called when we close the Device file
*/
int hgd_release(struct inode *inode, struct file *file)
{
    pr_info("Device File Closed...!!!\n");

    return 0;
}

/*
** This function will be called when we read the Device file
*/
ssize_t hgd_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{

    pr_info("Read function");

    return 0;
}

/*
** This function will be called when we write the Device file
*/
ssize_t hgd_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{

    pr_info("Write function");

    return len;
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

    pr_info("Device Driver Insert...Done!!!\n");
    return 0;

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
    device_destroy(hgd_class, hgd_dev);
    class_destroy(hgd_class);
    cdev_del(&hgd_cdev);
    unregister_chrdev_region(hgd_dev, 1);
    pr_info("Device Driver Remove...Done!!\n");
}

module_init(hgd_driver_init);
module_exit(hgd_driver_exit);