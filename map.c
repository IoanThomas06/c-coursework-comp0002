#include "map.h"
#include "position.h"
#include "direction.h"
#include "allocations.h"
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

Map *initialiseMap(size_t rowSize, size_t columnSize,
                   void (*mapGenerationFunction)(Map *))
{
    Map *map = (Map *)checkedMalloc(sizeof(Map), "Map");

    map->mapMatrix = (int **)checkedMalloc(rowSize * sizeof(int *),
                                           "'mapMatrix' row in Map");
    for (size_t i = 0; i < rowSize; i++)
    {
        map->mapMatrix[i] = (int *)checkedCalloc(columnSize, sizeof(int),
                                                 "'mapMatrix' column in Map");
    }

    map->rowSize = rowSize;
    map->columnSize = columnSize;

    mapGenerationFunction(map);

    return map;
}

void deallocateMap(Map *map)
{
    for (size_t i = 0; i < getRowSize(map); i++)
    {
        free(map->mapMatrix[i]);
    }
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

static int isMapPositionValid(Map *map, Position position)
{
    if (position.y >= 0 && position.y < getRowSize(map) &&
        position.x >= 0 && position.x < getColumnSize(map))
    {
        return 1;
    }
    return 0;
}

static void checkPositionInMapError(Map *map, Position position,
                                    char callingFunction[])
{
    if (!isMapPositionValid(map, position))
    {
        printf("call: %s: Position out of map bounds.\n"
               "Attempted to index row: %d column: %d",
               callingFunction, position.y, position.x);
        exit(EXIT_FAILURE);
    }
}

static int convertMapRowToArray(Map *map, size_t row)
{
    return getRowSize(map) - row - 1;
}

void setMapPositionValue(Map *map, Position position, int value)
{
    checkPositionInMapError(map, position, "setMapPosition");

    map->mapMatrix[convertMapRowToArray(map, position.y)][position.x] = value;
}

int isMapPositionEmpty(Map *map, Position position)
{
    checkPositionInMapError(map, position, "isMapPositionEmpty");

    return !map->mapMatrix[convertMapRowToArray(map, position.y)][position.x];
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

static DirectionVector getRandomDirection()
{
    return generateDirectionVector(rand() % 4);
}

static Position getRandomAdjacentPosition(Map *map, Position position)
{
    Position tempPosition;
    unsigned int randomDirection = abs(rand());

    do
    {
        randomDirection %= 4;
        DirectionVector next = generateDirectionVector(randomDirection++);
        tempPosition = addDirectionToPosition(position, next);
    } while (!isMapPositionValid(map, tempPosition));

    return tempPosition;
}

// Specific map generations.

void generateEmptyMap(Map *map)
{
    ;
}

// The 'eater algorithm', designed by me.

static Position eaterAgent(Map *map, size_t lifetime, Position position)
{
    while (lifetime-- > 0)
    {
        position = getRandomAdjacentPosition(map, position);

        setMapPositionValue(map, position, 0);
    }

    return position;
}

void generateEatenMap(Map *map)
{
    srand(time(NULL));

    fillMap(map);

    eaterAgent(map, 2 * getRowSize(map) * getColumnSize(map),
               getRandomPosition(map));
}