#ifndef _HELPER_H
#define _HELPER_H
#include <stdio.h>
#include <stdlib.h>

int* listGen(int n);

int _search(int* list, int list_size, int parallels, int target);

void modifyList(int* list, int n, int index);

int split_list(int list_size, int* start_i, int n_parallels);

#endif
