/*
    <Kernel of mos(medical operation system) to support all kernel and task related operation.>
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

#include "mos.h"
#include "mos_queue.h"
#include "mos_kernel.h"

typedef struct
{
    mos_task_id_t sender;
    mos_event_id_t event;    
}mos_evt_t;

typedef struct
{
    mos_queue_t events; 
    mos_task_pri_t priority;        
    mos_task_init_handle_t init_handle;
    mos_task_event_handle_t event_handle;
    mos_evt_t events_buffer[MOS_MAX_EVENT];
}mos_task_ctl_t;

static mos_s32_t irq_nest_count = 0;
static mos_task_id_t task_id_count = 0;
static mos_task_ctl_t mos_task_controller[MOS_MAX_TASK] = { 0 };

void mos_enter_critial(void)
{
    MOS_EN_IRQ();
    irq_nest_count++;
}

void mos_exit_critial(void)
{
    MOS_EN_IRQ();
    {
        if(--irq_nest_count <= 0)
        {
            irq_nest_count = 0;
            MOS_DIS_IRQ();
        }
    }
}

mos_s32_t mos_kernel_init(void)
{
    mos_s32_t ret = 0;
    mos_s32_t index = 0;

    mos_enter_critial();
    {
        task_id_count = 0;

        for(index = 0; index < MOS_MAX_TASK; index++)
        {
            mos_task_controller[index].priority = 0;
            mos_task_controller[index].init_handle = MOS_NULL_PTR;
            mos_task_controller[index].event_handle = MOS_NULL_PTR;

            ret = mos_queue_init(&mos_task_controller[index].events, 
                                 mos_task_controller[index].events_buffer,
                                 sizeof(mos_evt_t) * MOS_MAX_EVENT);

            if(ret < 0)
            {
                break;
            }
        }
    }
    mos_exit_critial();

    return ret;
}

mos_s32_t mos_kernel_run(void)
{
    mos_s32_t ret = 0;
    mos_s32_t index = 0;    
    mos_s32_t task_count = 0;
    mos_task_id_t task_id = 0;
    mos_evt_t task_event = { 0 };    
    mos_task_pri_t task_priority = 0;
    mos_task_event_handle_t event_handle = MOS_NULL_PTR;

    mos_enter_critial();
    {
        task_count = task_id_count;
    }
    mos_exit_critial();

    if(task_count <= 0)
    {
        ret = -1;
    }
    else
    {
        while(1)
        {
            task_id = -1;
            task_priority = 0;

            mos_enter_critial();
            {
                for(index = 0; index < MOS_MAX_TASK; index++)
                {
                    if(mos_queue_try_pop(&mos_task_controller[index].events, &task_event, sizeof(mos_evt_t)) == 0)
                    {
                        if(mos_task_controller[index].priority >= task_priority)
                        {
                            task_id = index;
                            task_priority = mos_task_controller[index].priority;
                        }
                    }
                }
            }
            mos_exit_critial();

			
            if(task_id >= 0)
            {
                event_handle = MOS_NULL_PTR;
                mos_enter_critial();
                {
                    if(mos_queue_pop(&mos_task_controller[task_id].events, &task_event, sizeof(mos_evt_t)) == 0)
                    {
                        event_handle = mos_task_controller[task_id].event_handle;
                    }
                }
                mos_exit_critial();
				
				if(event_handle != MOS_NULL_PTR)
				{
					event_handle(task_event.sender, task_event.event);
				}				
            }
        }
    }

    return ret;
}


mos_task_id_t mos_kernel_task_create(mos_task_t task)
{
    mos_task_id_t ret = 0;    
    mos_task_init_handle_t init_handle = MOS_NULL_PTR;

    mos_enter_critial();
    {
        if(task_id_count >= MOS_MAX_TASK)
        {
            ret = -1;
        }
        else
        {
            task_id_count++;
            mos_task_controller[task_id_count - 1].priority = task.priority;
            mos_task_controller[task_id_count - 1].init_handle = task.init_handle;
            mos_task_controller[task_id_count - 1].event_handle = task.event_handle;
            init_handle = task.init_handle;
            ret = task_id_count - 1;
        }
    }
    mos_exit_critial();    

    if(ret >= 0)
    {
        init_handle();
    }

    return ret;
}

mos_task_id_t mos_kernel_irq_create(mos_irq_t irq)
{
    mos_task_id_t ret = 0;    

    mos_enter_critial();
    {
        if(task_id_count >= MOS_MAX_TASK)
        {
            ret = -1;
        }
        else
        {
            task_id_count++;
            ret = task_id_count - 1;
        }
    }
    mos_exit_critial();    

    return ret;
}

mos_s32_t mos_kernel_event_publish(mos_task_id_t sender, mos_task_id_t receiver, mos_event_id_t event)
{
    mos_s32_t ret = 0;
    mos_evt_t task_event = { 0 };
    mos_task_event_handle_t event_handle = MOS_NULL_PTR;

    mos_enter_critial();
    {
        if(receiver >= task_id_count || sender >= task_id_count)
        {
            ret = -1;
        }
        else
        {
            task_event.event = event;
            task_event.sender = sender;

            if(IS_IN_INTTERUPT())
            {
                if(mos_queue_push(&mos_task_controller[receiver].events, &task_event, sizeof(task_event)) == 0)
                {
                    ret = 0;
                }
                else
                {
                    ret = -1;
                }
            }
            else
            {
                if(mos_task_controller[receiver].priority > mos_task_controller[sender].priority)
                {
                    event_handle = mos_task_controller[receiver].event_handle;
                    ret = 0;
                }
                else
                {
                    if(mos_queue_push(&mos_task_controller[receiver].events, &task_event, sizeof(task_event)) == 0)
                    {
                        ret = 0;
                    }
                    else
                    {
                        ret = -1;
                    }                    
                }                
            }
        }
    }
    mos_exit_critial();   

    if(event_handle != MOS_NULL_PTR)
    {
        event_handle(sender, event);
    }

    return ret;
}
