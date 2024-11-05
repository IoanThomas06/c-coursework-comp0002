#include <stddef.h>

#ifndef ALLOCATIONS_H
#define ALLOCATIONS_H

void *checkedMalloc(size_t, char[]);
void *checkedCalloc(size_t, size_t, char[]);

#endif