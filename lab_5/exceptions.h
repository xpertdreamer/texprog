#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

class SizeException : public std::runtime_error {
    public:
        SizeException() : std::runtime_error("File cannot be bigger than 24Mb") {}
};

class FileDeletedException : public std::runtime_error {
    public:
        FileDeletedException(const std::string& p) : std::runtime_error("File was deleted from watched direcotry: " + p) {}
};

class PermissionException : public std::runtime_error {
    public:
        PermissionException(const std::string& p) : std::runtime_error("Permission denied: " + p) {}
};

class WrongFormatException : public std::runtime_error {
    public:
        WrongFormatException(const std::string& p) : std::runtime_error("File doesn't matched format: " + p) {}
};

#endif
