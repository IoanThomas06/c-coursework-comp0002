#include "position.h"
#include "direction.h"
#include "marker.h"
#include "map.h"
#include <stddef.h>
#include <stdbool.h>

#ifndef ROBOT_H
#define ROBOT_H

typedef struct Robot
{
    Position position;
    // Maps North through to West directions to the values 0 through to 3
    // respectively.
    int neswDirection;
    int markerCount;
    Marker **markers;
} Robot;

// Robot utility declarations.

Robot *initialiseRobot(Position, int, size_t);
void deallocateRobot(Robot *);
Position getRobotPosition(Robot *);
DirectionVector getDirectionVector(Robot *);
int getRotationalOffset(Robot *);

// Robot control declarations.

void forward(Robot *);
void left(Robot *);
void right(Robot *);
bool atMarker(Robot *, Marker *[], size_t);
bool canMoveForward(Robot *, Map *);
void pickUpMarker(Robot *, Marker *[], size_t);
void dropMarker(Robot *, Marker *[], size_t);
int markerCount(Robot *);
bool atHome(Robot *, Marker *[], size_t);

#endif