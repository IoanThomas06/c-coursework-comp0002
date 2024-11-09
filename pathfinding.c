#include "pathfinding.h"
#include "position.h"
#include "direction.h"
#include "map.h"
#include "allocations.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

// Static type definitions.

struct Path
{
    size_t currentPoint;
    size_t pathLength;
    Position *pathStack;
};

typedef struct Visited
{
    Position *stack;
    size_t count;
} Visited;

typedef struct Queue
{
    int rear;
    int front;
    Position *queue;
} Queue;

// End of static type definitions.

// Static declarations.

static Path *initialisePath(Map *);
static void deallocatePath(Path *);
static Visited *initialiseVisited(Map *);
static void deallocateVisited(Visited *);
static Queue *initialiseQueue(Map *);
static void deallocateQueue(Queue *);

static void pushToPath(Path *, Position);
static Position popFromPath(Path *);

static bool linearSearch(Position, Position *, size_t, size_t);
static bool checkIsUnvisited(Visited *, Queue *, Position);
static Visited *breadthFirstSearch(Map *, Position, Position);
static void traceVisited(Path *, Visited *, Position, Position);

// End of static declarations.

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

static Queue *initialiseQueue(Map *map)
{
    Queue *queue = (Queue *)checkedMalloc(sizeof(Queue), "Visited");

    queue->queue = (Position *)checkedCalloc(countMapEmptySpace(map),
                                             sizeof(Position),
                                             "'queue' in Queue");

    queue->rear = -1;
    queue->front = -1;

    return queue;
}

static void deallocateQueue(Queue *queue)
{
    free(queue->queue);
    free(queue);
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

// Breadth-first search and related utilities.

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

static bool checkIsValidMove(Map *map, Position position)
{
    return isMapPositionValid(map, position) &&
           isMapPositionEmpty(map, position);
}

static bool checkIsUnvisited(Visited *visited, Queue *queue, Position next)
{
    return !linearSearch(next, visited->stack, 0, visited->count) &&
           !linearSearch(next, queue->queue, queue->front,
                         queue->rear - queue->front);
}

static Visited *breadthFirstSearch(Map *map, Position startPosition,
                                   Position target)
{
    Queue *queue = initialiseQueue(map);
    Visited *visited = initialiseVisited(map);

    visited->stack[visited->count++] =
        queue->queue[++queue->rear] = startPosition;

    Position current;
    bool hasVisitedTarget = false;
    while (queue->front != queue->rear && !hasVisitedTarget)
    {
        current = queue->queue[++queue->front];

        for (int i = 0; i < 4; i++)
        {
            Position adjacent =
                addDirectionToPosition(current, generateDirectionVector(i));

            if (checkIsValidMove(map, adjacent) &&
                checkIsUnvisited(visited, queue, adjacent) &&
                !hasVisitedTarget)
            {
                visited->stack[visited->count++] =
                    queue->queue[++queue->rear] = adjacent;
                hasVisitedTarget = comparePositionValues(adjacent, target);
            }
        }
    }

    deallocateQueue(queue);
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

// End of breadth-first search and related utilities.

// Path creator.

Path *findPath(Map *map, Position startPosition, Position endPosition)
{
    Path *path = initialisePath(map);

    traceVisited(path, breadthFirstSearch(map, startPosition, endPosition),
                 startPosition, endPosition);

    return path;
}

// End of Path creator.