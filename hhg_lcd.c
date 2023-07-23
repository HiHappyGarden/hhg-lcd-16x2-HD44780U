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

//LCD MANAGEMENT

static bool data_mode_8_bit = false;

static short gpio_rs  = -1;
static short gpio_en  = -1;
static short gpio_db0  = -1;
static short gpio_db1  = -1;
static short gpio_db2  = -1;
static short gpio_db3  = -1;
static short gpio_db4  = -1;
static short gpio_db5  = -1;
static short gpio_db6  = -1;
static short gpio_db7  = -1;


static char msg_to_display[(HHG_ROWS * HHG_COLS) + 2] = { [ 0 ... (HHG_ROWS * HHG_COLS) + 1] = 1};

// static decl

/**
 * @brief Initializes the LCD in 8-bit mode.
 *
 * This function initializes the LCD in 8-bit mode.
 *
 * @return `true` if the initialization was successful, `false` otherwise.
 */
static bool hhg_lcd_init(void);

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
 * @brief Frees the pin used by the HHG LCD.
 *
 * This function frees the specified GPIO pin used by the HHG LCD.
 *
 * @param gpio_number The GPIO pin number to be freed.
 */
static void hhg_lcd_pin_free(u8 gpio_number);

/**
 * @brief Initializes the LCD in 4-bit mode.
 * 
 */
static void hhg_lcd_free(void);

/**
 * @brief Sets up the pin for the HHG LCD.
 *
 * This function sets up the specified GPIO pin for the HHG LCD.
 *
 * @param gpio_number The GPIO pin number to be set up.
 * @param gpio_direction The direction of the GPIO pin (input or output).
 * @return `true` if the operation was successful, `false` otherwise.
 */
static u8 hhg_lcd_pin_setup(u8 gpio_number, u8 gpio_direction);

/**
 * @brief Sends one nibble of data to the LCD.
 *
 * This function sends one nibble of data to the LCD.
 *
 * @param byte The nibble of data to be sent.
 */
static void hhg_lcd_send_nibble(u8 byte, bool rs_value);


bool hhg_lcd_init(void)
{
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
        return false;
    }

    if(gpio_rs == -1)
    {
        pr_err("GPIO RS mandatory");
        return false;
    }

    if(gpio_en == -1)
    {
        pr_err("GPIO RS mandatory");
        return false;
    }

    if(data_mode_8_bit)
    {
        //init 8 bit
        if(!hhg_lcd_init_8_bit())
        {
            pr_err("Init 4 bit data error");
            return false;
        }
    }
    else
    {
        //init 4 bit
        if(!hhg_lcd_init_4_bit())
        {
            pr_err("Init 4 bit data error");
            return false;
        }
    }

    return true;
}

bool hhg_lcd_init_8_bit(void)
{
    pr_info("init 8 bit data mode done"
    " gpio_rs:%d"
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
    " gpio_en:%d"
    " gpio_db4:%d"
    " gpio_db5:%d"
    " gpio_db6:%d"
    " gpio_db7:%d"
    , gpio_rs
    , gpio_en
    , gpio_db4
    , gpio_db5
    , gpio_db6
    , gpio_db7
    );


    hhg_lcd_pin_setup(gpio_rs, 0);
    hhg_lcd_pin_setup(gpio_en, 0);
    hhg_lcd_pin_setup(gpio_db4, 0);
    hhg_lcd_pin_setup(gpio_db5, 0);
    hhg_lcd_pin_setup(gpio_db6, 0);
    hhg_lcd_pin_setup(gpio_db7, 0);

    //for main timing see manual page 45
    //for timing and command see table 6 page 24

	usleep_range(45*1000, 55*1000);	// Wait for more than 40 ms

	hhg_lcd_send_command(0x20);		// Function set
	usleep_range(5*1000, 6*1000);   //Wait for more than 4.1 ms

	hhg_lcd_send_command(0x20);		// Function set
	usleep_range(100,200);		

	hhg_lcd_send_command(0x20);		// Instruction 0011b (Function set)
	usleep_range(100,200);		    //Wait for more than 100 μs
	
    hhg_lcd_select_line(1);

	usleep_range(41*1000,50*1000);

	hhg_lcd_set_flags(HHG_LCD_DISPLAY_OFF); 
	usleep_range(100,200);

    hhg_lcd_clear();
	usleep_range(100,200);

					/* Entry mode set */
	hhg_lcd_send_command(0x00);		// Instruction 0000b
	hhg_lcd_send_command(0x60);		/* Instruction 01(I/D)Sb -> 0110b
					   Set I/D = 1, or increment or decrement DDRAM address by 1
					   Set S = 0, or no display shift
					*/
	usleep_range(100,200);

    hhg_lcd_set_flags(HHG_LCD_DISPLAY_ON); 

    return true;
}

inline static void hhg_lcd_pin_free(u8 gpio_number)
{
    gpio_unexport(gpio_number);
    gpio_free(gpio_number);
}

static void hhg_lcd_free(void)
{
    hhg_lcd_pin_free(gpio_rs);
    hhg_lcd_pin_free(gpio_en);
    if(data_mode_8_bit)
    {
        hhg_lcd_pin_free(gpio_db0);
        hhg_lcd_pin_free(gpio_db1);
        hhg_lcd_pin_free(gpio_db2);
        hhg_lcd_pin_free(gpio_db3);
    }
    hhg_lcd_pin_free(gpio_db4);
    hhg_lcd_pin_free(gpio_db5);
    hhg_lcd_pin_free(gpio_db6);
    hhg_lcd_pin_free(gpio_db7);
}


