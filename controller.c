#include "controller.h"
#include "allocations.h"
#include "robot.h"
#include "map.h"
#include "marker.h"
#include "display.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Controller *initialiseController(size_t squarePixelWidth, size_t mapRows,
                                 size_t mapColumns, size_t numberOfMarkers,
                                 void (*mapGenerationFunction)(Map *))
{
    Controller *controller = (Controller *)checkedMalloc(sizeof(Controller),
                                                         "Controller");
    controller->markers = (Marker **)checkedCalloc(numberOfMarkers,
                                                   sizeof(Marker *),
                                                   "'markers' in Controller");

    controller->numberOfMarkers = numberOfMarkers;

    controller->map = initialiseMap(mapRows, mapColumns, mapGenerationFunction);
    controller->display = initialiseDisplay(mapRows, mapColumns,
                                            squarePixelWidth, 1);

    Position p = {0, 0};
    controller->robot = initialiseRobot(p, 0, numberOfMarkers);

    return controller;
}

Controller *parseControllerArguments(size_t argc, char **argv)
{
    return initialiseController(
        // Pixel size of grid squares.
        (argc > 1) ? atoi(argv[1]) : 20,

        // Maximum map row size.
        (argc > 3) ? atoi(argv[2]) : 20,

        // Maximum map column size.
        (argc > 3) ? atoi(argv[3]) : 20,

        // Marker count.
        (argc > 4) ? atoi(argv[4]) : 5,

        // Map generation type.
        (argc > 5 && strcmp(argv[5], "basic")) ? generateIrregularMap
                                               : generateEmptyMap);
}

void deallocateController(Controller *controller)
{
    deallocateDisplay(controller->display);
    deallocateMap(controller->map);
    deallocateRobot(controller->robot);
    free(controller->markers);
    free(controller);
}

void run(Controller *controller)
{
    drawBackground(controller->display, controller->map);
    updateForeground(controller->display, controller->robot, controller->markers, 0);
}