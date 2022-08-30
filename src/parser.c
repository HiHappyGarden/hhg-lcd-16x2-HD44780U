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

#include "parser.h"

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>


#define HGD_STD_FILL_PARSER(_const) \
        parser->type = _const; \
        long status; \
        long ret = kstrtol (args[1], 10, &status); \
        if(ret == -ERANGE || ret == -EINVAL ) \
        { \
            return false; \
        } \
        parser->status = status; \
        return true; 

bool hgd_parser_params(const char* buf, size_t len, hgd_parser_t* parser)
{
    if (!parser || !buf)
    {
        return false;
    }

    memset(parser, 0, sizeof(hgd_parser_t));

    char args[2][HDG_PARSER_BUF_MAX] = {
        [0] = {0},
        [1] = {0},
    };
    
    __u8 no_buffer_overflow = 0;
    bool check = true;
    char* args0_ptr = args[0];
    char* args1_ptr = args[1];
    const char* ptr_after_arg0 = buf; 
    for(__u8 i = 0; i < len; i++)
    {
        if(buf[i] == ' ')
        {
            if(check == false && strlen(args[1]))
            {
                break;
            }
            else
            {
                no_buffer_overflow = 0;
                check = false;
                ptr_after_arg0++;
                continue;
            }
            
        }
        else
        {
            if(check)
            {
                if(no_buffer_overflow >= HDG_PARSER_BUF_MAX)
                {
                    return false;
                }
                *args0_ptr = buf[i];
                args0_ptr++;
                ptr_after_arg0++;
                no_buffer_overflow++;
            } 
            else
            {
                if(no_buffer_overflow >= HDG_PARSER_BUF_MAX)
                {
                    return false;
                }
                *args1_ptr = buf[i];
                args1_ptr++;
                no_buffer_overflow++;
            }
        }

        
    }

    
    pr_info("---1 >%s< >%s<", args[0], args[1]);
    if(strncmp(args[0], HGD_TO_STR(HGD_LED), HDG_PARSER_BUF_MAX) == 0)
    {
        HGD_STD_FILL_PARSER(HGD_LED)
    }
    else if(strncmp(args[0], HGD_TO_STR(HGD_BUTTON), HDG_PARSER_BUF_MAX) == 0)
    {
        HGD_STD_FILL_PARSER(HGD_BUTTON)
    }
    if(strncmp(args[0], HGD_TO_STR(HGD_LCD), HDG_PARSER_BUF_MAX) == 0)
    {
        parser->type = HGD_LCD;
        strncpy(parser->buff, ptr_after_arg0,  strlen(ptr_after_arg0));
        return true;
    }
    else if(strncmp(args[0], HGD_TO_STR(HGD_RELAY_1), HDG_PARSER_BUF_MAX) == 0)
    {
        HGD_STD_FILL_PARSER(HGD_RELAY_1)   
    }
    else if(strncmp(args[0], HGD_TO_STR(HGD_RELAY_2), HDG_PARSER_BUF_MAX) == 0)
    {
        HGD_STD_FILL_PARSER(HGD_RELAY_2)
    }
    else if(strncmp(args[0], HGD_TO_STR(HGD_RELAY_3), HDG_PARSER_BUF_MAX) == 0)
    {
        HGD_STD_FILL_PARSER(HGD_RELAY_3)
    }
    else if(strncmp(args[0], HGD_TO_STR(HGD_RELAY_4), HDG_PARSER_BUF_MAX) == 0)
    {
        HGD_STD_FILL_PARSER(HGD_RELAY_4)
    }
    return false;
}
