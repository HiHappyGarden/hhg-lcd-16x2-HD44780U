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



#ifndef _HDG_SYS_INFO_
#define _HDG_SYS_INFO_

#include "error.h"

/**
 * @brief Init pin configuration
 * 
 * @param[out] error if pass NULL not return erro 
 * @return 0 init correctly
 * @return true some error
 */
bool hgs_sys_info_init(hgd_error_t** error);

/**
 * @brief Free pin config
 * 
 */
void hgd_sys_info_free(void);

#endif
