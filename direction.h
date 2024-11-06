#include "position.h"
#include <stddef.h>

#ifndef DIRECTION_H
#define DIRECTION_H

/*
    The members represent the directional displacement of moving one unit in the
    current direction.
    For example:
        {0, 1} represents the direction of North (upwards).
        {-1, 0} represents the direction of West (leftwards).
*/
typedef struct DirectionVector
{
    int x;
    int y;
} DirectionVector;

DirectionVector generateDirectionVector(size_t);

Position addDirectionToPosition(Position, DirectionVector);

#endif