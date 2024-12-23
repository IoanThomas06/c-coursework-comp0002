#include "display.h"
#include "marker.h"
#include "robot.h"
#include "map.h"
#include "position.h"
#include "allocations.h"
#include "graphics.h"
#include <stddef.h>
#include <stdlib.h>

#include <stdio.h>

#define ROBOT_VERTICES 3

/*
    Some frequently used expressions.
    Require a local scope variable 'Display *display'.
*/

#define BORDER_WIDTH display->borderWidth
#define SQUARE_WIDTH display->pixelWidthOfGridSquare
#define WINDOW_WIDTH (display->columnCount + 2 * BORDER_WIDTH) * SQUARE_WIDTH
#define WINDOW_HEIGHT (display->rowCount + 2 * BORDER_WIDTH) * SQUARE_WIDTH

// Static type definitions.

struct Display
{
    size_t rowCount;
    size_t columnCount;
    size_t pixelWidthOfGridSquare;
    size_t borderWidth;
};

// End of static type definitions.

// Static declarations.

static int formatYCoordinate(Display *, int);
static void drawLineToDisplay(Display *, int, int, int, int);
static void fillRectOnDisplay(Display *, int, int, int, int);
static void fillPolygonOnDisplay(Display *, size_t, int[], int[]);
static void setDisplayWindowSize(Display *);
static void editDisplayForeground();
static void editDisplayBackground();
static void setDisplayDrawColour(int, int, int);
static void delayDisplayUpdate(int);

// Static background functions.

static void drawColumns(Display *, Map *);
static void drawRows(Display *, Map *);
static void drawGrid(Display *, Map *);
static void drawObstacle(Display *, int, int);
static void drawObstacles(Display *, Map *);

// Static foreground functions, and relevant static utilities.

static void swapXY(size_t, int[], int[]);
static void rotatePointSets(int, int, int, int[], int[]);
static void generateTriangleRobotPoints(int, int[], int[]);
static void generateRobotPointSet(Display *, Robot *, int[], int[]);
static void drawRobot(Display *, Robot *);
static void drawMarker(Display *, Marker *);
static void drawMarkers(Display *, Marker *[], size_t);

// End of static declarations.

// Static drawing general utility functions.

