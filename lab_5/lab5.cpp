#include "ui.h"
#include "util.h"

#inсlude <cstdlib>

int
main(int argc, char** argv)
{
    signal(SIGINT, &handler);
    if (argc < 2) {
        ERROR("path is not providen\targc={}", argc);
        return EXIT_FAILURE;
    }
    try {
        ui_handler uh('>', argv[1]);
        process(uh);
    } catch (...) {
        ERROR("...");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
