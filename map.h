typedef struct Map
{
    int xSize;
    int ySize;
    int *mapMatrix[];
} Map;

Map initialiseMap(Map map, int xSize, int ySize, void (*mapGenerationFunction)(Map *map));
int getPositionState(int x, int y);