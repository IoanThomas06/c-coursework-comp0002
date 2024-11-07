#include "range.h"
#include <stdlib.h>
#include <time.h>

size_t selectFromRange(Range range)
{
    return range.min + rand() % range.max;
}