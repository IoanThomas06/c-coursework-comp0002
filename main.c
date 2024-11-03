#include "controller.h"

int main()
{
    Controller *controller = initialiseController();
    run(controller);

    return 0;
}