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

std::pair<int, int> input_range() {
    std::pair<int, int> res = {0, 0};
    std::string raw;
    while(true) {
        std::cout << "Enter range (format: x1 x2) ";
        int first, second;
        if (std::getline(std::cin, raw)) {
            std::stringstream ss(raw);
            if (ss >> first >> second) {
                DEBUG("Range: %d %d\n", first, second);
                res.first = first;
                res.second = second;
                break;
            }
            ERROR("Invalid input");
        }
    }
    return res;
}

void print_container(const std::vector<int>& container) {
    std::cout << std::endl;
    if (container.size() == 0) std::cout << "Empty container" << std::endl;
    for (const auto& element : container) std::cout << element << " ";
    std::cout << std::endl;
}
