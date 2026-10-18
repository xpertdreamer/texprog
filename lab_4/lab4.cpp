// Menu in main thread
// Other functions live in other threads

#include "ui.h"

int
main(void)
{
    ui_handler uh('>');
    process(uh);
    return 0;
}
