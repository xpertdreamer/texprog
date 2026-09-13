#include "cmd.h"

void CMD::parse_cmd() {
    std::string input;
    while (true) {
        CMD::print_help();
        std::cout << prompt << " ";
        if (!std::getline(std::cin, input)) break;
        if (input.empty()) continue;
        auto it = handlers.find(input);
        if (it != handlers.end()) it->second();
        else { std::cout <<  "unknown" << std::endl; continue; }
    }
}
