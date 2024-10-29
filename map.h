#include <stddef.h>

typedef struct Map
{
    size_t rowSize;
    size_t columnSize;
    int *mapMatrix;
} Map;

// Map utility functions.

Map *initialiseMap(size_t rowSize, size_t columnSize,
                   void (*mapGenerationFunction)(Map *));
size_t getRowSize(Map *map);
size_t getColumnSize(Map *map);
void setMapPositionValue(Map *map, int row, int column, int value);
int isPositionEmpty(Map *map, int row, int column);

// Map generation functions.

void basicMap(Map *map);
void otherMap(Map *map);