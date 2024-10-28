#include "position.h"

typedef struct Marker
{
    Position position;
    int isPlaced;
    int isHome;
} Marker;

Marker initialiseMarker(Position initialPosition, int isHome);
int isHomeMarker(Marker marker);