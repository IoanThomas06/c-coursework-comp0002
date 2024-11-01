#include "display.h"
#include "marker.h"
#include "robot.h"
#include "map.h"
#include "allocations.h"
#include "./drawapp/graphics.h"
#include <stddef.h>

// Static background functions.

static void drawGrid(Display *display, Map *map);
static void drawObstacle(Display *display, int row, int column);
static void drawObstacles(Display *display, Map *map);

// Static foreground functions.

static void drawRobot(Display *display, Robot *robot);
static void drawMarker(Display *display, Marker *marker);

// Function definitions.

Display *initialiseDisplay(size_t rowCount, size_t columnCount,
                           size_t pixelWidthOfGridSquare)
{
    Display *display = (Display *)checkedMalloc(sizeof(Display), "Display");
    display->rowCount = rowCount;
    display->columnCount = columnCount;
    setWindowSize((columnCount + 2) * pixelWidthOfGridSquare,
                  (rowCount + 2) * pixelWidthOfGridSquare);
    return display;
}

void drawBackground(Display *display, Map *map)
{
    background();
    drawGrid(display, map);
    drawObstacles(display, map);
}

void updateForeground(Display *display, Robot *robot, Marker *markers[],
                      int numberOfMarkers)
{
    foreground();
    clear();
    drawRobot(display, robot);
    for (size_t i = 0; i < numberOfMarkers; i++)
    {
        drawMarker(display, markers[i]);
    }
}