#include "display.h"
#include "marker.h"
#include "robot.h"
#include "map.h"
#include "position.h"
#include "allocations.h"
#include "./drawapp/graphics.h"
#include <stddef.h>

// take these out, put as functions operating on display, and add borderwidth property to display
#define BORDER_WIDTH 1
#define ROBOT_VERTICES 3
#define SQUARE_WIDTH display->pixelWidthOfGridSquare
#define WINDOW_WIDTH (display->columnCount + 2 * BORDER_WIDTH) * SQUARE_WIDTH
#define WINDOW_HEIGHT (display->rowCount + 2 * BORDER_WIDTH) * SQUARE_WIDTH

// Static drawing general utility functions.

/*
    Used to decouple from the dependance on drawapp interface.
    Subsequent drawing implementations abstract the need to program to drawapp
    implementation.
*/
static int formatYCoordinate(Display *display, int yPoint)
{
    return WINDOW_HEIGHT - yPoint;
}

static void drawLineToDisplay(Display *display, int x1, int y1, int x2, int y2)
{
    drawLine(x1, formatYCoordinate(display, y1),
             x2, formatYCoordinate(display, y2));
}

static void fillRectOnDisplay(Display *display, int x1, int y1, int width,
                              int height)
{
    fillRect(x1, formatYCoordinate(display, y1) - height, width, height);
}

static void fillPolygonOnDisplay(Display *display, size_t count, int x[], int y[])
{
    for (size_t i = 0; i < count; i++)
    {
        y[i] = formatYCoordinate(display, y[i]);
    }
    fillPolygon(count, x, y);
}

// Static background functions.

static void drawColumns(Display *display, Map *map)
{
    for (int i = BORDER_WIDTH; i < display->columnCount + BORDER_WIDTH; i++)
    {
        drawLineToDisplay(display, i * SQUARE_WIDTH, 0, i * SQUARE_WIDTH,
                          display->rowCount);
    }
}

static void drawRows(Display *display, Map *map)
{
    for (int i = BORDER_WIDTH; i < display->rowCount + BORDER_WIDTH; i++)
    {
        drawLineToDisplay(display, 0, i * SQUARE_WIDTH, display->rowCount,
                          i * SQUARE_WIDTH);
    }
}

static void drawGrid(Display *display, Map *map)
{
    drawColumns(display, map);
    drawRows(display, map);
}

static void drawObstacle(Display *display, int row, int column)
{
    fillRectOnDisplay(display, column * SQUARE_WIDTH, row * SQUARE_WIDTH,
                      SQUARE_WIDTH, SQUARE_WIDTH);
}

static void drawObstacles(Display *display, Map *map)
{
    for (size_t row = 0; row < getRowSize(map) + 2 * BORDER_WIDTH; row++)
    {
        for (size_t column = 0; column < getColumnSize(map) + 2 * BORDER_WIDTH;
             column++)
        {
            Position position = {column - BORDER_WIDTH, row - BORDER_WIDTH};
            if (row < BORDER_WIDTH ||
                row >= getRowSize(map) + BORDER_WIDTH ||
                column < BORDER_WIDTH ||
                column >= getColumnSize(map) + BORDER_WIDTH ||
                !isMapPositionEmpty(map, position))
            {
                drawObstacle(display, row, column);
            }
        }
    }
}

// Static foreground functions, and relevant static utilities.

static void swapXY(size_t i, int xPoints[], int yPoints[])
{
    int temp = xPoints[i];
    xPoints[i] = yPoints[i];
    yPoints[i] = xPoints[i];
}

static void rotatePointSets(int numberOfRotations, int xPoints[],
                            int yPoints[])
{

    // Handles any values < -1 or > 4 of rotation amount.
    // i.e. rotation amount will be 0, 1, 2, or 3.
    numberOfRotations = ((numberOfRotations % 4) + 4) % 4;
    for (int i = 0; i < ROBOT_VERTICES; i++)
    {
        if (numberOfRotations > 1)
        {
            yPoints[i] *= -1;
        }
        if (numberOfRotations % 2)
        {
            swapXY(i, xPoints, yPoints);
        }
    }
}

static void generateTriangleRobotPoints(int squareWidth, int xPoints[],
                                        int yPoints[])
{
    for (size_t i = 0; i < ROBOT_VERTICES; i++)
    {
        xPoints[i] = i * squareWidth / 3;
    }
    yPoints[0] = yPoints[2] = 0;
    yPoints[1] = squareWidth / 2;
}

static void generateRobotPointSet(Robot *robot, int squareWidth, int xPoints[],
                                  int yPoints[])
{
    generateTriangleRobotPoints(squareWidth, xPoints, yPoints);
    rotatePointSets(getRotationalOffset(robot), xPoints, yPoints);
    for (size_t i = 0; i < ROBOT_VERTICES; i++)
    {
        xPoints[i] += (robot->position.y + BORDER_WIDTH) * squareWidth;
        yPoints[i] += (robot->position.x + BORDER_WIDTH) * squareWidth;
    }
}

static void drawRobot(Display *display, Robot *robot)
{
    int xPoints[ROBOT_VERTICES];
    int yPoints[ROBOT_VERTICES];
    generateRobotPointSet(robot, SQUARE_WIDTH, xPoints, yPoints);
    fillPolygonOnDisplay(display, 3, xPoints, yPoints);
}

static void drawMarker(Display *display, Marker *marker)
{
    fillRectOnDisplay(display,
                      (getMarkerPosition(marker).x + BORDER_WIDTH) *
                          SQUARE_WIDTH,
                      (getMarkerPosition(marker).y + BORDER_WIDTH) *
                          SQUARE_WIDTH,
                      SQUARE_WIDTH, SQUARE_WIDTH);
}

// Header function definitions.

Display *initialiseDisplay(size_t rowCount, size_t columnCount,
                           size_t pixelWidthOfGridSquare)
{
    Display *display = (Display *)checkedMalloc(sizeof(Display), "Display");
    display->rowCount = rowCount;
    display->columnCount = columnCount;
    display->pixelWidthOfGridSquare = pixelWidthOfGridSquare;
    setWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    return display;
}

void drawBackground(Display *display, Map *map)
{
    background();
    setRGBColour(0x00, 0x00, 0x00);
    drawGrid(display, map);
    setRGBColour(0xFF, 0x00, 0x00);
    drawObstacles(display, map);
}

void updateForeground(Display *display, Robot *robot, Marker *markers[],
                      int numberOfMarkers)
{
    sleep(500);
    foreground();
    clear();
    setRGBColour(0x00, 0xFF, 0x00);
    drawRobot(display, robot);
    setRGBColour(0x00, 0x00, 0xFF);
    for (size_t i = 0; i < numberOfMarkers; i++)
    {
        drawMarker(display, markers[i]);
    }
}