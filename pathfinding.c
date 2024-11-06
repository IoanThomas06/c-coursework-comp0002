#include "pathfinding.h"
#include "position.h"
#include "direction.h"
#include "map.h"
#include "allocations.h"
#include <stddef.h>
#include <stdlib.h>

static Path *initialisePath(Map *map)
{
    Path *path = (Path *)checkedMalloc(sizeof(Path), "Path");

    path->path = (Position *)checkedMalloc(sizeof(Position) *
                                               countMapEmptySpace(map),
                                           "'path' in Path");

    path->currentPoint = 0;
    path->pathLength = 0;
}

static void deallocatePath(Path *path)
{
    free(path->path);
    free(path);
}

static int linearSearch(Position target, Position *positions, size_t count)
{
    for (--count; count >= 0; count--)
    {
        if (comparePositionValues(target, positions[count]))
        {
            return 1;
        }
    }
    return 0;
}

static void pushToPath(Path *path, Position position)
{
    path->path[path->pathLength++] = position;
}

static void popFromPath(Path *path)
{
    path->pathLength--;
}

// BFS and related utilities.

typedef struct Visited
{
    Position *visited;
    size_t visitedCount;
} Visited;

static Visited *initialiseVisited(Map *map)
{
    Visited *visited = (Visited *)checkedMalloc(sizeof(Visited), "Visited");

    visited->visited = (Position *)checkedCalloc(countMapEmptySpace(map),
                                                 sizeof(Position),
                                                 "'visited' in Visited");

    visited->visitedCount = countMapEmptySpace(map);

    return visited;
}

static void deallocateVisited(Visited *visited)
{
    free(visited->visited);
    free(visited);
}

static Visited *breadthFirstSearch(Map *map, Path *path,
                                   Position startPosition, Position target)
{
    int qRear = 0;
    int qFront = 0;
    Position *queue = (Position *)checkedCalloc(countMapEmptySpace(map),
                                                sizeof(Position),
                                                "'queue' in generatePath");

    Visited *visited = initialiseVisited(map);

    Position current;

    visited->visited[visited->visitedCount++] = queue[++qRear] = startPosition;

    while (qFront != qRear &&
           !comparePositionValues(current = queue[++qFront], target))
    {
        for (size_t i = 0; i < 4; i++)
        {
            Position adjacent = addDirectionToPosition(current,
                                                       generateDirectionVector(i));
            if (isMapPositionEmpty(map, adjacent) &&
                !linearSearch(adjacent, visited, visited->visitedCount))
            {
                visited->visited[visited->visitedCount++] =
                    queue[++qRear] = adjacent;
            }
        }
    }

    free(queue);
    return visited;
}

static void traceVisited(Visited *visited, Position target)
{
    Position current = visited->visited[visited->visitedCount];
    if (comparePositionValues(current, target))
    {
        while (visited->visitedCount-- > 0)
        {
            // check for all directions added to current if it is equal to visited->visited[visited->count]
            // (so use the for 0 to 3 like in bfs)
            // if it is equal, then push to stack, else don't, also remember to push the target to the stack?
        }
    }
}

Path *generatePath(Map *map, Position startPosition, Position endPosition)
{
    Path *path = initialisePath(map);

    breadthFirstSearch(map, path, startPosition, endPosition);

    return path;
}