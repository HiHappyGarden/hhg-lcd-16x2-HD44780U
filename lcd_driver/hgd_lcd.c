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


static short rs = 0;
static short rw = 0;
static short e = 0;
static short db0 = 0;
static short db1 = 0;
static short db2 = 0;;
static short db3 = 0;;
static short db4 = 0;
static short db5 = 0;
static short db6 = 0;
static short db7 = 0;


module_param(rs, short, 0660);
MODULE_PARM_DESC(rs, "PIN RS - Select registers");

module_param(rw, short, 0660);
MODULE_PARM_DESC(rw, "PIN RW - Select read or write");

module_param(e, short, 0660);
MODULE_PARM_DESC(e, "PIN RW - Start data read/write");

module_param(db0, short, 0660);
MODULE_PARM_DESC(db0, "PIN DB0 - bit 0");

module_param(db1, short, 0660);
MODULE_PARM_DESC(db1, "PIN DB1 - bit 1");

module_param(db2, short, 0660);
MODULE_PARM_DESC(db2, "PIN DB2 - bit 2");

module_param(db3, short, 0660);
MODULE_PARM_DESC(db3, "PIN DB3 - bit 3");

module_param(db4, short, 0660);
MODULE_PARM_DESC(db4, "PIN DB4 - bit 4");

module_param(db5, short, 0660);
MODULE_PARM_DESC(db5, "PIN DB5 - bit 5");

module_param(db6, short, 0660);
MODULE_PARM_DESC(db6, "PIN DB6 - bit 6");

module_param(db7, short, 0660);
MODULE_PARM_DESC(db7, "PIN DB7 - bit 7");

/*
** Module Init function
*/
static int __init hgd_lcd_init(void)
{
    printk(KERN_INFO "hgd_lcd_init\n");
    return 0;
}

/*
** Module exit function
*/
static void __exit hgd_lcd_exit(void)
{

    printk(KERN_INFO "hgd_lcd_exit\n");
}

module_init(hgd_lcd_init);
module_exit(hgd_lcd_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Antonio Salsi <passy.linux@zresa.it>");
MODULE_DESCRIPTION("Happy GardenPi driver to get access to hardware resources");
MODULE_INFO(intree, "Y");
MODULE_VERSION("0.80.0");