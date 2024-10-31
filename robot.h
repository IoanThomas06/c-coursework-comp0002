#include "position.h"
#include "marker.h"
#include "map.h"

/*
    The members represent the directional displacement of moving one unit in the
    current direction.
    For example:
        {0, 1} represents the direction of North (upwards).
        {-1, 0} represents the direction of West (leftwards).
*/
typedef struct DirectionVector
{
    int x;
    int y;
} DirectionVector;

typedef struct Robot
{
    Position position;
    int neswDirection;
    int markerCount;
    Marker **markers;
} Robot;

// Robot utility declarations.

Robot *initialiseRobot(Position initialPosition, int initialDirection,
                       size_t numberOfMarkers);
DirectionVector getDirectionVector(Robot *robot);
Position addDirectionToPosition(Position position,
                                DirectionVector direction);

// Robot control declarations.

void forward(Robot *robot);
void left(Robot *robot);
void right(Robot *robot);
int atMarker(Robot *robot, Marker *markers[], size_t numberOfMarkers);
int canMoveForward(Robot *robot, Map *map);
void pickUpMarker(Robot *robot, Marker *markers[], size_t numberOfMarkers);
void dropMarker(Robot *robot, Marker *marker);
int markerCount(Robot *robot);
int isAtHome(Robot *robot, Marker *markers[], size_t numberOfMarkers);