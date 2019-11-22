#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>
#include <math.h>
#include "multitest.h"
#ifdef PROC
#include "multitest_proc.h"
#endif
#ifdef THREAD
#include "multitest_thread.h"
#endif

// SEARCH_MODE = 1 for proc, 2 for thread

#define TARGET_VALUE 1
#define REPETITIONS 1

int main(int argc, char **argv) {	
	int i;
	FILE *file;
	
	// Process tests.
	if(SEARCH_MODE == 1) {
		// Process test 1
		file = fopen("proctest1_data", "w");
		for(i=1; i<=100; i+=1) {
			search_test(i, 100, 10, file);
		}
		fclose(file);
		// Process test 2
		file = fopen("proctest2_data", "w");
		for(i=1; i<=64; i*=2) {
			search_test(i, 10000, 10, file);
		}
		fclose(file);
	}
	
	// Thread tests.
	if(SEARCH_MODE == 2) {
		// Thread test 1
		file = fopen("threadtest1_data", "w");
		for(i=1; i<=100; i+=1) {
			search_test(i, 100, 10, file);
		}
		fclose(file);
		// Thread test 2
		file = fopen("threadtest2_data", "w");
		for(i=1; i<=64; i*=2) {
			search_test(i, 1000000, 10, file);
		}
		fclose(file);
	}
}

int search_test(int n_parallels, int list_size, int n_batches, FILE* file) {
	int j, k, indexOfValue;	

	double* time_array = malloc(n_batches*sizeof(double));
	double time_total;
	double time_min; 
	double time_max;
	double time_average;
	double time_std;
	double time_variance;

	clock_t start, end, start_single, end_single;

	// Get list of scrambled numbers.
	int* list = listGen(list_size);

	// Display the current multi-mode.
	printf("\n");
	if(SEARCH_MODE == 1) {
		printf("Multi-mode: process\n");
	}
	if(SEARCH_MODE == 2) {
		printf("Multi-mode: thread\n");
	}

	// Display current testplan.
	printf("Current testplan:\n");
	printf("\tParallels: %d\n", n_parallels);
	printf("\tSize of list: %d\n", list_size);
	printf("\tNumber of batches: %d\n", n_batches);

	start = clock();
	for (j=0; j < n_batches; j++){
		start_single = clock();
		for(k=0; k<REPETITIONS; ++k) {
			indexOfValue = _search(list, list_size, n_parallels, TARGET_VALUE);
		}
		end_single = clock();
		time_array[j] = (double) (end_single - start_single);
		printf("Time for %d searches on list of size %d with %d parallels: %.2f seconds\n", REPETITIONS, list_size, n_parallels, time_array[j] / CLOCKS_PER_SEC);
		modifyList(list, list_size, indexOfValue);
	}
	end = clock();
		
	// Getting aggregate data.
	time_total = ((double) (end - start));
	time_average = time_total / n_batches;
	time_max = 0;
	time_min = time_total;
	for(j=0; j<n_batches; ++j) {
		if(time_array[j] > time_max) {
			time_max = time_array[j];
		}
		if(time_array[j] < time_min) {
			time_min = time_array[j];
		}
	}
	time_variance = 0;
	for(j=0; j<n_batches; ++j) {
		time_variance += (time_array[j] - time_average)*(time_array[j] - time_average);
	}
	time_variance -= time_average*time_average;
	time_std = sqrt(fabs(time_variance));
		
	// Printing aggregate data.
	printf("Aggregate data for %d parallels:\n", n_parallels);
	printf("Min: %.2f seconds\n", time_min / CLOCKS_PER_SEC);
        printf("Max: %.2f seconds\n", time_max / CLOCKS_PER_SEC);
        printf("Average: %.2f seconds\n", time_average / CLOCKS_PER_SEC);
        printf("Standard deviation: %.2f seconds\n", time_std / CLOCKS_PER_SEC);	
	printf("\n");	

	// Saving data for figures.
	// <number of repetitons> <number of procs/threads>
	// (tab)
	// <min> <max> <average> <standard deviation>
	// (newline)
	fprintf(file, "%d %d\t%.2f %.2f %.2f %.2f\n", REPETITIONS, n_parallels, time_min / CLOCKS_PER_SEC, time_max / CLOCKS_PER_SEC, time_average / CLOCKS_PER_SEC, time_std / CLOCKS_PER_SEC);
	
	// Heap cleanup.
	free(time_array);
}

