#include "position.h"
#include "marker.h"
#include "map.h"

typedef struct DirectionVector
{
    int x;
    int y;
} DirectionVector;

typedef struct Robot
{
    Position position;
    int directionPointer;
    int markerCount;
    Marker *markers[];
} Robot;

// Robot utility declarations.
DirectionVector getDirection(Robot *robot);

// Robot control declarations.
Robot initialiseRobot(Position initialPosition, int initialDirection, int numberOfMarkers);
void forward(Robot *robot);
void left(Robot *robot);
void right(Robot *robot);
int atMarker(Robot *robot, Marker *markers[], int numberOfMarkers);
int canMoveForward(Robot *robot, Map *map);
void pickUpMarker(Robot *robot, Marker *marker);
void dropMarker(Robot *robot, Marker *marker);
int markerCount(Robot *robot);
int isAtHome(Robot *robot, Marker *markers[]);