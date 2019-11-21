#ifndef _PROC_H
#define _PROC_H
#include <unistd.h>
#include "helper.h"

#define _search(list, size, parallels, target) search((list), (size), (parallels), (target))

// Structs
typedef struct _Sublist {
	int* list;
	int size;
} Sublist;

// Functin Prototypes

int search(int* list, int size, int parallels, int target);
int* generate_sublist_indices(int list_size, int n_procs);
void show_indices(int* sublist_indices, int n_procs);
void create_procs(int n_procs, int* list, int* sublist_indices, int list_size, int target, pid_t* children);
Sublist gen_sublist(int* list, int list_size, int* sublist_indices, int n_procs, int proc_id,  int offset);

#endif
