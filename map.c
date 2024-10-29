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

void setMapPositionValue(Map *map, int row, int column, int value)
{
    map->mapMatrix[row * getRowSize(map) + column] = value;
}

int isPositionEmpty(Map *map, int row, int column)
{
    return map->mapMatrix[row * getRowSize(map) + column];
}