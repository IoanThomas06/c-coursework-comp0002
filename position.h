#include <stddef.h>
#include <stdbool.h>

#ifndef POSITION_H
#define POSITION_H

typedef struct Position
{
    size_t x;
    size_t y;
} Position;

bool comparePositionValues(Position, Position);

#endif