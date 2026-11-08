#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <iostream>
#include <mutex>
#include <format>

class Logger {
    private:
        std::mutex mutex;
        std::ofstream file;

        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

    public:
        Logger() = default;

        ~Logger() {
            if (file.is_open()) {
                file.close();
            }
        }

        inline void
        open(const std::string& path) {
            std::lock_guard l(mutex);
            if (!file.is_open()) file.open(path, std::ios::app);
        }

        inline void
        fmt(const std::string& msg) {
            std::lock_guard l(mutex);
            auto now = std::chrono::system_clock::now();
            if (file.is_open()) {
                file << std::format("{:%Y-%m-%d %H:%M:%S}", now) << "\tEXCEPT\t" << msg << '\n';
                file.flush();
            }
        }

        static Logger& instance() {
            static Logger l;
            return l;
        }
};

void
LOG(const std::string& msg) {
    Logger::instance().fmt(msg);
}

#endif
