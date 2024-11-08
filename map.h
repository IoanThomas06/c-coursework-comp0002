#include "position.h"
#include <stddef.h>
#include <stdbool.h>

#ifndef MAP_H
#define MAP_H

typedef struct Map
{
    size_t rowSize;
    size_t columnSize;
    // Obstacles represented by 1.
    bool *mapMatrix;
} Map;

// Map utility functions.

Map *initialiseMap(size_t, size_t,
                   void (*mapGenerationFunction)(Map *));
void deallocateMap(Map *map);
size_t getRowSize(Map *);
size_t getColumnSize(Map *);
int isMapPositionValid(Map *, Position);
void setMapPositionValue(Map *, Position, bool);
int isMapPositionEmpty(Map *, Position);
size_t countMapEmptySpace(Map *);

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