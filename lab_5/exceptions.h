#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

class SizeException : public std::runtime_error {
    public:
        SizeException() : std::runtime_error("File cannot be bigger than 24Mb") {}
};

#endif
