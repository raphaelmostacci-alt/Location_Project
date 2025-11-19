#ifndef MUTUAL_LOAD_H
#define MUTUAL_LOAD_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void* mutual_load(const char* filename, size_t type_size, int* out_count);

#endif
