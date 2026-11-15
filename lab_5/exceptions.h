#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <stdexcept>
#include <string>

class SizeException : public std::exception {
    private:
        std::string _msg;
    public:
        SizeException(const std::string& msg = "File cannot be bigger than 24Mb") : _msg(msg) {}

        const char*
        what() const noexcept override {
            return _msg.c_str();
        }
};

class FileDeletedException : public std::exception {
    private:
        std::string _msg;
    public:
        FileDeletedException(const std::string& p, const std::string& msg = "File was deleted from watched directory: ")
            : _msg(msg + p) {}

        const char*
        what() const noexcept override {
            return _msg.c_str();
        }
};

class PermissionException : public std::exception {
    private:
        std::string _msg;
    public:
        PermissionException(const std::string& p, const std::string& msg="Permission denied: ") : _msg(msg + p) {}

        const char*
        what() const noexcept override {
            return _msg.c_str();
        }
};

class WrongFormatException : public std::exception {
    private:
        std::string _msg;
    public:
        WrongFormatException(const std::string& p, const std::string& msg="File doesn't matched format: ") : _msg(msg + p) {}
};

#endif
