#ifndef UI_H
#define UI_H

#include "watcher.h"
#include "tdsqueue.h"

#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <memory>
#include <string>
#include <iostream>
#include <functional>
#include <algorithm>
#include <thread>

struct query {
    std::string name;
    std::string description;
    std::function<void()> handler = nullptr;
};

struct ui_handler {
    std::array<query, 3> qs;
    char prompt;
    bool yes = true;

    std::unique_ptr<TdSQueue<std::filesystem::directory_entry>> queue;
    std::unique_ptr<Watcher> w;
    std::thread watch_thread;

    ui_handler(char p) {
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
            }},

            {"START", "start to execute watcher", [this]() {
                if (watch_thread.joinable()) return;
                queue = std::make_unique<TdSQueue<std::filesystem::directory_entry>>();
                w = std::make_unique<Watcher>(std::filesystem::current_path());
                watch_thread = std::thread(&Watcher::watch, w.get(), queue.get());
            }},
        }};
    }

    ~ui_handler() {
        if (w) {
            if (w->watching()) {
                w->stop();
            }
        }
        if (watch_thread.joinable()) watch_thread.join();
        if (queue) {
            std::cout << queue->size() << std::endl;
        }
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
    while (handler.yes) {
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
