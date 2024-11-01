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
    // Use of calloc in initialiseMap means that map positions are initialised
    // to 0, due to how the map generations will be implemented, it is most
    // convenient that 0 represents an obstacle.
    return map->mapMatrix[convertMapRowToArray(map, position.y) *
                              getRowSize(map) +
                          position.x];
}