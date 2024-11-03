#include "controller.h"
#include "allocations.h"
#include "robot.h"
#include "map.h"
#include "marker.h"
#include "display.h"
#include <stdlib.h>

Controller *initialiseController()
{
    Controller *controller = (Controller *)checkedMalloc(sizeof(Controller),
                                                         "Controller");

    //
}

void deallocateController(Controller *controller);
void run(Controller *);