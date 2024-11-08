#include "controller.h"
#include "allocations.h"
#include "robot.h"
#include "map.h"
#include "marker.h"
#include "display.h"
#include "pathfinding.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

static bool checkMarkerPositionEquality(Controller *controller,
                                        Position position,
                                        size_t initialisedMarkerCount)
{
    for (size_t i = 0; i < initialisedMarkerCount; i++)
    {
        if (comparePositionValues(getMarkerPosition(controller->markers[i]),
                                  position))
        {
            return false;
        }
    }

    return true;
}

static Position getUniqueRandomPosition(Controller *controller,
                                        size_t initialisedMarkerCount)
{
    Position position;
    do
    {
        position = getEmptyRandomPosition(controller->map);
    } while (comparePositionValues(getRobotPosition(controller->robot),
                                   position) &&
             checkMarkerPositionEquality(controller, position,
                                         initialisedMarkerCount));

    return position;
}

static void initialiseMarkers(Controller *controller)
{
    controller->markers = (Marker **)checkedMalloc(controller->numberOfMarkers *
                                                       sizeof(Marker *),
                                                   "'markers' in Controller");

    if (controller->numberOfMarkers)
    {
        controller->markers[0] = initialiseMarker(getUniqueRandomPosition(
                                                      controller, 0),
                                                  true);
    }

    for (size_t i = 1; i < controller->numberOfMarkers; i++)
    {
        controller->markers[i] = initialiseMarker(getUniqueRandomPosition(
                                                      controller, i),
                                                  false);
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

    // Dependent on map and robot being allocated.
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

// Prerequisites for 'void run(Controller *)' function.

static int getTurnValue(Controller *controller, Position nextPosition)
{
    return getRelativeTurnBetweenDirections(
        generateDirectionVector(
            getRotationalOffset(controller->robot)),
        getDirectionBetweenPositions(
            nextPosition,
            getRobotPosition(controller->robot)));
}

static int getNextTurnValue(Controller *controller, Path *path)
{
    Position nextPosition = getNextPosition(path);

    return getTurnValue(controller, nextPosition);
}

static void parseTurnValue(Controller *controller, int turnValue)
{
    switch (turnValue)
    {
    case 1:
        right(controller->robot);
        break;
    case -1:
        left(controller->robot);
        break;
    case 2:
        right(controller->robot);
        right(controller->robot);
        break;
    default:
        break;
    }
}

static void pickupIfAble(Controller *controller)
{
    if (atMarker(controller->robot, controller->markers,
                 controller->numberOfMarkers) &&
        !atHome(controller->robot, controller->markers,
                controller->numberOfMarkers))
    {
        pickUpMarker(controller->robot, controller->markers,
                     controller->numberOfMarkers);
    }
}

static void followPath(Controller *controller, Path *path)
{
    while (getPathLength(path) > 0)
    {
        parseTurnValue(controller, getNextTurnValue(controller, path));

        updateForeground(controller->display, controller->robot,
                         controller->markers, controller->numberOfMarkers, 200);

        if (canMoveForward(controller->robot, controller->map))
        {
            forward(controller->robot);
        }
        else
        {
            printf("Robot could not move forward.\n");
        }

        pickupIfAble(controller);

        updateForeground(controller->display, controller->robot,
                         controller->markers, controller->numberOfMarkers, 300);
    }
}

// End of prerequisites for 'void run(Controller *)' function.

void run(Controller *controller)
{
    drawBackground(controller->display, controller->map);
    updateForeground(controller->display, controller->robot,
                     controller->markers, controller->numberOfMarkers, 0);

    for (size_t i = 1; i < controller->numberOfMarkers + 1; i++)
    {
        Marker *current = controller->markers[i % controller->numberOfMarkers];

        if (isPlacedMarker(current))
        {
            Path *path = findPath(controller->map,
                                  getRobotPosition(controller->robot),
                                  getMarkerPosition(current));

            followPath(controller, path);

            updateForeground(controller->display, controller->robot,
                             controller->markers, controller->numberOfMarkers,
                             0);
        }
    }
}