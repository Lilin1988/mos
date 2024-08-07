/*
    <Header of mos_queue.c which is part of mos(micro-kernel operation system).>
    Copyright (C) <2024>  <LL2524460@qq.com, Phone: 139-8424-5508>

    This file is part of mos(medical operation system) is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or any later version.

    This file is part of mos(medical operation system) is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this file which is part of mos(medical operation system).  
    If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef __MOS_QUEUE_H__
#define __MOS_QUEUE_H__

#include "mos_ports.h"

typedef struct
{
    mos_u8_t *buffer;
    mos_s32_t queue_length;
    mos_s32_t valid_length;   
    mos_u8_t *pointer_read;
    mos_u8_t *pointer_write;
}mos_queue_t;

#endif