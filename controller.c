#include "controller.h"
#include "allocations.h"
#include "robot.h"
#include "map.h"
#include "marker.h"
#include "display.h"
#include "pathfinding.h"
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

static void initialiseMarkers(Controller *controller)
{
    controller->markers = (Marker **)checkedMalloc(controller->numberOfMarkers *
                                                       sizeof(Marker *),
                                                   "'markers' in Controller");

    if (controller->numberOfMarkers)
    {
        controller->markers[0] = initialiseMarker(getEmptyRandomPosition(
                                                      controller->map),
                                                  1);
    }

    for (size_t i = 1; i < controller->numberOfMarkers; i++)
    {
        controller->markers[i] = initialiseMarker(getEmptyRandomPosition(
                                                      controller->map),
                                                  0);
    }
}

Controller *initialiseController(size_t squarePixelWidth, size_t mapRows,
                                 size_t mapColumns, size_t numberOfMarkers,
                                 void (*mapGenerationFunction)(Map *))
{
    srand(time(NULL));

    Controller *controller = (Controller *)checkedMalloc(sizeof(Controller),
                                                         "Controller");

    controller->map = initialiseMap(mapRows, mapColumns, mapGenerationFunction);

    controller->robot = initialiseRobot(getEmptyRandomPosition(controller->map),
                                        rand() % 4,
                                        numberOfMarkers);
    // Dependent on map allocation.
    controller->numberOfMarkers = numberOfMarkers;
    initialiseMarkers(controller);

    controller->display = initialiseDisplay(mapRows, mapColumns,
                                            squarePixelWidth, 1);
    return controller;
}

Controller *parseControllerArguments(size_t argc, char **argv)
{
    return initialiseController(
        // Pixel size of grid squares.
        (argc > 1 && atoi(argv[1]) > 0) ? atoi(argv[1]) : 20,

        // Maximum map row size.
        (argc > 3 && atoi(argv[2]) > 5) ? atoi(argv[2]) : 20,

        // Maximum map column size.
        (argc > 3 && atoi(argv[3]) > 5) ? atoi(argv[3]) : 20,

        // Marker count.
        (argc > 4 && atoi(argv[4]) > 0) ? atoi(argv[4]) : 5,

        // Map generation type.
        (argc > 5 && strcmp(argv[5], "basic")) ? generateEatenMap
                                               : generateEmptyMap);
}

void deallocateController(Controller *controller)
{
    deallocateDisplay(controller->display);
    deallocateMap(controller->map);
    deallocateRobot(controller->robot);

    for (size_t i = 0; i < controller->numberOfMarkers; i++)
    {
        deallocateMarker(controller->markers[i]);
    }
    free(controller->markers);

    free(controller);
}

void run(Controller *controller)
{
    drawBackground(controller->display, controller->map);
    for (size_t i = 0; i < controller->map->columnSize; i++)
    {
        for (size_t j = 0; j < controller->map->rowSize; j++)
        {
            Position p = {.x = i,
                          .y = j};
        }
    }
    updateForeground(controller->display, controller->robot,
                     controller->markers, controller->numberOfMarkers);

    /*for (size_t i = 1; i < controller->numberOfMarkers; i++)
    {
        Path *path = generatePath(controller->map,
                                  getRobotPosition(controller->robot),
                                  controller->markers[i]->position);

        while (path->pathLength > 0)
        {
            updateForeground(controller->display, controller->robot,
                             controller->markers, 0);

            ;

            int turnValue = getRelativeTurnBetweenNeswDirections(
                generateDirectionVector(getRotationalOffset(controller->robot)),
                getDirectionBetweenPositions(
                    getNextPosition(path),
                    getRobotPosition(controller->robot)));

            (turnValue && turnValue == 1) ? right(controller->robot)
                                          : left(controller->robot);

            if (canMoveForward(controller->robot, controller->map))
            {
                forward(controller->robot);
            }
            else
            {
                printf("Robot could not move forward.");
            }
        }

    }*/
}