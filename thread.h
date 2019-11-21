#ifndef _THREAD_H
#define _THREAD_H
#include "helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define _search(list, size, parallels, target) search((list), (size), (parallels), (target))
#define THREAD_DEBUG 0

typedef struct _thread_atts {
	int* list;
	int list_size;
	int target;
	int result;
} thread_atts;

int search(int* list, int list_size, int n_threads, int target);
void* my_thread(void* arg);
int linear_search(int* list, int list_size, int target);

#endif
