#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "mos.h"

extern void port_init();
extern void *port_sim_irq(void *args);

// -------------------------------------------------------------------
// Task definition
// -------------------------------------------------------------------
mos_task_id_t task_id_1;
mos_task_id_t task_id_2;
mos_task_id_t task_id_sim_irq;

void task_init_handle_1(void)
{
    printf("Task 1 init.\n");
}
void task_event_handle_1(mos_task_id_t sender, mos_event_id_t evt)
{
    printf("Task 1 process event: sender: %d, event: %d.\n", sender, evt);

    mos_kernel_event_publish(task_id_1, task_id_2, 2);
}

mos_task_t task_1 = 
{
    .priority = 1,
    .init_handle = task_init_handle_1,
    .event_handle = task_event_handle_1,
};

void task_init_handle_2(void)
{
    printf("Task 2 init.\n");
}

void task_event_handle_2(mos_task_id_t sender, mos_event_id_t evt)
{
    printf("Task 2 process event: sender: %d, event: %d.\n", sender, evt);
}

mos_task_t task_2 = 
{
    .priority = 2,
    .init_handle = task_init_handle_2,
    .event_handle = task_event_handle_2,
};

// -------------------------------------------------------------------
// Simulated irq by pthread
// -------------------------------------------------------------------
mos_irq_t sim_irq = 
{
    .name = "sim irq"
};

int main(int argc, char* argv[])
{
    // ports
    port_init();

    // init mos kernel
    mos_kernel_init();

    // create tasks
    task_id_1 = mos_kernel_task_create(task_1);
    task_id_2 = mos_kernel_task_create(task_2);

    // register irqs
    task_id_sim_irq = mos_kernel_irq_create(sim_irq);

    // start to run mos kernel
    mos_kernel_run();

    printf("failed to start mos kernel.\n");
}

// -------------------------------------------------------------------
// Portings for linux
// -------------------------------------------------------------------
pthread_mutex_t mutex_lock;

void port_init()
{
    pthread_t pid;
    pthread_mutexattr_t mat;
    pthread_mutexattr_init(&mat);   

    pthread_mutexattr_settype(&mat, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutex_lock, &mat);

    pthread_create(&pid, NULL, port_sim_irq, NULL);
}

void port_enable_irq()
{
    pthread_mutex_lock(&mutex_lock);
}

void port_disable_irq()
{
    pthread_mutex_unlock(&mutex_lock);
}

void *port_sim_irq(void *args)
{
    printf("%s start to run.\n", __FUNCTION__);

    while(1)
    {
        mos_kernel_event_publish(task_id_sim_irq, task_id_1, 1);
        usleep(100 * 1000);        
    }
}