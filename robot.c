#include "robot.h"
#include "position.h"
#include "direction.h"
#include "marker.h"
#include "map.h"
#include "allocations.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

// Static type definitions.

struct Robot
{
    Position position;
    // Maps North through to West directions to the values 0 through to 3
    // respectively.
    int neswDirection;
    int markerCount;
    Marker **markers;
};

// End of static type definitions.

// Static declarations.

static void turn(Robot *, int);

// End of static declarations.

// Utilities.

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

void deallocateRobot(Robot *robot)
{
    free(robot->markers);
    free(robot);
}

Position getRobotPosition(Robot *robot)
{
    return robot->position;
}

DirectionVector getDirectionVector(Robot *robot)
{
    return generateDirectionVector(robot->neswDirection);
}

int getRotationalOffset(Robot *robot)
{
    return robot->neswDirection;
}

static void turn(Robot *robot, int relativeDirection)
{
    /*
        Accounts for -1 and 4 as nesw values by circling from 'West' back to
        'North' and vice versa.
        Also handles any < -1 or > 4 if the argument was not -1 or 1.
    */
    robot->neswDirection =
        ((getRotationalOffset(robot) + relativeDirection) % 4 + 4) % 4;
}

// End of utilities.

// Robot control interface.

void forward(Robot *robot)
{
    robot->position = addDirectionToPosition(getRobotPosition(robot),
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

bool atMarker(Robot *robot, Marker *markers[], size_t numberOfMarkers)
{
    for (size_t i = 0; i < numberOfMarkers; i++)
    {
        if (comparePositionValues(getRobotPosition(robot),
                                  getMarkerPosition(markers[i])))
        {
            return true;
        }
    }
    return false;
}

bool canMoveForward(Robot *robot, Map *map)
{
    Position nextPosition = addDirectionToPosition(getRobotPosition(robot),
                                                   getDirectionVector(robot));

    if (isMapPositionValid(map, nextPosition) &&
        isMapPositionEmpty(map, nextPosition))
    {
        return true;
    }
    return false;
}

void pickUpMarker(Robot *robot, Marker *markers[], size_t numberOfMarkers)
{
    for (size_t i = 0; i < numberOfMarkers; i++)
    {
        if (comparePositionValues(getRobotPosition(robot),
                                  getMarkerPosition(markers[i])))
        {
            pickUp(markers[i]);
            robot->markers[i] = markers[i];
            robot->markerCount++;
        }
    }
}

void dropMarker(Robot *robot, Marker *markers[],
                size_t numberOfMarkers)
{
    size_t i = 0;
    while (!markers[i] && ++i < numberOfMarkers)
        ;

    if (i != numberOfMarkers)
    {
        drop(markers[i], getRobotPosition(robot));
        robot->markers[i] = NULL;
        robot->markerCount--;
    }
}

int markerCount(Robot *robot)
{
    return robot->markerCount;
}

bool atHome(Robot *robot, Marker *markers[], size_t numberOfMarkers)
{
    for (size_t i = 0; i < numberOfMarkers; i++)
    {
        if (isHomeMarker(markers[i]) &&
            comparePositionValues(getRobotPosition(robot),
                                  getMarkerPosition(markers[i])))
        {
            return true;
        }
    }
    return false;
}

// End of robot control interface.