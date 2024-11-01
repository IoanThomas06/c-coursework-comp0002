#define _USE_MATH_DEFINES
#include "display.h"
#include "position.h"
#include "marker.h"
#include "robot.h"
#include "map.h"
#include "allocations.h"
#include "./drawapp/graphics.h"
#include <stddef.h>
#include <math.h>

#define BORDER_WIDTH 1
#define SQUARE_WIDTH display->pixelWidthOfGridSquare

// Static background functions.

static void drawGrid(Display *display, Map *map)
{
    setRGBColour(0x00, 0x00, 0x00);
    for (int i = BORDER_WIDTH; i < display->columnCount + BORDER_WIDTH; i++)
    {
        drawLine(i * SQUARE_WIDTH, 0, i * SQUARE_WIDTH, display->rowCount);
    }
    for (int i = BORDER_WIDTH; i < display->rowCount + BORDER_WIDTH; i++)
    {
        drawLine(0, i * SQUARE_WIDTH, display->rowCount, i * SQUARE_WIDTH);
    }
}

static void drawObstacle(Display *display, int row, int column)
{
    fillRect(column * SQUARE_WIDTH, row * SQUARE_WIDTH, SQUARE_WIDTH,
             SQUARE_WIDTH);
}

static void drawObstacles(Display *display, Map *map)
{
    setRGBColour(0xFF, 0x00, 0x00);
    for (size_t row = 0; row < getRowSize(map) + 2 * BORDER_WIDTH; row++)
    {
        for (size_t column = 0; column < getColumnSize(map) + 2 * BORDER_WIDTH;
             column++)
        {
            Position position = {column, row};
            if (row == 0 || row == getRowSize(map) + 1 ||
                column == 0 || column == getColumnSize(map) + 1 ||
                !isMapPositionEmpty(map, position))
            {
                // Note the adjustment of the y, or row, coordinates for the
                // interface provided by drawapp.
                drawObstacle(display, getRowSize(map) - row - 1, column);
            }
        }
    }
}

// Static foreground functions.

void rotateTrianglePointSet(int numberOfRotations, int xPoints[], int yPoints[])
{
    numberOfRotations %= 4;
    for (int i = 0; i < 3; i++)
    {
        if (numberOfRotations == 2 ||
            numberOfRotations == 3)
        {
            yPoints[i] *= -1;
        }
        if (numberOfRotations % 2)
        {
            int temp = xPoints[i];
            xPoints[i] = yPoints[i];
            yPoints[i] = temp;
        }
    }
}

static void drawRobot(Display *display, Robot *robot)
{
    DirectionVector direction = getDirectionVector(robot);
    double rowCorner = (robot->position.y + BORDER_WIDTH) * SQUARE_WIDTH; // display->rowCount - robot->pos
    double columnCorner = (robot->position.x + BORDER_WIDTH) * SQUARE_WIDTH;
    int xPoints[3];
    int yPoints[3];
    if (direction.y == 1)
    {
    }
    else if (direction.y == -1)
    {
    }
    else if (direction.x == 1)
    {
    }
    else if (direction.x == -1)
    {
    }
    setRGBColour(0x00, 0xFF, 0x00);
    drawRegularPolygon(3, );
}

static void drawMarker(Display *display, Marker *marker);

// Function definitions.

Display *initialiseDisplay(size_t rowCount, size_t columnCount,
                           size_t pixelWidthOfGridSquare)
{
    Display *display = (Display *)checkedMalloc(sizeof(Display), "Display");
    display->rowCount = rowCount;
    display->columnCount = columnCount;
    setWindowSize((columnCount + 2 * BORDER_WIDTH) * pixelWidthOfGridSquare,
                  (rowCount + 2 * BORDER_WIDTH) * pixelWidthOfGridSquare);
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
    sleep(500);
    foreground();
    clear();
    drawRobot(display, robot);
    for (size_t i = 0; i < numberOfMarkers; i++)
    {
        drawMarker(display, markers[i]);
    }
}