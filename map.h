#include "position.h"
#include <stddef.h>

#ifndef MAP_H
#define MAP_H

typedef struct Map
{
    size_t rowSize;
    size_t columnSize;
    // Obstacles represented by 0.
    int *mapMatrix;
} Map;

// Map utility functions.

Map *initialiseMap(size_t, size_t, void (*mapGenerationFunction)(Map *));
void deallocateMap(Map *map);
size_t getRowSize(Map *);
size_t getColumnSize(Map *);
void setMapPositionValue(Map *, Position, int);
int isMapPositionEmpty(Map *, Position);

// Map generation functions.

void generateBasicMap(Map *);
void generateOtherMap(Map *);

#endif