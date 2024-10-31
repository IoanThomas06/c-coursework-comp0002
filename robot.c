#include "robot.h"
#include "position.h"
#include "marker.h"
#include "map.h"
#include "allocations.h"
#include <stdlib.h>

Robot *initialiseRobot(Position initialPosition, int initialDirection,
                       size_t numberOfMarkers)
{
    Robot *robot = (Robot *)checkedMalloc(sizeof(Robot), "Robot");
    Marker **markers = (Marker **)checkedCalloc(numberOfMarkers,
                                                sizeof(Marker *),
                                                "'markers' in Robot");
    robot->position = initialPosition;
    robot->neswDirection = initialDirection;
    robot->markerCount = 0;
    robot->markers = markers;
    return robot;
}

DirectionVector getDirectionVector(Robot *robot)
{
    switch (robot->neswDirection)
    {
    case 0:
        return (DirectionVector){0, 1};
    case 1:
        return (DirectionVector){1, 0};
    case 2:
        return (DirectionVector){0, -1};
    // Default case only occurs for values: 3.
    default:
        return (DirectionVector){-1, 0};
    }
}

Position addDirectionToPosition(Position position, DirectionVector direction)
{
    position.y += direction.y;
    position.x += direction.x;
    return position;
}

static void makeTurn(Robot *robot, int relativeDirection)
{
    // Accounts for -1 and 4 as values by circling from 'West' back to 'North'
    // and vice versa.
    robot->neswDirection = (robot->neswDirection + relativeDirection + 4) % 4;
}

void forward(Robot *robot)
{
    robot->position = addDirectionToPosition(robot->position,
                                             getDirectionVector(robot));
}

void left(Robot *robot)
{
    makeTurn(robot, -1);
}

void right(Robot *robot)
{
    makeTurn(robot, 1);
}