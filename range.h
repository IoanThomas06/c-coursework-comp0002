#include <stddef.h>

#ifndef RANGE_H
#define RANGE_H

typedef struct Range
{
    // Inclusive.
    size_t min;
    // Exclusive.
    size_t max;
} Range;

size_t selectFromRange(Range);

#endif