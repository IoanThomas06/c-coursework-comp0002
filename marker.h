#include "position.h"
#include <stdbool.h>

#ifndef MARKER_H
#define MARKER_H

typedef struct Marker Marker;

Marker *initialiseMarker(Position, bool);
void deallocateMarker(Marker *marker);
Position getMarkerPosition(Marker *);
void pickUp(Marker *);
void drop(Marker *, Position);
bool isPlacedMarker(Marker *);
bool isHomeMarker(Marker *);

#endif