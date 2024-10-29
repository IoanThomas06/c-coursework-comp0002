#include "robot.h"
#include "position.h"
#include "marker.h"
#include "map.h"
#include <stdlib.h>

Robot *initialiseRobot(Position initialPosition, int initialDirection,
                       int numberOfMarkers)
{
    Robot *robot = (Robot *)malloc(sizeof(Robot));
    Marker **markers = (Marker **)calloc(numberOfMarkers, sizeof(Marker *));
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
    default:
        return (DirectionVector){-1, 0};
    }
}