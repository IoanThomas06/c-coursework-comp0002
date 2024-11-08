#include "position.h"

#ifndef DIRECTION_H
#define DIRECTION_H

/*
    The members represent the directional displacement of moving one unit in the
    current direction on a 2D Cartesian grid, with origin {x = 0, y = 0}.
    For example:
        {0, 1} represents the direction of North (upwards).
        {-1, 0} represents the direction of West (leftwards).
*/
typedef struct DirectionVector
{
    int x;
    int y;
} DirectionVector;

DirectionVector generateDirectionVector(int);
int convertDirectionVectorToNesw(DirectionVector);

Position addDirectionToPosition(Position, DirectionVector);
DirectionVector getDirectionBetweenPositions(Position, Position);
int getRelativeTurnBetweenDirections(DirectionVector, DirectionVector);

#endif