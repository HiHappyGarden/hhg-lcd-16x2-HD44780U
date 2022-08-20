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

#include <linux/vmalloc.h>
#include <linux/string.h>

#include <stddef.h>



hgd_error_t *hgd_last_error = NULL;


_Bool hgd_new_error(hgd_error_t** error, hgd_error_code_t code, const char* msg)
{
    if(error == NULL)
    {
        hgd_last_error = NULL;
        return 0;
    }
    *error = vmalloc(sizeof(hgd_error_t));
    if(!*error)
    {
        hgd_last_error = NULL;
        return 0;
    }
    (*error)->code = code;
    strncpy((*error)->msg, msg, MSG_LEN_ERROR_HGD);
    hgd_last_error = *error;
    return 1;
}

void hgd_free(hgd_error_t** error)
{
    if(*error == NULL)
    {
        return;
    }

    if(hgd_last_error == *error)
    {
        hgd_last_error = NULL;
    }
    
    *error = NULL;
    vfree(*error);
}