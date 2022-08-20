// MIT License
//
// Copyright (c) 2022 Happy GardenPI
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/gpio.h>
#include <linux/proc_fs.h>

#include "pin_config.h"
#include "constants.h"
#include "error.h"
#include "log.h"

MODULE_LICENSE("MIT");
MODULE_AUTHOR("Antonio Salsi <passy.linux@zresa.it>");
MODULE_DESCRIPTION("Happy GardenPi driver to get access to hardware resources");

struct proc_dir_entry *Our_Proc_File;

int procfile_read(char *buffer,
                  char **buffer_location,
                  off_t offset, int buffer_length, int *eof, void *data)
{
    int ret;
    printk(KERN_INFO "procfile_read (/proc/%s) called\n", HGD_NAME);

    /*
     * We give all of our information in one go, so if the
     * user asks us if we have more information the
     * answer should always be no.
     *
     * This is important because the standard read
     * function from the library would continue to issue
     * the read system call until the kernel replies
     * that it has no more information, or until its
     * buffer is filled.
     */
    if (offset > 0)
    {
        /* we have finished to read, return 0 */
        ret = 0;
    }
    else
    {
        /* fill the buffer, return the buffer size */
        ret = sprintf(buffer, "HelloWorld!\n");
    }
    return ret;
}

static int __init hdg_init(void)
{
    // Our_Proc_File = create_proc_entry(HGD_NAME, 0644, NULL);

    // if (Our_Proc_File == NULL)
    // {
    //     proc_create_data(HGD_NAME, &proc_root);
    //     printk(KERN_ALERT "Error: Could not initialize /proc/%s\n",
    //            procfs_name);
    //     return -ENOMEM;
    // }

    // Our_Proc_File->read_proc = procfile_read;
    // Our_Proc_File->owner = THIS_MODULE;
    // Our_Proc_File->mode = S_IFREG | S_IRUGO;
    // Our_Proc_File->uid = 0;
    // Our_Proc_File->gid = 0;
    // Our_Proc_File->size = 37;

    // printk(KERN_INFO "/proc/%s created\n", procfs_name);

    hgd_error_t* error = NULL;

    hgd_error_new(&error, 10, "test ciao");

    hgd_error_print(error, "messaggio aggiuntivo", 1);


    return 0; /* everything is ok */
}

static void __exit hdg_exit(void)
{
    pr_info("Goodbye, world 2\n");
}

module_init(hdg_init);
module_exit(hdg_exit);