#ifndef UPLOADER_H
#define UPLOADER_H

#include <string>

class Uploader {
    public:
        static std::string
        upload_file(const std::string& path);

        static std::wstring
        upload_stdin();
};

#endif
