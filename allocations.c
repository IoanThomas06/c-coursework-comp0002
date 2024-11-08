#include "allocations.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

static void raiseAllocationError(char errorMessage[])
{
    printf("Failed to allocate memory to %s.\n", errorMessage);
    exit(EXIT_FAILURE);
}

void *checkedMalloc(size_t size, char errorMessage[])
{
    void *pointer = malloc(size);

    if (pointer)
    {
        return pointer;
    }
    raiseAllocationError(errorMessage);

    return NULL;
}

void *checkedCalloc(size_t numberOfObjects, size_t size, char errorMessage[])
{
    void *pointer = calloc(numberOfObjects, size);

    if (pointer)
    {
        return pointer;
    }
    raiseAllocationError(errorMessage);

    return NULL;
}