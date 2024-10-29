#include "position.h"

typedef struct Marker
{
    Position position;
    int isPlaced;
    int isHome;
} Marker;

Marker *initialiseMarker(Position initialPosition, int isHome);
Position getPosition(Marker *marker);
int isPlacedMarker(Marker *marker);
int isHomeMarker(Marker *marker);