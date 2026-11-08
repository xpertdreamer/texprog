#include "./ui.h"
#include "./util.h"
#include "logger.h"

#include <cstdlib>

constexpr std::string LOGFILE_NAME = "lab5.log";

int
main(int argc, char** argv)
{
    signal(SIGINT, &handler);
    if (argc < 2) {
        ERROR("path is not providen\targc={}", argc);
        return EXIT_FAILURE;
    }

    Logger::instance().open(LOGFILE_NAME);

    ui_handler uh('>', argv[1]);
    process(uh);

    return EXIT_SUCCESS;
}
