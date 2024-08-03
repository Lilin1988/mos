/*
    <Thread safe queue operation of mos(medical operation system) to support common queue functions.>
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

#include "mos_queue.h"
#include "mos_kernel.h"

mos_s32_t mos_queue_init(mos_queue_t *queue, void *buffer, mos_s32_t length)
{
    mos_s32_t ret = -1;

    mos_enter_critial();
    {
        if(queue == MOS_NULL_PTR || buffer == MOS_NULL_PTR || length <= 0)
        {
            ret = -1;
        }
        else
        {            
            queue->valid_length = 0;
            queue->queue_length = length;  
            queue->buffer = (mos_u8_t*)buffer;          
            queue->pointer_read = queue->buffer;
            queue->pointer_write = queue->buffer;
            ret = 0;
        }
    }
    mos_exit_critial();

    return ret;
}

mos_s32_t mos_queue_pop(mos_queue_t *queue, void *buffer, mos_s32_t size)
{
    mos_s32_t ret = -1;
    mos_s32_t index = 0;
    mos_u8_t *ptr_buffer = 0;

    mos_enter_critial();
    {
        if(queue == MOS_NULL_PTR || buffer == MOS_NULL_PTR || size <= 0)
        {
            ret = -1;
        }
        else
        {
            if(queue->valid_length < size)
            {
                ret = -1;
            }
            else
            {
                ptr_buffer = (mos_u8_t*)buffer;                
                for(index = 0; index < size; index++)
                {
                    *ptr_buffer++ = *queue->pointer_read++;
                    queue->valid_length--;

                    if(queue->pointer_read >= queue->buffer + queue->queue_length)
                    {
                        queue->pointer_read = queue->buffer;
                    }
                }

                ret = 0;
            }
        }
    }
    mos_exit_critial();

    return ret;   
}

mos_s32_t mos_queue_push(mos_queue_t *queue, void *buffer, mos_s32_t size)
{
    mos_s32_t ret = -1;
    mos_s32_t index = 0;
    mos_u8_t *ptr_buffer = 0;

    mos_enter_critial();
    {
        if(queue == MOS_NULL_PTR || buffer == MOS_NULL_PTR || size <= 0)
        {
            ret = -1;
        }
        else
        {
            if(queue->queue_length - queue->valid_length < size)
            {
                ret = -1;
            }
            else
            {
                ptr_buffer = (mos_u8_t*)buffer;                
                for(index = 0; index < size; index++)
                {
                    *queue->pointer_write++ = *ptr_buffer++;
                    queue->valid_length++;

                    if(queue->pointer_write >= queue->buffer + queue->queue_length)
                    {
                        queue->pointer_write = queue->buffer;
                    }
                }

                ret = 0;
            }
        }
    }
    mos_exit_critial();

    return ret;   
}

mos_s32_t mos_queue_try_pop(mos_queue_t *queue, void *buffer, mos_s32_t size)
{
    mos_s32_t ret = -1;

    mos_enter_critial();
    {
        if(queue == MOS_NULL_PTR || buffer == MOS_NULL_PTR || size <= 0)
        {
            ret = -1;
        }
        else
        {
            if(queue->valid_length < size)
            {
                ret = -1;
            }
            else
            {
                ret = 0;
            }
        }
    }
    mos_exit_critial();

    return ret;       
}