#include "robot.h"
#include "map.h"
#include "marker.h"
#include "display.h"

#ifndef CONTROLLER_H
#define CONTROLLER_H

typedef struct Controller
{
    Robot *robot;
    Map *map;
    Marker **markers;
    Display *display;
} Controller;

Controller *initialiseController();
void deallocateController(Controller *controller);
void run(Controller *);

#endif