#include "direction.h"
#include "position.h"
#include <stdlib.h>

DirectionVector generateDirectionVector(int neswDirection)
{
    switch (neswDirection)
    {
    case 0:
        return (DirectionVector){.x = 0, .y = 1};
    case 1:
        return (DirectionVector){.x = 1, .y = 0};
    case 2:
        return (DirectionVector){.x = 0, .y = -1};
    case 3:
        return (DirectionVector){.x = -1, .y = 0};
    default:
        return (DirectionVector){.x = 0, .y = 0};
    }
}

Position addDirectionToPosition(Position position, DirectionVector direction)
{
    return (Position){.x = position.x + direction.x,
                      .y = position.y + direction.y};
}

DirectionVector getDirectionBetweenPositions(Position positionA,
                                             Position positionB)
{
    DirectionVector direction = {.x = 0,
                                 .y = 0};

    int xDifference = positionA.x - positionB.x;
    int yDifference = positionA.y - positionB.y;

    if (abs(xDifference))
    {
        direction.x = (xDifference > 0) ? 1 : -1;
    }

    if (abs(yDifference))
    {
        direction.y = (yDifference > 0) ? 1 : -1;
    }

    return direction;
}

int getRelativeTurnBetweenDirections(DirectionVector initialDirection,
                                     DirectionVector targetDirection)
{
    if (initialDirection.y == targetDirection.y &&
        initialDirection.x == targetDirection.x)
    {
        return 0;
    }

    if ((initialDirection.y && targetDirection.y) ||
        (initialDirection.x && targetDirection.x))
    {
        return 2;
    }

    return (initialDirection.y) ? initialDirection.y * targetDirection.x
                                : -(initialDirection.x * targetDirection.y);
}