#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include "counter.h"

typedef struct {
    std::string name;
    std::string description;
} Command;

class CMD {
private:
    bool quiet = false;
    char prompt = '$';
    std::vector<Command> commands {
    {"q", "Quiet mode", },
    {"count", "Count every word occurence in the given file"},
    {"index", "Index every word occurence in the give file"}
    };
    std::unordered_map<std::string, std::function<void()>> handlers;
    Counter* counter_ref;

    inline void print_help() const {
        std::cout << std::endl;
        for (const auto& cmd : commands) {
            std::cout << cmd.name << " - " << cmd.description << std::endl;
        }
        std::cout << std::endl;
    }

    inline void do_q() {
        std::cout << "Quite=" << quiet << std::endl;
        quiet = !quiet;
    }
    inline void do_count() {
        counter_ref->count();
        if (quiet == false) counter_ref->print_counted();
    }
    inline void do_index() {
        counter_ref->index();
        if (quiet == false) counter_ref->print_indicies();
    }

public:
    CMD(Counter* counter) {
        counter_ref = counter;
        counter_ref->tokenize();
        handlers["q"]     = [this]() { do_q(); };
        handlers["count"] = [this]() { do_count(); };
        handlers["index"] = [this]() { do_index(); };
    };
    ~CMD() = default;
    void parse_cmd();
};
