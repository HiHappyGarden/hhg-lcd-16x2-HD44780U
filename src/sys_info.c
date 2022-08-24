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

#include "sys_info.h"
#include "relay.h"

#include <linux/vmalloc.h>
#include <linux/init.h>

#define _HDG_SYS_INFO_BUFFER_MAX 64

#define HGD_CREATE_FILE(_attr_) \
    if (sysfs_create_file(kobj_ref, &_attr_.attr)) \
    { \
        hgd_error_new(error, HGD_ERROR_SYS_INFO_SYSFS_CREATE, "Cannot create attribute " #_attr_); \
        goto r_sysfs; \
    } 



static volatile __u8 led = 0;
static volatile __u8 button = 0;
static volatile char lcd[_HDG_SYS_INFO_BUFFER_MAX];
static volatile __u8 releay = 0;

static struct kobject *kobj_ref;

ssize_t hgd_sys_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
// ssize_t hgd_sys_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);

static struct kobj_attribute led_attr = __ATTR(led, 0660, hgd_sys_show, NULL);
static struct kobj_attribute button_attr = __ATTR(button, 0660, hgd_sys_show, NULL);
static struct kobj_attribute lcd_attr = __ATTR(lcd, 0660, hgd_sys_show, NULL);
static struct kobj_attribute relay_attr = __ATTR(relay, 0660, hgd_sys_show, NULL);



bool hgd_sys_info_init(hgd_error_t **error)
{

    memset((void*)lcd, '\0', _HDG_SYS_INFO_BUFFER_MAX);

    /*Creating a directory in /sys/kernel/ */
    kobj_ref = kobject_create_and_add(HGD_NAME, kernel_kobj);

    /*Creating sysfs file for etx_value*/
    pr_info("Creating sysfs file for " HGD_NAME);
    HGD_CREATE_FILE(led_attr);
    HGD_CREATE_FILE(button_attr);
    HGD_CREATE_FILE(lcd_attr);
    HGD_CREATE_FILE(relay_attr);
    pr_info("Sysfs... done");

    return true;

r_sysfs:
    hgd_sys_info_free();
    return false;
}

ssize_t hgd_sys_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    pr_info("Sysfs - Read!!!\n");
    return sprintf(buf, "%d", led);
}

inline void hgd_sys_info_set_led(__u8 status)
{
    led = status;
}

inline void hgd_sys_info_set_button(__u8 status)
{

}

inline void hgd_sys_info_set_lcd(const char *status)
{

}

inline void hgd_sys_info_set_releay(hgd_relay_t status)
{

}

// ssize_t hgd_sys_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
// {
//     pr_info("Sysfs - Write!!!\n");
//     sscanf(buf, "%d", &led);
//     return count;
// }

void hgd_sys_info_free(void)
{
    sysfs_remove_file(kernel_kobj, &led_attr.attr);
    sysfs_remove_file(kernel_kobj, &button_attr.attr);
    sysfs_remove_file(kernel_kobj, &lcd_attr.attr);
    sysfs_remove_file(kernel_kobj, &relay_attr.attr);
}
