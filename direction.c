#include "direction.h"
#include "position.h"

DirectionVector generateDirectionVector(size_t neswDirection)
{
    switch (neswDirection)
    {
    case 0:
        return (DirectionVector){0, 1};
    case 1:
        return (DirectionVector){1, 0};
    case 2:
        return (DirectionVector){0, -1};
    case 3:
        return (DirectionVector){-1, 0};
    default:
        return (DirectionVector){0, 0};
    }
}

Position addDirectionToPosition(Position position, DirectionVector direction)
{
    position.x += direction.x;
    position.y += direction.y;

    return position;
}