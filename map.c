#include "map.h"
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

static int mapToArrayRowCoordinates(Map *map, size_t row)
{
    return getRowSize(map) - row - 1;
}

void setMapPositionValue(Map *map, size_t row, size_t column, int value)
{
    map->mapMatrix[mapToArrayRowCoordinates(map, row) * getRowSize(map) +
                   column] = value;
}

int isMapPositionEmpty(Map *map, size_t row, size_t column)
{
    return map->mapMatrix[mapToArrayRowCoordinates(map, row) * getRowSize(map) +
                          column];
}