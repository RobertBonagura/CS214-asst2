#include "proc.h"


int search(int* list, int list_size, int n_procs, int target){

	int i;	
	
	// Getting first index of each sublist,
	// where each process will search through each sublist.
	int* sublist_indices = generate_sublist_indices(list_size, n_procs);
	//show_indices(sublist_indices, n_procs);

	// Preallocate space for each child's pid 
	pid_t* children = malloc(sizeof(pid_t)*n_procs);

	// create children processes and write to child_responses array
	create_procs(n_procs, list, sublist_indices, list_size, target, children);


	// Look through children processes and find child who found target
	int childResult = -1;
	int output = -1;
	for (i=0; i < n_procs; i++){
		pid_t childPid =  children[i];
		waitpid(childPid, &childResult, 0);
		int return_val = WEXITSTATUS(childResult);
		if (return_val != 255){  // If it is found
			output =  return_val + sublist_indices[i];
		}
	}
	free(sublist_indices);
	free(children);
	return output;
}

// Getting start indicies for each process.
int* generate_sublist_indices(int list_size, int n_procs){

	int i;	
	// Initialize array of size n_procs to all zeros
	int* sublist_indices = malloc(n_procs*sizeof(int));
	
	for (i=0; i<n_procs; i++){
		sublist_indices[i] = 0;
	}

	// Use split_list function to generate list of starting
	// indices for each sublist.
	split_list(list_size, sublist_indices, n_procs);
	
	return sublist_indices;;
}

void show_indices(int* sublist_indices, int n_procs){

	int i;
	printf("proc_search(): Start indicies: [");
	for (i=0; i<n_procs; i++){
		printf("%d ", sublist_indices[i]);
	}
	printf("]\n");
}

void create_procs(int n_procs, int* list, int* sublist_indices, int list_size, int target, pid_t* children){

	int offset = 0;
	int i;
	for (i=0; i < n_procs; i++){
	
		int first_index, last_index, sublist_size;

		// Use number of procs and sublist array to determine sublists
		Sublist sublist =  gen_sublist(list, list_size, sublist_indices, n_procs, i, offset);
		sublist_size = sublist.size;
		int* sub = sublist.list;
		first_index = offset;
		offset = offset + sublist_size;

		pid_t childPid = fork();
		if (childPid == 0){	//Child
			int j = 0;
			// replace list and list_size below with new values.
			for (j; j < sublist_size; j++){ 
				if (sub[j] == target){
	//				printf("Child %d found target at index %d of sublist\n", i, j);
					exit(j);
				}
			}
	//		printf("Child %d did not find target of sublist\n", i);
			exit(-1);
		}
		else if (childPid < 1){
			printf("Fork failed"); // Log error
		} else {
			// Add pid to array
			children[i] = childPid;
		}
	}
	return;
}

// Determine subList to use
Sublist gen_sublist(int* list, int list_size, int* sublist_indices, int n_procs, int proc_id, int offset){

	int first_index, last_index, sublist_size;
		
	first_index = sublist_indices[proc_id];
	if (proc_id == n_procs - 1){  // if this is the last process
			last_index = list_size - 1;
	} else {
			last_index = sublist_indices[proc_id+1] - first_index + offset - 1;
	}
	sublist_size = last_index - first_index + 1;
	int* sub = malloc(sizeof(int) * sublist_size);
	
	int i;
	for (i = 0; i < sublist_size; i++){
		sub[i] = list[i+offset];
	}

	Sublist sublist = {sub, sublist_size};
	return sublist;
}

