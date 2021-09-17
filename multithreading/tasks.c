#include "multithreading.h"
#include <stdlib.h>


__attribute__((constructor)) void tasks_mutex_init(void)
{
	pthread_mutex_init(&tasks_mutex, NULL);
}

__attribute__((destructor)) void tasks_mutex_destroy(void)
{
	pthread_mutex_destroy(&tasks_mutex);
}


/**
 * create_task - creates a new task structure and returns a pointer to it
 * @entry: pointer to the entry function of the task
 * @param: parameter to be passed to entry function
 * Return: pointer to the created task structure
 **/
task_t *create_task(task_entry_t entry, void *param)
{
	task_t *task = malloc(sizeof(task_t));
	static unsigned int id;

	if (task)
	{	task->entry = entry;
		task->param = param;
		task->lock = tasks_mutex;
		task->status = PENDING;
		task->result = NULL;
		task->id = id++;
	}

	return (task);
}

/**
 * destroy_task - destroys a task
 * @task: task to destroy
 **/
void destroy_task(task_t *task)
{
	if (task)
	{
		free(task->result);
		free(task);
	}
}

/**
 * exec_tasks - executes a list of tasks
 * @tasks: NULL-terminated list of tasks
 * Return: ???
 **/
void *exec_tasks(llist_t const *tasks)
{
	int tasks_pending = 1, task_id, i, size;
	task_t *node;

	if (tasks == NULL || (size = llist_size(tasks)) <= 0)
		pthread_exit(NULL);

	while (tasks_pending)
    {
        tasks_pending = 0;
        for (i = 0; i < size; i++)
        {
            node = llist_get_node_at(tasks, i);
			if (get_task_status(node) == PENDING)
			{
				tasks_pending = 1;
                task_id = node->id;
				set_task_status(node, STARTED);
				if (exec_task(node))
				{
					set_task_status(node, SUCCESS);
				}
				else
				{
					set_task_status(node, FAILURE);
				}
			}
        }
    }
	return (NULL);
}
