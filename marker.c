#include "marker.h"
#include "position.h"
#include "allocations.h"
#include <stdlib.h>
#include <stdbool.h>

Marker *initialiseMarker(Position initialPosition, bool isHome)
{
    Marker *marker = (Marker *)checkedMalloc(sizeof(Marker), "Marker");
    marker->position = initialPosition;
    marker->isPlaced = true;
    marker->isHome = isHome;

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
    marker->isPlaced = false;
}

void drop(Marker *marker, Position position)
{
    marker->position = position;
    marker->isPlaced = true;
}

bool isPlacedMarker(Marker *marker)
{
    return marker->isPlaced;
}

bool isHomeMarker(Marker *marker)
{
    return marker->isHome;
}