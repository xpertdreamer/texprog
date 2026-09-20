#include "util.h"
#include "uploader.h"
#include "parser.h"
#include "validator.h"

#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <iomanip>

#define FLAGS "p:hf:v"

// TODO: struct options to avoid massive bool groups

#define HELP(name) do { \
        fprintf(stderr, "Usage %s [-p <path>]\n", name); \
        fprintf(stderr, "\t-p\tpath to upload input file\n"); \
        fprintf(stderr, "\t-h\tprint this message\n"); \
        fprintf(stderr, "\t-f\tfind structure element in text\n"); \
        fprintf(stderr, "\t-v\tinput as string and validate\n"); \
        return EXIT_SUCCESS; } while(0)

int
main(int argc, char** argv)
{
    if (!std::setlocale(LC_ALL, "ru_RU.UTF-8")) {
        ERROR("setlocale failed — wide output will be empty");
        return EXIT_FAILURE;
    }
    // p - path to parsed file ;
    // h - print help ;
    // f - find structure element;
    // v - input as string and validate
    int opt;
    std::string path, elem;
    std::wstring input;
    Type type;
    bool p_providen = false, f_providen = false, v_provided = false;
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
            case 'f': {
                elem = optarg;
                if (elem.empty()) {
                    ERROR("Element is not provided");
                    return EXIT_FAILURE;
                }
                if (elem == "h") type = Type::Header;
                else if (elem == "l") type = Type::List;
                else type = Type::Paragraph;
                f_providen = true;
                break;
            }
            case 'v': v_provided = true; break;
            case 'h': [[fallthrough]];
            default: HELP(argv[0]); break;
        }
    }

    try {
        if (v_provided) {
            std::wstring text = Uploader::upload_stdin();
            std::wstring res = Validator::validate(text);
            std::wcout << L"Result: " << res << std::endl;
            return EXIT_SUCCESS;
        }
        if (p_providen && f_providen) {
            std::string text = Uploader::upload_file(path);
            if (!Parser::validate(text, Format::AsciiDoc)) return EXIT_FAILURE;
            std::vector<Element> res = Parser::find(text, type);
            std::cout << "Found:" << std::endl;
            for (const auto &it : res) {
                std::cout << std::left << std::setw(100) << it.text << std::left <<std::setw(10) << "\t" << "line=" << it.line;
                if (it.type == Type::Header) {
                    std::cout << std::right << std::setw(10) << "\tlevel=" << static_cast<int>(it.level);
                }
                std::cout << std::endl;
            }
        }
        else {
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
