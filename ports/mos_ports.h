/*
    <Portable definitions for target hardware platform.>
    Copyright (C) <2024>  <LL2524460@qq.com, Phone: 139-8424-5508>

    This file is part of mos(micro-kernel operation system) is free software: you can redistribute it and/or modify
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

#ifndef __MOS_TYPES_H__
#define __MOS_TYPES_H__

typedef signed char         mos_s8_t;
typedef signed short        mos_s16_t;
typedef signed int          mos_s32_t;
typedef signed long long    mos_s64_t;

typedef unsigned char       mos_u8_t;
typedef unsigned short      mos_u16_t;
typedef unsigned int        mos_u32_t;
typedef unsigned long long  mos_u64_t;

typedef float               mos_float32_t;
typedef double              mos_float64_t;    

#define MOS_EN_IRQ()        (__enable_irq())
#define MOS_DIS_IRQ()       (__disable_irq())

#define IS_IN_INTTERUPT()   (__get_IPSR() == 0 ? 0 : 1)

#endif