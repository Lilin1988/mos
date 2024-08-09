/*
    <Header of kernel on the top level which is part of mos(micro-kernel operation system).>
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

#ifndef __MOS_H__
#define __MOS_H__

#include "mos_ports.h"
#include "mos_queue.h"

#define MOS_MAX_TASK    (8)
#define MOS_MAX_EVENT   (8)

typedef mos_s32_t   mos_task_id_t;
typedef mos_u32_t   mos_task_pri_t;
typedef mos_s32_t   mos_event_id_t;

typedef void (*mos_task_init_handle_t)(void);
typedef void (*mos_task_idle_hook_t)(void);
typedef void (*mos_daemon_task_event_handle_t)(void);
typedef void (*mos_task_event_handle_t)(mos_task_id_t sender, mos_event_id_t evt);
typedef void (*mos_kernel_tick_handle_t)(void);

typedef struct
{
    mos_s8_t name[32];
}mos_irq_t;

typedef struct
{
    mos_task_pri_t priority;        
    mos_task_init_handle_t init_handle;
    mos_task_event_handle_t event_handle;
}mos_task_t;

typedef struct
{       
    mos_task_init_handle_t init_handle;
    mos_daemon_task_event_handle_t daemon_event_handle;
}mos_daemon_task_t;

typedef struct
{
    mos_s32_t current;
    mos_s32_t minimum;
    mos_s32_t maximum;
}mos_cpu_usage_t;

void mos_exit_critial(void);
void mos_enter_critial(void);

mos_s32_t mos_queue_pop(mos_queue_t *queue, void *buffer, mos_s32_t size);
mos_s32_t mos_queue_push(mos_queue_t *queue, void *buffer, mos_s32_t size);
mos_s32_t mos_queue_init(mos_queue_t *queue, void *buffer, mos_s32_t length);
mos_s32_t mos_queue_try_pop(mos_queue_t *queue, void *buffer, mos_s32_t size);

mos_s32_t mos_kernel_run(void);
mos_s32_t mos_kernel_init(void);
mos_task_id_t mos_kernel_irq_create(mos_irq_t irq);
mos_task_id_t mos_kernel_task_create(mos_task_t task);
mos_task_id_t mos_kernel_daemon_task_create(mos_daemon_task_t task);
mos_s32_t mos_kernel_register_idle_hook(mos_task_idle_hook_t idle_hook);
mos_s32_t mos_kernel_event_publish(mos_task_id_t sender, mos_task_id_t receiver, mos_event_id_t event);

void mos_kernel_isr_switch_in(void);
void mos_kernel_isr_switch_out(void);
void mos_kernel_cpu_usage_monitor(void);
void mos_kernel_get_cpu_usage(mos_cpu_usage_t *cpu_usage);
#endif