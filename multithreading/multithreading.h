#ifndef MULTITHREADING_H
#define MULTITHREADING_H
#include <stddef.h>
#include <stdint.h>
#include <pthread.h>
#include <llist.h>


pthread_mutex_t tasks_mutex;


typedef void *(*task_entry_t)(void *);

/**
 * enum task_status_e - Task statuses
 *
 * @PENDING: Task is pending
 * @STARTED: Task has been started
 * @SUCCESS: Task has completed successfully
 * @FAILURE: Task has completed with issues
 */
typedef enum task_status_e
{
    PENDING = 0,
    STARTED,
    SUCCESS,
    FAILURE
} task_status_t;

/**
 * struct task_s - Executable task structure
 *
 * @entry:  Pointer to a function to serve as the task entry
 * @param:  Address to a custom content to be passed to the entry function
 * @status: Task status, default to PENDING
 * @result: Stores the return value of the entry function
 * @lock:   Task mutex
 */
typedef struct task_s
{
    task_entry_t    entry;
    void        *param;
    task_status_t   status;
    void        *result;
    pthread_mutex_t lock;
	unsigned int id;
} task_t;

task_t *create_task(task_entry_t entry, void *param);
void destroy_task(task_t *task);
void *exec_tasks(llist_t const *tasks);
void *exec_task(task_t *task);
task_status_t get_task_status(task_t *task);
void set_task_status(task_t *task, task_status_t status);

#endif /* MULTITHREADING_H */
