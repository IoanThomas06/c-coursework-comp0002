#include "marker.h"
#include "robot.h"
#include "map.h"
#include <stddef.h>

typedef struct Display
{
    size_t xSize;
    size_t ySize;
    size_t pixelWidthOfGridSquare;
} Display;

Display *initialiseDisplay(size_t xSize, size_t ySize,
                           int pixelWidthOfGridSquare);
void drawBackground(Display *display, Map *map);
void updateForeground(Display *display, Robot *robot, Marker *markers[],
                      int numberOfMarkers);