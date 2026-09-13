#include "cmd.h"
#include <stdexcept>

int main(int argc, char** argv) {
    if (argc < 2) {
        ERROR("File not providen\n");
        return 1;
    }
    try {
        const std::string path = *(argv+1);
        Counter counter(path);
        CMD cmd(&counter);
        cmd.parse_cmd();
    }
    catch(std::invalid_argument & e) {
      ERROR("Error occured: %s", e.what());
      return 1;
    }
    return 0;
}
