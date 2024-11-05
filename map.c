#include "map.h"
#include "position.h"
#include "allocations.h"
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

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

// General map generation utilities.

typedef struct Range
{
    // Inclusive.
    size_t min;
    // Exclusive.
    size_t max;
} Range;

static size_t selectFromRange(Range range)
{
    srand(time(0));
    return range.min + rand() % range.max;
}

static int countMapObstacles(Map *map)
{
    int obstacleCount = 0;
    for (size_t row = 0; row < getRowSize(map); row++)
    {
        for (size_t column = 0; column < getColumnSize(map); column++)
        {
            Position position = {.x = column,
                                 .y = row};
            if (!isMapPositionEmpty(map, position))
            {
                obstacleCount++;
            }
        }
    }
    return obstacleCount;
}

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

static Position getRandomPosition(Map *map)
{
    Range rowRange = {.min = 0, .max = getRowSize(map)};
    Range columnRange = {.min = 0, .max = getColumnSize(map)};
    Position position = {.x = selectFromRange(columnRange),
                         .y = selectFromRange(rowRange)};
    return position;
}

// Specific map generations.

void generateEmptyMap(Map *map)
{
    ;
}

static Position eaterAgent(Map *map, size_t lifetime, Position initialPosition)
{
}

void generateIrregularMap(Map *map)
{
    fillMap(map);
    Position position = getRandomPosition(map);
    do
    {
        position = (map, getRowSize(map) * getColumnSize(map), position);
    } while (countMapObstacles(map) < getRowSize(map) * getColumnSize(map) / 3);
}