#include "controller.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    Controller *controller = parseControllerArguments((size_t)argc, argv);

    run(controller);

    return 0;
}