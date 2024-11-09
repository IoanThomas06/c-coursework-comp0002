#include "map.h"
#include "position.h"
#include "direction.h"
#include "range.h"
#include "allocations.h"
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>

// Static type definitions.

struct Map
{
    size_t rowSize;
    size_t columnSize;
    // Obstacles represented by true.
    bool *mapMatrix;
};

// End of static type definitions.

// Static declarations (does not include specific map generation utilities).

static void checkPositionInMapError(Map *, Position, char[]);
static int convertMapRowToArray(Map *, size_t);
static size_t formatMapIndex(Map *, Position);
static Position getRandomPosition(Map *);
static void fillMap(Map *);
static Position getRandomAdjacentPosition(Map *, Position);

// End of static declarations.

// Utilities: initialising and deallocating, getting member information.

Map *initialiseMap(size_t rowSize, size_t columnSize,
                   void (*mapGenerationFunction)(Map *))
{
    Map *map = (Map *)checkedMalloc(sizeof(Map), "Map");

    map->mapMatrix = (bool *)checkedCalloc(rowSize * columnSize, sizeof(bool),
                                           "'mapMatrix' in Map");

    map->rowSize = rowSize;
    map->columnSize = columnSize;

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

// Utilities: accessing elements in the map.

int isMapPositionValid(Map *map, Position position)
{
    // Does not need to check >= 0, as position is implemented with members of
    // size_t.
    if (position.y < getRowSize(map) && position.x < getColumnSize(map))
    {
        return true;
    }
    return false;
}

static void checkPositionInMapError(Map *map, Position position,
                                    char callingFunction[])
{
    if (!isMapPositionValid(map, position))
    {
        printf("call: %s: Position out of map bounds.\n"
               "Attempted to index row: %zu column: %zu",
               callingFunction, position.y, position.x);
        exit(EXIT_FAILURE);
    }
}

static int convertMapRowToArray(Map *map, size_t row)
{
    return getRowSize(map) - row - 1;
}

static size_t formatMapIndex(Map *map, Position position)
{
    return convertMapRowToArray(map, position.y) * getColumnSize(map) +
           position.x;
}

void setMapPositionValue(Map *map, Position position, bool isObstacle)
{
    checkPositionInMapError(map, position, "setMapPosition");

    map->mapMatrix[formatMapIndex(map, position)] = isObstacle;
}

int isMapPositionEmpty(Map *map, Position position)
{
    checkPositionInMapError(map, position, "isMapPositionEmpty");

    return !map->mapMatrix[formatMapIndex(map, position)];
}

// Utilities: getting map information.

size_t countMapEmptySpace(Map *map)
{
    size_t emptyCount = 0;
    for (size_t row = 0; row < getRowSize(map); row++)
    {
        for (size_t column = 0; column < getColumnSize(map); column++)
        {
            Position position = {.x = column,
                                 .y = row};
            if (isMapPositionEmpty(map, position))
            {
                emptyCount++;
            }
        }
    }
    return emptyCount;
}

static Position getRandomPosition(Map *map)
{
    Range rowRange = {.min = 0, .max = getRowSize(map)};
    Range columnRange = {.min = 0, .max = getColumnSize(map)};
    Position position = {.x = selectFromRange(columnRange),
                         .y = selectFromRange(rowRange)};
    return position;
}

Position getEmptyRandomPosition(Map *map)
{
    size_t emptyCount = 0;
    Position emptyPositions[getRowSize(map) * getColumnSize(map)];

    for (size_t row = 0; row < getRowSize(map); row++)
    {
        for (size_t column = 0; column < getColumnSize(map); column++)
        {
            Position position = {.x = column,
                                 .y = row};
            if (isMapPositionEmpty(map, position))
            {
                emptyPositions[emptyCount++] = position;
            }
        }
    }

    return emptyPositions[rand() % emptyCount];
}

// End of utilities.

// Map generation utilities.

static void fillMap(Map *map)
{
    for (size_t row = 0; row < getRowSize(map); row++)
    {
        for (size_t column = 0; column < getColumnSize(map); column++)
        {
            Position position = {.x = column,
                                 .y = row};
            setMapPositionValue(map, position, true);
        }
    }
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

// End of map generation utilities.

// Specific map generations.

void generateEmptyMap(Map *map)
{
    ;
}

/*
    Eater Algorithm:
        - Designed by me, although I am unlikely to be the first to use this
          concept.
        - Fills the map completely with obstacles.
        - An 'eater' agent starts at a given position.
        - The agent makes n random moves to new positions,
          limited by it's lifetime.
        - Every position the eater moves to (including the initial position) is
          cleared of obstacles.
        - If a given eater did not sufficiently clear the map, another starts
          where the last terminated. This ensures all cleared positions are
          connected.
*/

static Position eaterAgent(Map *map, size_t lifetime, Position position)
{
    while (lifetime-- > 0)
    {
        position = getRandomAdjacentPosition(map, position);

        setMapPositionValue(map, position, false);
    }

    return position;
}

void generateEatenMap(Map *map)
{
    srand(time(NULL));

    fillMap(map);

    Position position = getRandomPosition(map);
    do
    {
        position = eaterAgent(map, 2 * getRowSize(map) * getColumnSize(map),
                              position);
    } while (countMapEmptySpace(map) <
             getRowSize(map) * getColumnSize(map) / 3);
}

// End of specific map generations.