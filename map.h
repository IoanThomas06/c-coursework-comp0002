#include "position.h"
#include <stddef.h>

#ifndef MAP_H
#define MAP_H

typedef struct Map
{
    size_t rowSize;
    size_t columnSize;
    // Obstacles represented by 1.
    int *mapMatrix;
} Map;

// Map utility functions.

Map *initialiseMap(size_t, size_t,
                   void (*mapGenerationFunction)(Map *));
void deallocateMap(Map *map);
size_t getRowSize(Map *);
size_t getColumnSize(Map *);
int isMapPositionValid(Map *, Position);
void setMapPositionValue(Map *, Position, int);
int isMapPositionEmpty(Map *, Position);
int countMapEmptySpace(Map *);

Position getEmptyRandomPosition(Map *);

/*
    Map generation functions.
    Shared function signature:
        - Return type: 'void'
        - Parameters: '1'
        - Parameter types: 'Map *'
*/

void generateEmptyMap(Map *);
void generateEatenMap(Map *);

#endif