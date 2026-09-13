/**
 * @file cmd.h
 * @brief Command-line interface handler.
 */

#ifndef CMD_H
#define CMD_H

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <sstream>

#include "counter.h"
#include "stl.h"

/**
 * @struct Command
 * @brief Represents a single command with a name and description.
 */
typedef struct {
    std::string name;
    std::string description;
} Command;

/**
 * @brief Prompts the user to enter a container of integers.
 * @return Vector of integers entered by the user.
 */
std::vector<int> input_container();

/**
 * @brief Prompts the user to enter a range (two integers).
 * @return Pair representing the range.
 */
std::pair<int, int> input_range();

/**
 * @brief Prints the contents of an integer container.
 * @param container Vector of integers to print.
 */
void print_container(const std::vector<int>& container);

/**
 * @brief Command handler: squares all prime numbers in the input container.
 */
inline void do_prime() {
    std::vector<int> container = input_container();
    DEBUG("Call primes\n");
    primes(container);
    print_container(container);
}

/**
 * @brief Command handler: sorts the input container.
 */
inline void do_sort() {
    std::vector<int> container = input_container();
    DEBUG("Call sort\n");
    sort(container);
    print_container(container);
}

/**
 * @brief Command handler: finds integers in the input container within a range.
 */
inline void do_find() {
    std::vector<int> container = input_container();
    std::pair<int, int> range = input_range();
    DEBUG("Call find\n");
    container = find(container, range);
    print_container(container);
}

/**
 * @class CMD
 * @brief Interactive command dispatcher.
 */
class CMD {
private:
    bool quiet = false; ///< Suppress output when true
    char prompt = '$';
    std::vector<Command> commands; ///< List of available commands (fill inside constructor)
    std::unordered_map<std::string, std::function<void()>> handlers; ///< Map: command name : function handler
    Counter* counter_ref; ///< Counter instance

     /**
     * @brief Prints the help message listing all available commands.
     */
    inline void print_help() const {
        std::cout << std::endl;
        for (const auto& cmd : commands) {
            std::cout << cmd.name << " - " << cmd.description << std::endl;
        }
        std::cout << std::endl;
    }

    /**
     * @brief Command handler: Toggles quiet mode.
     */
    inline void do_q() {
        std::cout << "Quite=" << quiet << std::endl;
        quiet = !quiet;
    }

    /**
     * @brief Command handler: Counts word occurrences and prints results unless quiet mode is on.
     */
    inline void do_count() {
        counter_ref->count();
        if (quiet == false) counter_ref->print_counted();
    }

    /**
     * @brief Command handler: Indexes word occurrences and prints results unless quiet mode is on.
     */
    inline void do_index() {
        counter_ref->index();
        if (quiet == false) counter_ref->print_indicies();
    }

public:
    /**
     * @brief Constructs a CMD instance for file-based word counting.
     * @param counter Pointer to an initialized Counter object.
     */
    CMD(Counter* counter) {
        commands = {
           {"q", "Quiet mode"},
           {"count", "Count every word occurence in the given file"},
           {"index", "Index every word occurence in the give file"},
        };
        counter_ref = counter;
        counter_ref->tokenize();
        handlers["q"]     = [this]() { do_q(); };
        handlers["count"] = [this]() { do_count(); };
        handlers["index"] = [this]() { do_index(); };
    };

    /**
     * @brief Constructs a CMD instance for vector manipulation commands.
     */
    CMD() {
        commands = {
            {"primes", "Squaring prime numbers"},
            {"sort", "Perform sorting"},
            {"find", "Find integers in given range"},
        };
        handlers["primes"] = []() { do_prime(); };
        handlers["sort"]   = []() { do_sort(); };
        handlers["find"]   = []() { do_find(); };
    }

    ~CMD() = default;

    /**
     * @brief Parses and executes a single command from standard input.
     */
    void parse_cmd();
};

#endif
