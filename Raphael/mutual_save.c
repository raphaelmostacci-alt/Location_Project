#include "mutual_save.h"

void mutual_save(const char* filename, void* array, size_t type_size, int count) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    for (int i = 0; i < count; i++) {
        fwrite((char*)array + i * type_size, type_size, 1, f);
    }
    fclose(f);
}
