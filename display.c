#include "display.h"
#include "./drawapp/graphics.h"

// Static background functions.

static void drawGrid(Display *display, Map *map);
static void fillObstacles(Display *display, Map *map);

// Static foreground functions.

static void drawRobot(Display *display, Robot *robot);
static void drawMarker(Display *display, Marker *marker);

// Function definitions.

Display *initialiseDisplay(size_t xSize, size_t ySize,
                           size_t pixelWidthOfGridSquare); // remember to do setwindowsize
void drawBackground(Display *display, Map *map);           // remember the flips on foreground() and background()
void updateForeground(Display *display, Robot *robot, Marker *markers[],
                      int numberOfMarkers);