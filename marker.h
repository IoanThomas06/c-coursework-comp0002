#include "position.h"

#ifndef MARKER_H
#define MARKER_H

typedef struct Marker
{
    Position position;
    int isPlaced;
    int isHome;
} Marker;

Marker *initialiseMarker(Position, int);
void deallocateMarker(Marker *marker);
Position getMarkerPosition(Marker *);
void pickUp(Marker *);
void drop(Marker *, Position);
int isPlacedMarker(Marker *);
int isHomeMarker(Marker *);

#endif