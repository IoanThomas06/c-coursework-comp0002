#include "marker.h"
#include <stdlib.h>

Marker *initialiseMarker(Position initialPosition, int isHome)
{
    Marker *marker = (Marker *)malloc(sizeof(marker));
    marker->position = initialPosition;
    marker->isPlaced = 0;
    marker->isHome = (isHome) ? 1 : 0;
    return marker;
}

Position getPosition(Marker *marker)
{
    return marker->position;
}

int isPlacedMarker(Marker *marker)
{
    return marker->isPlaced;
}

int isHomeMarker(Marker *marker)
{
    return marker->isHome;
}