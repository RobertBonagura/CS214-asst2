#include "thread.h"

/*
thread_search(): a driver to do multiple searches on a randomized list using multithreading.
--
int* list: array of randomized integers.
int list_size: number of elements in list.
int n_threads: number of threads to use for search.
int target: the number to find in the list.
--
int return: the index of the target in list, or -1 if not found.
*/
int search(int* list, int list_size, int n_threads, int target){
	int i;
	int err;
	int* start_i;
	pthread_t thread[n_threads];
	thread_atts* atts[n_threads];
	int thread_returns[n_threads];
	
	// Getting start indicies for each thread.
	start_i = malloc(n_threads*sizeof(int));
	for(i=0; i<n_threads; ++i) {
		start_i[i] = 0;
	}
	split_list(list_size, start_i, n_threads);
	
	if(THREAD_DEBUG) {
		printf("thread_search(): Start indicies: [ ");
		for(i=0; i<n_threads; ++i) {
			printf("%d ", start_i[i]);
		}
		printf("]\n");
	}

	// Creating attribute structs for each thread.
	for(i=0; i<n_threads; ++i) {
		atts[i] = malloc(sizeof(thread_atts));
		atts[i]->list = &(list[start_i[i]]);
		if(i == n_threads-1) {
			atts[i]->list_size = list_size-start_i[i];
		}
		else {
			atts[i]->list_size = start_i[i+1]-start_i[i];
		}
		atts[i]->target = target;
	}

	// Creating a number of threads running linear_search() on parts of the list.
	for(i=0; i<n_threads; ++i) {
		err = pthread_create(&thread[i], NULL, my_thread, (void*) atts[i]);
		
		// If thread creation failed.
		if(err) {
			return(-1);
		}
	}

	// Joining the threads back together.
	for(i=0; i<n_threads; ++i) {
		pthread_join(thread[i], NULL);
		thread_returns[i] = start_i[i]+atts[i]->result;
	}

	if(THREAD_DEBUG) {
                printf("thread_search(): thread returns: [ ");
                for(i=0; i<n_threads; ++i) {
                        printf("%d ", atts[i]->result);
                }
                printf("]\n");
        }


	// Freeing allocated memory.
	free(start_i);
	for(i=0; i<n_threads; ++i) {
		free(atts[i]);
	} 
	
	// Inspecting the return values of each thread for a success.
	for(i=0; i<n_threads; ++i) {
		if(thread_returns[i] >= start_i[i]) {
			return(thread_returns[i]);
		}
	}
	return(-1);
}

void* my_thread(void* arg) {
	thread_atts *tdata = (thread_atts*)arg;
	
	int* list = tdata->list;
	int list_size = tdata->list_size;
	int target = tdata->target;

	int result = linear_search(list, list_size, target);
	tdata->result = result;
//	printf("my_thread(): linear search on list of size %d, found at %d\n", list_size, result);
	pthread_exit(NULL);
}

/*
linear_search(): a linear search of an int array.
--
int* list: array of integers.
int list_size: number of elements in list.
int target: the number to find in list.
--
int return: the index of the target in list, or -1 if not found.
*/
int linear_search(int* list, int list_size, int target) {
        int i;
        for(i=0; i<list_size; ++i) {
                if(list[i] == target) {
                        return(i);
                }
        }
        return(-1);
}



