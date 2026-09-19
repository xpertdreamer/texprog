#include <string>

class Uploader {
    public:
        static std::string
        upload_file(const std::string& path);

        static std::string
        upload_stdin();
};
