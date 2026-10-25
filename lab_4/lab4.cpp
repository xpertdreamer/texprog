// Menu in main thread
// Other functions live in other threads

#include "ui.h"

#include <csignal>
#include <cstdlib>

int
main(int argc, char** argv)
{
    signal(SIGINT, &handler);
    if (argc < 2) {
        ERROR("path is not providen");
        return EXIT_FAILURE;
    }
    ui_handler uh('>', argv[1]);
    process(uh);
    return 0;
}
