#include "position.h"

#ifndef MARKER
#define MARKER

typedef struct Marker
{
    Position position;
    int isPlaced;
    int isHome;
} Marker;

Marker *initialiseMarker(Position, int);
Position getMarkerPosition(Marker *);
void pickUp(Marker *);
void drop(Marker *, Position);
int isPlacedMarker(Marker *);
int isHomeMarker(Marker *);

#endif