#include "pathfinding.h"
#include "position.h"
#include "direction.h"
#include "map.h"
#include "allocations.h"
#include <stddef.h>
#include <stdlib.h>

typedef struct Visited
{
    Position *list;
    size_t count;
} Visited;

// Memory allocation functions.

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

static Visited *initialiseVisited(Map *map)
{
    Visited *visited = (Visited *)checkedMalloc(sizeof(Visited), "Visited");

    visited->list = (Position *)checkedCalloc(countMapEmptySpace(map),
                                              sizeof(Position),
                                              "'list' in Visited");

    visited->count = countMapEmptySpace(map);

    return visited;
}

static void deallocateVisited(Visited *visited)
{
    free(visited->list);
    free(visited);
}

// End of memory allocation functions.

static int linearSearch(Position target, Visited *visited, size_t count)
{
    for (--count; count >= 0; count--)
    {
        if (comparePositionValues(target, visited->list[count]))
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

static Position popFromPath(Path *path)
{
    return path->path[--path->pathLength];
}

// BFS and related utilities.

static Visited *breadthFirstSearch(Map *map, Position startPosition,
                                   Position target)
{
    int qRear = 0;
    int qFront = 0;
    Position *queue = (Position *)checkedCalloc(countMapEmptySpace(map),
                                                sizeof(Position),
                                                "'queue' in generatePath");

    Visited *visited = initialiseVisited(map);

    Position current;

    visited->list[visited->count++] = queue[++qRear] = startPosition;

    while (qFront != qRear &&
           !comparePositionValues(current = queue[++qFront], target))
    {
        for (int i = 0; i < 4; i++)
        {
            Position adjacent =
                addDirectionToPosition(current, generateDirectionVector(i));
            if (isMapPositionValid(map, adjacent) &&
                isMapPositionEmpty(map, adjacent) &&
                !linearSearch(adjacent, visited, visited->count))
            {
                visited->list[visited->count++] = queue[++qRear] = adjacent;
            }
        }
    }

    free(queue);
    return visited;
}

static void traceVisited(Path *path, Visited *visited, Position target)
{
    Position current;
    pushToPath(path, current = visited->list[--visited->count]);

    if (comparePositionValues(current, target))
    {
        while (visited->count-- > 0)
        {
            for (int i = 0; i < 4; i++)
            {
                DirectionVector nextDirection = generateDirectionVector(i);
                Position adjacent = addDirectionToPosition(current,
                                                           nextDirection);

                if (comparePositionValues(visited->list[visited->count],
                                          adjacent))
                {
                    pushToPath(path, current = adjacent);
                }
            }
        }
    }

    deallocateVisited(visited);
}

// End of BFS.

Path *generatePath(Map *map, Position startPosition, Position endPosition)
{
    Path *path = initialisePath(map);

    traceVisited(path, breadthFirstSearch(map, startPosition, endPosition),
                 endPosition);

    return path;
}

Position getNextPosition(Path *path)
{
    Position next = popFromPath(path);

    if (path->pathLength = 0)
    {
        deallocatePath(path);
    }

    return next;
}