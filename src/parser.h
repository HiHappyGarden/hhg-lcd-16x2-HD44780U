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

#ifndef _HDG_PARSER_
#define _HDG_PARSER_
#include "constants.h"

#include <linux/init.h>

#define HDG_PARSER_BUF_MAX (48)

typedef struct
{
    hgd_type_t type;
    char buff[HDG_PARSER_BUF_MAX];
    __u8 status;
} hgd_parser_t;

/**
 * @brief Parse write section
 *
 * @param buf data buffer from user
 * @param len buffer
 * @param[out] parser data parsed (already alloced)
 * @return true successful
 * @return false fail
 */
bool hgd_parser_params(const char* buf, size_t len, hgd_parser_t* parser);

#endif
