#include "pathfinding.h"
#include "position.h"
#include "direction.h"
#include "map.h"
#include "allocations.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include <stdio.h>

typedef struct Visited
{
    Position *stack;
    size_t count;
} Visited;

// Memory allocation functions.

static Path *initialisePath(Map *map)
{
    Path *path = (Path *)checkedMalloc(sizeof(Path), "Path");

    path->pathStack = (Position *)checkedMalloc(sizeof(Position) *
                                                    countMapEmptySpace(map),
                                                "'path' in Path");

    path->currentPoint = 0;
    path->pathLength = 0;

    return path;
}

static void deallocatePath(Path *path)
{
    free(path->pathStack);
    free(path);
}

static Visited *initialiseVisited(Map *map)
{
    Visited *visited = (Visited *)checkedMalloc(sizeof(Visited), "Visited");

    visited->stack = (Position *)checkedCalloc(countMapEmptySpace(map),
                                               sizeof(Position),
                                               "'list' in Visited");

    visited->count = 0;

    return visited;
}

static void deallocateVisited(Visited *visited)
{
    free(visited->stack);
    free(visited);
}

// End of memory allocation functions.

// Path utilities.

int getPathLength(Path *path)
{
    return path->pathLength;
}

Position peekPath(Path *path)
{
    return path->pathStack[path->pathLength - 1];
}

static void pushToPath(Path *path, Position position)
{
    path->pathStack[path->pathLength++] = position;
}

static Position popFromPath(Path *path)
{
    return path->pathStack[--path->pathLength];
}

Position getNextPosition(Path *path)
{
    Position next = popFromPath(path);

    // Checks if the length was 0, and subtracted 1, i.e. there are no more
    // positions in the path.
    if (path->pathLength == SIZE_MAX)
    {
        deallocatePath(path);
    }

    return next;
}

// End of Path utilities.

// BFS and related utilities.

static bool linearSearch(Position target, Position *list, size_t startIndex,
                         size_t count)
{
    for (size_t i = startIndex; i < startIndex + count; i++)
    {
        if (comparePositionValues(target, list[i]))
        {
            return true;
        }
    }
    return false;
}

static Visited *breadthFirstSearch(Map *map, Position startPosition,
                                   Position target)
{
    int qRear = -1;
    int qFront = -1;
    Position *queue = (Position *)checkedCalloc(countMapEmptySpace(map),
                                                sizeof(Position),
                                                "'queue' in findPath");

    Visited *visited = initialiseVisited(map);

    Position current;
    bool hasVisitedTarget = false;

    visited->stack[visited->count++] = queue[++qRear] = startPosition;

    while (qFront != qRear && !hasVisitedTarget)
    {
        current = queue[++qFront];

        for (int i = 0; i < 4; i++)
        {
            Position adjacent =
                addDirectionToPosition(current, generateDirectionVector(i));

            if (isMapPositionValid(map, adjacent) &&
                isMapPositionEmpty(map, adjacent) &&
                !linearSearch(adjacent, visited->stack, 0, visited->count) &&
                !linearSearch(adjacent, queue, qFront, qRear - qFront) &&
                !hasVisitedTarget)
            {
                visited->stack[visited->count++] = queue[++qRear] = adjacent;
                hasVisitedTarget = comparePositionValues(adjacent, target);
            }
        }
    }

    free(queue);
    return visited;
}

static void traceVisited(Path *path, Visited *visited, Position start,
                         Position target)
{
    Position current;

    pushToPath(path, current = target);

    bool hasReachedStart = false;
    while (visited->count-- > 1 && !hasReachedStart)
    {
        for (int i = 0; i < 4; i++)
        {
            DirectionVector nextDirection = generateDirectionVector(i);
            Position adjacent = addDirectionToPosition(current,
                                                       nextDirection);
            if (!hasReachedStart &&
                comparePositionValues(visited->stack[visited->count], adjacent))
            {
                hasReachedStart = comparePositionValues(adjacent, start);
                pushToPath(path, current = adjacent);
            }
        }
    }

    deallocateVisited(visited);
}

// End of BFS.

// Path creator.
Path *findPath(Map *map, Position startPosition, Position endPosition)
{
    Path *path = initialisePath(map);

    traceVisited(path, breadthFirstSearch(map, startPosition, endPosition),
                 startPosition, endPosition);

    return path;
}

// End of Path creator.