#include "map.h"
#include "position.h"
#include "allocations.h"
#include <stddef.h>
#include <stdlib.h>

Map *initialiseMap(size_t rowSize, size_t columnSize,
                   void (*mapGenerationFunction)(Map *))
{
    Map *map = (Map *)checkedMalloc(sizeof(Map), "Map");

    map->rowSize = rowSize;
    map->columnSize = columnSize;
    map->mapMatrix = (int *)checkedCalloc(rowSize * columnSize, sizeof(int),
                                          "'mapMatrix' in Map");

    mapGenerationFunction(map);

    return map;
}

void deallocateMap(Map *map)
{
    free(map->mapMatrix);
    free(map);
}

size_t getRowSize(Map *map)
{
    return map->rowSize;
}

size_t getColumnSize(Map *map)
{
    return map->columnSize;
}

static int convertMapRowToArray(Map *map, size_t row)
{
    return getRowSize(map) - row - 1;
}

void setMapPositionValue(Map *map, Position position, int value)
{
    map->mapMatrix[convertMapRowToArray(map, position.y) * getRowSize(map) +
                   position.x] = value;
}

int isMapPositionEmpty(Map *map, Position position)
{
    return !map->mapMatrix[convertMapRowToArray(map, position.y) *
                               getRowSize(map) +
                           position.x];
}

// Map generation functions.

// An empty map.

void generateEmptyMap(Map *map)
{
    ;
}

/*
    "2D Grid Map Generation for Deep-Learning-based Navigation Approaches"

    Link: https://arxiv.org/pdf/2110.13242

    Authors: Authors: Gabriel O. Flores-Aquino, Jheison Duvier Díaz Ortega,
    Ricardo Yahir Almazan Arvizu, Raúl López Muñoz, O. Octavio Gutierrez-Frias,
    J. Irving Vasquez-Gomez

    My implementation is reduced in the variety of maps which can be produced.
*/

static void fillMap(Map *map)
{
    for (size_t row = 0; row < getRowSize(map); row++)
    {
        for (size_t column = 0; column < getColumnSize(map); column++)
        {
            Position position = {.x = column,
                                 .y = row};
            setMapPositionValue(map, position, 1);
        }
    }
}

static void clearRoom(Map *map, Position seed, size_t roomWidth,
                      size_t roomHeight)
{
    for (size_t row = seed.y - roomHeight / 2; row < seed.y + roomHeight / 2;
         row++)
    {
        if (row >= 0 && row < getRowSize(map))
        {
            for (size_t column = seed.x - roomWidth;
                 column < seed.x + roomWidth; column++)
            {
                if (column >= 0 && column < getColumnSize(map))
                {
                    Position position = {.x = column, .y = row};
                    setMapPositionValue(map, position, 0);
                }
            }
        }
    }
}

static void seedRooms(Map *map, size_t numberOfRooms, size_t roomWidthLimit[],
                      size_t roomHeightLimit[], Position seedPoints[])
{
    for (size_t i = 0; i < numberOfRooms; i++)
    {
        Position seed = {.x = rand() % getColumnSize(map),
                         .y = rand() % getRowSize(map)};
        size_t roomWidth = rand() % (roomWidthLimit[1] + 1) + roomWidthLimit[0];
        size_t roomHeight = rand() % (roomHeightLimit[1] + 1) +
                            roomHeightLimit[0];
        seedPoints[i] = seed;
        clearRoom(map, seed, roomWidth, roomHeight);
    }
}

static void connectRooms(Map *map, Position roomA, Position roomB,
                         size_t tunnelWidth)
{
    for (size_t column = roomA.x; column <= roomB.x; column++)
    {
        for (size_t row = roomA.y - tunnelWidth / 2;
             row < roomA.y + tunnelWidth; row++)
        {
            if (row >= 0 && row < getRowSize(map))
            {
                Position position = {.x = column, .y = row};
                setMapPositionValue(map, position, 0);
            }
        }
    }

    for (size_t column = roomB.x - tunnelWidth / 2;
         column <= roomB.x + tunnelWidth; column++)
    {
        if (column >= 0 && column < getColumnSize(map))
        {
            for (size_t row = roomA.y; row < roomB.y; row++)
            {

                Position position = {.x = column, .y = row};
                setMapPositionValue(map, position, 0);
            }
        }
    }
}

static void connectAllRooms(Map *map, size_t numberOfRooms,
                            Position seedPoints[], size_t tunnelWidth)
{
    for (int i = 0; i < numberOfRooms - 1; i++)
    {
        connectRooms(map, seedPoints[i], seedPoints[i + 1], tunnelWidth);
    }
}

void generateIrregularMap(Map *map)
{
    size_t numberOfRooms = 5;

    // Below limit arrays in the form inclusive {min, max}.
    size_t roomHeightLimit[2] = {
        getRowSize(map) / numberOfRooms,
        getRowSize(map) / numberOfRooms + 3};
    size_t roomWidthLimit[2] = {
        getColumnSize(map) / numberOfRooms,
        getColumnSize(map) / numberOfRooms + 3};

    size_t tunnelWidth = 1 + rand() % 3;
    Position seedPoints[numberOfRooms];

    fillMap(map);

    seedRooms(map, numberOfRooms, roomWidthLimit, roomHeightLimit, seedPoints);

    connectAllRooms(map, numberOfRooms, seedPoints, tunnelWidth);
}