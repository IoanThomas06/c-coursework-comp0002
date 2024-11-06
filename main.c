#include "controller.h"
#include <stddef.h>

int main(int argc, char **argv)
{
    Controller *controller = parseControllerArguments((size_t)argc, argv);

    run(controller);

    deallocateController(controller);

    return 0;
}