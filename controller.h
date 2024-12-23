#include "robot.h"
#include "map.h"
#include "marker.h"
#include "display.h"
#include <stddef.h>

#ifndef CONTROLLER_H
#define CONTROLLER_H

typedef struct Controller Controller;

Controller *initialiseController(size_t, size_t, size_t, size_t,
                                 void (*mapGenerationFunction)(Map *));
Controller *parseControllerArguments(size_t, char **);
void deallocateController(Controller *);
void run(Controller *);

#endif