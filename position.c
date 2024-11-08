#include "position.h"
#include <stdbool.h>

bool comparePositionValues(Position position1, Position position2)
{
    return position1.x == position2.x && position1.y == position2.y;
}