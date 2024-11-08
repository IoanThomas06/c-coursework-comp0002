#include "graphics.h"
#include <stddef.h>
#include <stdio.h>

#define ROBOT_VERTICES 3
#define BORDER_WIDTH 1
#define WINDOW_HEIGHT 300
#define WINDOW_WIDTH 500

static int formatYCoordinate(int yPoint)
{
    return WINDOW_HEIGHT - yPoint;
}

static void fillPolygonOnDisplay(size_t count, int x[], int y[])
{
    for (size_t i = 0; i < count; i++)
    {
        y[i] = formatYCoordinate(y[i]);
    }

    fillPolygon(count, x, y);
}

int main()
{
    setWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    drawLine(0, formatYCoordinate(20 * (3 + BORDER_WIDTH)), 100, formatYCoordinate(20 * (3 + BORDER_WIDTH)));

    return 0;
}