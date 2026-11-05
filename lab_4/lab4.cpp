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
        ERROR("path is not providen\targc={}", argc);
        return EXIT_FAILURE;
    }
    // try {
        ui_handler uh('>', argv[1]);
        process(uh);
    // } catch(const std::runtime_error& e) {
    //     ERROR("{}", e.what());
    //     return EXIT_FAILURE;
    // }
    return EXIT_SUCCESS;
}
