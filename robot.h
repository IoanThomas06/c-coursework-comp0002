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

Robot *initialiseRobot(Position, int, size_t);
DirectionVector getDirectionVector(Robot *);
Position addDirectionToPosition(Position, DirectionVector);

// Robot control declarations.

void forward(Robot *);
void left(Robot *);
void right(Robot *);
int atMarker(Robot *, Marker *[], size_t);
int canMoveForward(Robot *, Map *);
void pickUpMarker(Robot *, Marker *[], size_t);
void dropMarker(Robot *, Marker *[], size_t);
int markerCount(Robot *);
int isAtHome(Robot *, Marker *[], size_t);