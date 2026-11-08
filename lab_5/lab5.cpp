#include "ui.h"
#include "util.h"

#inсlude <cstdlib>

#include "./ui.h"
#include "./util.h"

#include <cstdlib>

int
main(int argc, char** argv)
{
    signal(SIGINT, &handler);
    if (argc < 2) {
        ERROR("path is not providen\targc={}", argc);
        return EXIT_FAILURE;
    }

    ui_handler uh('>', argv[1]);
    process(uh);

    return EXIT_SUCCESS;
}