/*
    Used to decouple from the dependance on drawapp interface.
    Subsequent drawing implementations abstract the need to program to drawapp
    interface.
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

static void fillPolygonOnDisplay(Display *display, size_t count, int x[],
                                 int y[])
{
    for (size_t i = 0; i < count; i++)
    {
        y[i] = formatYCoordinate(display, y[i]);
    }

    fillPolygon(count, x, y);
}

static void setDisplayWindowSize(Display *display)
{
    setWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
}

static void editDisplayForeground()
{
    foreground();
}

static void editDisplayBackground()
{
    background();
}

static void setDisplayDrawColour(int red, int green, int blue)
{
    setRGBColour(red, green, blue);
}

static void delayDisplayUpdate(int milliseconds)
{
    sleep(milliseconds);
}

// End of static drawing general utility functions.

// Static background functions.

static void drawColumns(Display *display, Map *map)
{
    for (size_t i = BORDER_WIDTH; i < display->columnCount + 2 * BORDER_WIDTH;
         i++)
    {
        drawLineToDisplay(display, i * SQUARE_WIDTH, 0, i * SQUARE_WIDTH,
                          (display->rowCount + BORDER_WIDTH) * SQUARE_WIDTH);
    }
}

static void drawRows(Display *display, Map *map)
{
    for (size_t i = BORDER_WIDTH; i < display->rowCount + 2 * BORDER_WIDTH; i++)
    {
        drawLineToDisplay(display, 0, i * SQUARE_WIDTH,
                          (display->columnCount + BORDER_WIDTH) * SQUARE_WIDTH,
                          i * SQUARE_WIDTH);
    }
}

static void drawGrid(Display *display, Map *map)
{
    setDisplayDrawColour(0x00, 0x00, 0x00);
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
    setDisplayDrawColour(0xFF, 0x00, 0x00);

    for (size_t row = 0; row < getRowSize(map) + 2 * BORDER_WIDTH; row++)
    {
        for (size_t column = 0; column < getColumnSize(map) + 2 * BORDER_WIDTH;
             column++)
        {
            Position position = {.x = column - BORDER_WIDTH,
                                 .y = row - BORDER_WIDTH};

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

// End of static background functions.

// Static foreground functions, and relevant static utilities.

static void swapXY(size_t i, int xPoints[], int yPoints[])
{
    int temp = xPoints[i];
    xPoints[i] = yPoints[i];
    yPoints[i] = temp;
}

static void rotatePointSets(int squareWidth, int numberOfRotations,
                            int numberOfPoints, int xPoints[], int yPoints[])
{
    // Handles any values < -1 or > 4 of rotation amount.
    // i.e. rotation amount will be 0, 1, 2, or 3.
    numberOfRotations = ((numberOfRotations % 4) + 4) % 4;

    for (int i = 0; i < numberOfPoints; i++)
    {
        if (numberOfRotations > 1)
        {
            yPoints[i] *= -1;
            yPoints[i] += squareWidth;
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
    for (size_t i = 0; i < 3; i++)
    {
        xPoints[i] = i * squareWidth / 2;
    }

    yPoints[0] = yPoints[2] = 0;
    yPoints[1] = squareWidth;
}

static void generateRobotPointSet(Display *display, Robot *robot,
                                  int xPoints[], int yPoints[])
{
    generateTriangleRobotPoints(SQUARE_WIDTH, xPoints, yPoints);

    rotatePointSets(SQUARE_WIDTH, getRotationalOffset(robot), ROBOT_VERTICES,
                    xPoints, yPoints);

    for (size_t i = 0; i < ROBOT_VERTICES; i++)
    {
        xPoints[i] += (getRobotPosition(robot).x + BORDER_WIDTH) * SQUARE_WIDTH;
        yPoints[i] += (getRobotPosition(robot).y + BORDER_WIDTH) * SQUARE_WIDTH;
    }
}

static void drawRobot(Display *display, Robot *robot)
{
    int xPoints[ROBOT_VERTICES];
    int yPoints[ROBOT_VERTICES];
    generateRobotPointSet(display, robot, xPoints, yPoints);
    fillPolygonOnDisplay(display, ROBOT_VERTICES, xPoints, yPoints);
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

static void drawMarkers(Display *display, Marker *markers[],
                        size_t numberOfMarkers)
{
    Marker *home;

    setDisplayDrawColour(0x00, 0x00, 0xFF);
    for (size_t i = 0; i < numberOfMarkers; i++)
    {
        if (isHomeMarker(markers[i]))
        {
            home = markers[i];
        }
        else if (isPlacedMarker(markers[i]))
        {
            drawMarker(display, markers[i]);
        }
    }

    setDisplayDrawColour(0x75, 0x3F, 0xEA);
    drawMarker(display, home);
}

// End of static foreground functions, and relevant static utilities.

// Public function definitions.

Display *initialiseDisplay(size_t rowCount, size_t columnCount,
                           size_t pixelWidthOfGridSquare, size_t borderWidth)
{
    Display *display = (Display *)checkedMalloc(sizeof(Display), "Display");
    display->rowCount = rowCount;
    display->columnCount = columnCount;
    display->pixelWidthOfGridSquare = pixelWidthOfGridSquare;
    display->borderWidth = borderWidth;

    setDisplayWindowSize(display);

    return display;
}

void deallocateDisplay(Display *display)
{
    free(display);
}

void drawBackground(Display *display, Map *map)
{
    editDisplayBackground();

    drawGrid(display, map);

    drawObstacles(display, map);
}

void updateForeground(Display *display, Robot *robot, Marker *markers[],
                      size_t numberOfMarkers, int delayMilliseconds)
{
    delayDisplayUpdate(delayMilliseconds);
    editDisplayForeground();
    clear();

    drawMarkers(display, markers, numberOfMarkers);

    setDisplayDrawColour(0x00, 0xFF, 0x00);
    drawRobot(display, robot);
}

// End of public function definitions.