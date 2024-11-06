#include "position.h"
#include "map.h"
#include <stddef.h>

#ifndef PATHFINDING_H
#define PATHFINDING_H

typedef struct Path
{
    size_t currentPoint;
    size_t pathLength;
    Position *path;
} Path;

Path *generatePath(Map *, Position, Position);
Position getNextMove();

#endif