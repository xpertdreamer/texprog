#ifndef UI_H
#define UI_H

#include <cctype>
#include <cstdlib>
#include <string>
#include <iostream>
#include <functional>
#include <algorithm>

struct query {
    std::string name;
    std::string description;
    std::function<void()> handler = nullptr;
};

struct ui_handler {
    std::array<query, 2> qs;
    char prompt;

    ui_handler(char p) {
        prompt = p;
        qs = {{
            {"HELP", "show help message", [this]() {
                std::cout << "Help:\n";
                for (size_t i = 0; i < qs.size(); ++i) {
                    std::cout << qs[i].name << " - " << qs[i].description << std::endl;
                }
            }},
            { "EXIT", "exit)", [this]() {
                std::exit(0);
            }}
        }};
    }

    inline void
    execute(size_t index) {
        if (qs[index].handler != nullptr) {
            qs[index].handler();
        }
    }
};

inline void
process(ui_handler handler) {
    while (true) {
        std::cout << handler.prompt << ' ';
        std::string input;
        std::cin >> input;
        std::transform(input.begin(), input.end(), input.begin(), ::toupper);
        for (auto &q : handler.qs) {
            if (q.name == input) {
                q.handler();
            }
        }
    }
}

#endif
