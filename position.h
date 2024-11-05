#include <stddef.h>

#ifndef POSITION_H
#define POSITION_H

typedef struct Position
{
    size_t x;
    size_t y;
} Position;

int comparePositionValues(Position, Position);

#endif