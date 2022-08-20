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

#include "error.h"
#include "log.h"

#include <linux/kernel.h>
#include <linux/vmalloc.h>
#include <linux/string.h>
#include <linux/stddef.h>



static hgd_error_t *last_error = NULL;


_Bool hgd_error_new(hgd_error_t** error, hgd_error_code_t code, const char* msg)
{
    if(error == NULL)
    {
        last_error = NULL;
        return 0;
    }
    *error = vmalloc(sizeof(hgd_error_t));
    if(!*error)
    {
        last_error = NULL;
        return 0;
    }
    (*error)->code = code;

    strncpy((*error)->msg, msg, MSG_LEN_ERROR_HGD);
    last_error = *error;
    return 1;
}

void hgd_error_print(hgd_error_t* error, const char* msg, _Bool free)
{
    if(error == NULL)
    {
        return;
    }

    
    if(msg)
    {
        static const char fmt[] = "%s: %s code=%u msg=%s";
        size_t len = strlen(msg) + strlen(fmt) + strlen(HGD_NAME);
        char* to_print = vmalloc(len);
        if(!to_print)
        {
            return;
        }
        memzero_explicit(to_print, len);

        sprintf(to_print, fmt, HGD_NAME, msg, error->code, error->msg);

        pr_info("%s", to_print);

        vfree(to_print);
    }
    else
    {
        pr_info("error code=%u msg=%s", error->code, error->msg);
    }

    if(free)
    {
        hgd_error_free(&error);
    }
}

const hgd_error_t * hgd_erro_get_last(void)
{
    return last_error;
}

void hgd_error_free(hgd_error_t** error)
{
    if(*error == NULL)
    {
        return;
    }

    if(last_error == *error)
    {
        last_error = NULL;
    }

    *error = NULL;
    vfree(*error);
}

