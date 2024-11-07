#include "marker.h"
#include "position.h"
#include "allocations.h"
#include <stdlib.h>

Marker *initialiseMarker(Position initialPosition, int isHome)
{
    Marker *marker = (Marker *)checkedMalloc(sizeof(Marker), "Marker");
    marker->position = initialPosition;
    marker->isPlaced = 0;
    marker->isHome = (isHome) ? 1 : 0;

    return marker;
}

void deallocateMarker(Marker *marker)
{
    free(marker);
}

Position getMarkerPosition(Marker *marker)
{
    return marker->position;
}

void pickUp(Marker *marker)
{
    marker->isPlaced = 1;
}

void drop(Marker *marker, Position position)
{
    marker->position = position;
    marker->isPlaced = 0;
}

int isPlacedMarker(Marker *marker)
{
    return marker->isPlaced;
}

int isHomeMarker(Marker *marker)
{
    return marker->isHome;
}