#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>
#include "helper.h"
#include "proc.h"
#include "thread.h"


int main(int argc, char **argv) {
	int i;

	/* These are just some preset values for running a simple test case. */
	int n = 100;
	int value = 5;

	/* Print list of scrambled numbers. */
	int* list = listGen(n);
 	
	/*
	printf("\nList: [");
    for(i=0; i<n; ++i) {
        printf("%d ", list[i]);
    }
	printf("]\n");
    printf("Target: %d\n", value);
	printf("Parallels \t runtime\n");
	*/

	/* Create file to write data to*/
	//FILE *file = fopen("data", "w");
	
	/* Search for value, and the modify list  */
	for(i=1; i<=n; i++) {
		int j;
		int repetitions = 100;
		clock_t start = clock();
		for (j=0; j < repetitions; j++){
			int indexOfValue = _search(list, n, i, value);
			modifyList(list, n, indexOfValue);
		}
		clock_t end = clock();
		double cpu_time_used = ((double) (end - start)) /  CLOCKS_PER_SEC;
		printf("%d\t%.2f\t\n", i, cpu_time_used);
		//fprintf(file, "%d\t%.2f\t\n", i, cpu_time_used);
		}
	
	/* Close the file*/
	//fclose(file);
}

