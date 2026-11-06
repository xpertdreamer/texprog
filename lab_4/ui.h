#ifndef UI_H
#define UI_H

#include "tdsqueue.h"
#include "watcher.h"

#include <cctype>
#include <csignal>
#include <cstdlib>
#include <filesystem>
#include <memory>
#include <string>
#include <iostream>
#include <functional>
#include <algorithm>
#include <thread>

volatile inline sig_atomic_t stop = 0;

inline void
handler(int) {
    stop = 1;
}

struct query {
    std::string name;
    std::string description;
    std::function<void()> handler = nullptr;
};

struct ui_handler {
    std::array<query, 6> qs;
    char prompt;
    bool yes = true;

    Watcher watcher;

    ui_handler(char p, char* path) {
        prompt = p;

        qs = {{
            {"HELP", "show help message", [this]() {
                std::cout << "Help:\n";
                for (size_t i = 0; i < qs.size(); ++i) {
                    std::cout << qs[i].name << " - " << qs[i].description << std::endl;
                }
            }},
            { "EXIT", "exit", [this]() {
                yes = false;
                watcher.stop();
                // TODO: stop watching
            }},
            {"START", "start watching loop", [this, path]() {
                watcher.start(path);
                // TODO: start loop
            }},
            {"DUMP", "dump queue state", [this](){
                std::cout << "Cache:" << '\n';
                auto cache = watcher.get_cache();
                for (const auto& [k, v] : cache) {
                    std::cout << k << " : " <<  status_to_string(v);
                }
                std::cout << '\n' << "Queue:" << '\n';
                watcher.get_queue().peek();
            }},
            {"DROP", "drop current queue", [this](){ watcher.get_queue().drop(); }},
            {"CHANGE", "change directory being watched", [this](){
                std::cout << "Enter relative path to new directory";
                std::string input;
                std::cin >> input;
                watcher.change_directory(input);
            }},
        }};
    }

    ~ui_handler() {
        // TODO
    }

    inline void
    execute(size_t index) {
        if (qs[index].handler != nullptr) {
            qs[index].handler();
        }
    }
};

inline void
process(ui_handler& handler) {
    while (handler.yes && !stop) {
        std::cout << handler.prompt << ' ';
        std::string input;
        std::cin >> input;
        std::transform(input.begin(), input.end(), input.begin(), ::toupper);
        for (auto &q : handler.qs) {
            if (q.name == input) {
                q.handler();
                break;
            }
        }
    }

}

#endif
