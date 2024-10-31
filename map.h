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
void setMapPositionValue(Map *map, size_t row, size_t column, int value);
int isMapPositionEmpty(Map *map, size_t row, size_t column);

// Map generation functions.

void generateBasicMap(Map *map);
void generateOtherMap(Map *map);