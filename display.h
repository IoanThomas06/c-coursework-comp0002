#include "marker.h"
#include "robot.h"
#include "map.h"
#include <stddef.h>

#ifndef DISPLAY
#define DISPLAY

typedef struct Display
{
    size_t rowCount;
    size_t columnCount;
    size_t pixelWidthOfGridSquare;
} Display;

Display *initialiseDisplay(size_t, size_t, size_t);
void drawBackground(Display *, Map *);
void updateForeground(Display *, Robot *, Marker *[], int);

#endif