#include "mutual_load.h"

void* mutual_load(const char* filename, size_t type_size, int* out_count) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        *out_count = 0;
        return NULL;
    }
    void *array = NULL;
    int count = 0;
    void *temp = malloc(type_size);
    while (fread(temp, type_size, 1, f) == 1) {
        array = realloc(array, (count + 1) * type_size);
        memcpy((char*)array + count * type_size, temp, type_size);
        count++;
    }
    free(temp);
    fclose(f);
    *out_count = count;
    return array;
}
