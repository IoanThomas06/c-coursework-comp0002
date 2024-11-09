#include "range.h"
#include <stdlib.h>

size_t selectFromRange(Range range)
{
    return range.min + rand() % range.max;
}