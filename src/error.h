/* 
 * This file is part of the Happy GardenPI distribution (https://github.com/HappyGardenPI/happy-gardenpi-driver).
 * Copyright (c) 2022-23 Antonio Salsi.
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



#ifndef _HDG_ERROR_
#define _HDG_ERROR_

#include <linux/kernel.h>

#define MSG_LEN_ERROR_HGD (30)

/**
 * @brief Error code 
 */
enum hgd_error_code
{
    HGD_ERROR_NONE,
    HGD_ERROR_GPIO_NOT_VALID = 1,
    HGD_ERROR_GPIO_REQUEST,
    HGD_ERROR_GPIO_DEBOUNCE,
    HGD_ERROR_GPIO_IRQ,
    HGD_ERROR_SYS_INFO_SYSFS_CREATE,
    HGD_ERROR_SYS_INFO_SYSFS_NO_VHEAP,
};

/**
 * @brief Error structure
 */
struct hgd_error
{
    enum hgd_error_code code;          //!< Error code
    char msg[MSG_LEN_ERROR_HGD];    //!< Error message
};

/**
 * @brief Build new error
 * 
 * @param error 
 * @return 0 ok 
 * @return 0 error 
 */
bool hgd_error_new(struct hgd_error** error, enum hgd_error_code code, const char* msg);

/**
 * @brief Prinnt and free error
 * 
 * @param error to print
 * @param msg addtidional message to add if NULL not print nothing
 * @param free 0 not free
 */
void hgd_error_print(struct hgd_error* error, const char* msg, bool free);

/**
 * @brief Gel last error
 * 
 * @return const struct hgd_error* NULL il no error occured
 */
const struct hgd_error * hgd_erro_get_last(void);

/**
 * @brief Free a rerro
 * 
 * @param error to free
 */
void hgd_error_free(struct hgd_error** error);

#endif