#include "cmd.h"
#include <stdexcept>

int main(int argc, char** argv) {
    try {
        switch (argc) {
            case 2: {
                const std::string path = *(argv+1);
                Counter counter(path);
                CMD cmd(&counter);
                cmd.parse_cmd();
                break;
            }
            case 1: {
                CMD cmd;
                cmd.parse_cmd();
                break;
            }
        }
    }
    catch(std::invalid_argument & e) {
      ERROR("Error occured: %s", e.what());
      return 1;
    }
    return 0;
}