u8 hhg_lcd_pin_setup(u8 gpio_number, u8 gpio_direction)
{
	u8 ret;

	ret = gpio_request( gpio_number, "GPIO request");
	if( ret != 0 )	{
		pr_err("failed to request GPIO %d \n", gpio_number );
		return ret;
	}	
	
	ret = gpio_export( gpio_number, 0);
	if( ret != 0 )	{
		pr_err("failed to export GPIO %d \n", gpio_number );
		return ret;
	}

	ret = gpio_direction_output( gpio_number, gpio_direction);
	if( ret != 0 )	{
		pr_err("failed to set GPIO direction %d \n", gpio_number );	
		return ret;
	}

	gpio_set_value(gpio_number, 0);

	return 0; 
}


void hhg_lcd_send_nibble(u8 byte, bool rs_value)
{
    gpio_set_value(gpio_db4, (byte >> 0 ) & 0x01);
    gpio_set_value(gpio_db5, (byte >> 1 ) & 0x01);
    gpio_set_value(gpio_db6, (byte >> 2 ) & 0x01);
    gpio_set_value(gpio_db7, (byte >> 3 ) & 0x01);

    gpio_set_value(gpio_rs, rs_value);
    usleep_range(5, 10);

	gpio_set_value(gpio_en, 1);
	usleep_range(5, 10);
	gpio_set_value(gpio_en, 0);
}

inline void hhg_lcd_send_command(u8 command)
{
    usleep_range(2000, 3000);
    hhg_lcd_send_nibble(command >> 4, HHG_COMMAND_MODE);
}


void hhg_lcd_send_char(char byte)
{
    if(data_mode_8_bit)
    {
        //TODO: 8 bit mode
    }
    else
    {
        hhg_lcd_send_nibble(byte >> 4, HHG_DATA_MODE);     // upper
        usleep_range(2000, 3000);
        hhg_lcd_send_nibble(byte & 0x0F, HHG_DATA_MODE);   // lower
    }
}
EXPORT_SYMBOL(hhg_lcd_send_char);

void hhg_lcd_send_str(const char buff[])
{
    if(!buff)
    {
        return;
    }

    u8 line = 1;

    hhg_lcd_select_line(line);

    u8 col_size = 0;
    const char* cursor = buff;
    while(*cursor != '\0' && line <= HHG_ROWS)
    {
        if(col_size >= HHG_COLS || *cursor == '\n')
        {
            line++;
            col_size = 0;
            hhg_lcd_select_line(line);
            if(*cursor == '\n')
            {
                cursor++;
            }
        }
        hhg_lcd_send_char(*cursor);
        col_size++;
        cursor++;
    }
}
EXPORT_SYMBOL(hhg_lcd_send_str);

void hhg_lcd_clear(void)
{
    hhg_lcd_send_command(0x00);		// Instruction 0000b
	hhg_lcd_send_command(0x10);		// Instruction 0001b
}
EXPORT_SYMBOL(hhg_lcd_clear);

void hhg_lcd_select_line(u8 line)
{
    u8 cmd = 0;
    switch (line)
    {
    case 1:
        cmd = 0x80;
        break;
    case 2:
        cmd = 0xC0;
        break;
    default:
        break;
    }

    if(cmd > 0)
    {
        hhg_lcd_send_command(0x20);		// Instruction 0010b (Function set)
        hhg_lcd_send_command(cmd);		/* Instruction select line */
    }
}
EXPORT_SYMBOL(hhg_lcd_select_line);

void hhg_lcd_set_flags(u8 flags)
{
    if(data_mode_8_bit)
    {
        //TODO: 8 bit mode
    }
    else
    {
        hhg_lcd_send_command(0x00); //upper
	    hhg_lcd_send_command(0x80 | ((flags & 0x07) << 4)); //lower
    }
    usleep_range(50, 100);
}
EXPORT_SYMBOL(hhg_lcd_set_flags);

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
static int __init hhg_lcd_module_init(void)
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

    if(!hhg_lcd_init())
    {
        pr_err("cannot init LCD\n");
        goto r_device;
    }

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
module_init(hhg_lcd_module_init);

/*
* Module exit function
*/
static void __exit hhg_lcd_module_exit(void)
{

    hhg_lcd_free();
    device_destroy(hhg_class, hhg_dev);
    class_destroy(hhg_class);
    cdev_del(&hhg_cdev);
    unregister_chrdev_region(hhg_dev, 1);
    pr_info("exit");
}
module_exit(hhg_lcd_module_exit);

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
    char str[] = { [ 0 ... (HHG_ROWS * HHG_COLS) + 1 ] = 0};    
    snprintf(str, sizeof(str), "%s", msg_to_display);
    return simple_read_from_buffer(buff, len, off, str, strlen(str));
}

ssize_t hhg_lcd_fops_write(struct file *filp, const char *buff, size_t len, loff_t *off)
{

    size_t bytes_to_write = 0;

    memset(msg_to_display, '\0', sizeof(msg_to_display));
    if(len > sizeof(msg_to_display) - 1)
    {
        bytes_to_write = sizeof(msg_to_display) - 1;
    }
    else
    {
        bytes_to_write = len;
    }


    ssize_t not_written = len - simple_write_to_buffer(msg_to_display, sizeof(msg_to_display) - 1, off, buff, len);
    if(not_written > 0)
    {
        pr_warn("not written all data exceed for: %ld char", not_written);
    }

    hhg_lcd_send_str(msg_to_display);

    return len;
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