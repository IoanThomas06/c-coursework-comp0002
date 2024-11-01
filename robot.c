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
    position.x += direction.x;
    position.y += direction.y;
    return position;
}

static void turn(Robot *robot, int relativeDirection)
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
    turn(robot, -1);
}

void right(Robot *robot)
{
    turn(robot, 1);
}

int atMarker(Robot *robot, Marker *markers[], size_t numberOfMarkers)
{
    for (size_t i = 0; i < numberOfMarkers; i++)
    {
        if (comparePositionValues(robot->position, markers[i]->position))
        {
            return 1;
        }
    }
    return 0;
}

int canMoveForward(Robot *robot, Map *map)
{
    Position nextPosition = addDirectionToPosition(robot->position,
                                                   getDirectionVector(robot));
    if (nextPosition.x == -1 ||
        nextPosition.x == getColumnSize(map) ||
        nextPosition.y == -1 ||
        nextPosition.y == getRowSize(map) ||
        !isMapPositionEmpty(map, nextPosition))
    {
        return 0;
    }
    return 1;
}

void pickUpMarker(Robot *robot, Marker *markers[], size_t numberOfMarkers)
{
    for (size_t i = 0; i < numberOfMarkers; i++)
    {
        if (comparePositionValues(robot->position, markers[i]->position))
        {
            pickUp(markers[i]);
            robot->markers[i] = markers[i];
            robot->markerCount++;
        }
    }
}

void dropMarker(Robot *robot, Marker *marker, Marker *markers[],
                size_t numberOfMarkers)
{
    for (size_t i = 0; i < numberOfMarkers; i++)
    {
        if (marker == markers[i])
        {
            drop(marker, robot->position);
            robot->markers[i] = NULL;
            robot->markerCount--;
        }
    }
}

int markerCount(Robot *robot)
{
    return robot->markerCount;
}

int isAtHome(Robot *robot, Marker *markers[], size_t numberOfMarkers)
{
    for (size_t i = 0; i < numberOfMarkers; i++)
    {
        if (isHomeMarker(markers[i]) &&
            comparePositionValues(robot->position, markers[i]->position))
        {
            return 1;
        }
    }
    return 0;
}