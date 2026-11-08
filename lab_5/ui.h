#ifndef UI_H
#define UI_H

#include "task.h"
#include "tdsqueue.h"
#include "util.h"
#include "watcher.h"
#include "parser.h"

#include <cctype>
#include <csignal>
#include <cstdlib>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <iostream>
#include <functional>
#include <algorithm>
#include <thread>
#include <mutex>

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
    std::array<query, 11> qs;
    char prompt;
    bool yes = true;
    std::string path;


    Watcher watcher;

    ui_handler(char p, char* pt) {
        prompt = p;
        path.assign(pt);

        qs = {{
            {"HELP", "show help message", [this]() {
                std::cout << "Help:\n";
                for (size_t i = 0; i < qs.size(); ++i) {
                    std::cout << qs[i].name << " - " << qs[i].description << std::endl;
                }
            }},
            {"EXIT", "exit", [this]() {yes = false;}},
            {"START", "start watching loop", [this]() {watcher.start(path, Mode::threads);}},
            {"DUMP", "dump current state", [this](){watcher.dump_state();}},
            {"DROP", "drop current queue", [this](){watcher.drop_queue();}},
            {"CHANGE", "change directory being watched", [this](){
                if (auto input = get_input("Enter relative path to new directory: ")) {
                    if (watcher.change_directory(*input)) path.assign(*input);
                }
            }},
            {"ASYNC", "start async execution", [this]{watcher.start(path, Mode::async);}},
            {"STOP", "stop all workers", [this](){watcher.stop();}},
            {"SHOW", "show result of file processing", [this](){
                if (auto input = get_input("Enter file path: ")) {
                    auto v = watcher.get_res(*input);
                    if (v.empty()) {
                        ERROR("no elements found or doesn't exists");
                        return;
                    }
                    std::cout << "Found:" << '\n';
                    for (const auto& e : v) {
                        std::cout << std::left << std::setw(100) << e.text
                                  << std::setw(10) << "\t" << "line=" << e.line;
                        if (e.type == Type::Header)
                            std::cout << std::right << std::setw(10)
                                      << "\tlevel=" << static_cast<int>(e.level);
                        std::cout << '\n';
                    }
                }
            }},
            {"CANCEL", "cancel async processing of a file", [this]() {
                if (auto input = get_input("Enter file path to cancel: ")) {
                    watcher.cancel(*input);
                }
            }},
            {"STATUS", "show status of a specific file", [this](){
                if (auto input = get_input("Enter file path: ")) {
                    auto s = watcher.get_stat(*input);
                    if (!s) {
                        ERROR("file not tracked");
                        return;
                    }
                    std::cout << *input << " : " << status_to_string(*s) << '\n';
                }
            }},
        }};
    }

    ~ui_handler() {watcher.stop();}

    inline void
    execute(size_t index) {
        if (qs[index].handler != nullptr) {
            qs[index].handler();
        }
    }

    inline std::optional<std::string>
    get_input(const std::string& msg)
    {
        std::cout << msg;
        std::string input;
        if (!(std::cin >> input)) {
            return std::nullopt;
        }
        return input;
    }
};

inline void
process(ui_handler& handler) {
    while (handler.yes && !stop) {
        std::cout << handler.prompt << ' ' << std::flush;
        std::string input;
        if (!(std::cin >> input)) {
            handler.yes = false;
            break;
        }
        std::transform(input.begin(), input.end(), input.begin(), ::toupper);
        bool found = false;
        for (auto &q : handler.qs) {
            if (q.name == input) {
                q.handler();
                found = true;
                break;
            }
        }
        if (!found) ERROR("not found");
    }
}

#endif
