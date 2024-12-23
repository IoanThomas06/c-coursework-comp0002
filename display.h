#include "marker.h"
#include "robot.h"
#include "map.h"
#include <stddef.h>

#ifndef DISPLAY_H
#define DISPLAY_H

typedef struct Display Display;

Display *initialiseDisplay(size_t, size_t, size_t, size_t);
void deallocateDisplay(Display *display);
void drawBackground(Display *, Map *);
void updateForeground(Display *, Robot *, Marker *[], size_t, int);

#endif