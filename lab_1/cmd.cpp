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

// TODO: trim
std::vector<int> input_container() {
    std::vector<int> res;
    std::cout << "Enter numbers (separated by space): ";
    std::string raw;
    if (std::getline(std::cin, raw)) {
        std::stringstream ss(raw);
        int number;
        while (ss >> number) res.push_back(number);
    }
    return res;
}

void print_container(const std::vector<int>& container) {
    std::cout << std::endl;
    for (const auto& element : container) std::cout << element << " ";
    std::cout << std::endl;
}
