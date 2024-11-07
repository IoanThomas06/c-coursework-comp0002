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

int convertDirectionVectorToNesw(DirectionVector directionVector)
{
    if (directionVector.x)
    {
        return (directionVector.x > 0) ? 1 : 3;
    }
    else if (directionVector.y)
    {
        return (directionVector.y > 0) ? 0 : 2;
    }
    return -1;
}

Position addDirectionToPosition(Position position, DirectionVector direction)
{
    position.x += direction.x;
    position.y += direction.y;

    return position;
}

DirectionVector getDirectionBetweenPositions(Position positionA,
                                             Position positionB)
{
    return (DirectionVector){.x = (positionB.x - positionA.x) % 1,
                             .y = (positionB.y - positionA.y) % 1};
}

int getRelativeTurnBetweenNeswDirections(DirectionVector initialDirection,
                                         DirectionVector targetDirection)
{
    return (initialDirection.y) ? initialDirection.y * targetDirection.x
                                : -(initialDirection.x * targetDirection.y);
}