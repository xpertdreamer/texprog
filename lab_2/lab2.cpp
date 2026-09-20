#include "util.h"
#include "uploader.h"
#include "parser.h"

#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <stdexcept>
#include <string>
#include <unistd.h>

#define FLAGS "p:h"

// TODO: struct options to avoid massive bool groups

#define HELP(name) do { \
        fprintf(stderr, "Usage %s [-p <path>]\n", name); \
        fprintf(stderr, "\t-p\tpath to upload input file\n"); \
        fprintf(stderr, "\t-h\tprint this message\n"); \
        return EXIT_SUCCESS; } while(0)

int
main(int argc, char** argv)
{
    if (!setlocale(LC_ALL, "C.UTF-8")) {
        ERROR("setlocale failed — wide output will be empty");
        return EXIT_FAILURE;
    }
    // p - path to parsed file ;
    // h - print help
    int opt;
    std::string path;
    bool p_providen = false;
    while ((opt = getopt(argc, argv, FLAGS)) != -1) {
        switch (opt) {
            case 'p': {
                path = optarg;
                if (path.empty()) {
                    ERROR("Path is not providen");
                    return EXIT_FAILURE;
                }
                p_providen = true;
                break;
            }
            case 'h': [[fallthrough]];
            default: HELP(argv[0]); break;
        }
    }

    try {
        if (p_providen) {
            std::string text = Uploader::upload_file(path);
            if (!Parser::validate(text, Format::AsciiDoc)) return EXIT_FAILURE;
        } else {
            ERROR("Some required flag did not providen");
            HELP(argv[0]);
            return EXIT_FAILURE;
        }
    } catch (std::invalid_argument& e) {
        ERROR("Error occured: %s", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
