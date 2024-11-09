#include "position.h"
#include "map.h"
#include <stddef.h>

#ifndef PATHFINDING_H
#define PATHFINDING_H

typedef struct Path Path;

Path *findPath(Map *, Position, Position);
int getPathLength(Path *path);
Position peekPath(Path *path);
Position getNextPosition(Path *);

#endif