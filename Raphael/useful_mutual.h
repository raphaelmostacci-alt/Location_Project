#ifndef USEFUL_MUTUAL_H
#define USEFUL_MUTUAL_H
#include "includes.h"

void mutual_free(void *array);
void* mutual_load(const char* filename, size_t type_size, int* out_count);
void mutual_save(const char* filename, void* array, size_t type_size, int count);

#endif