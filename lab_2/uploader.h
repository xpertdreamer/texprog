#include <string>

class Uploader {
    public:
        std::string
        upload_file(const std::string& path);

        std::string
        upload_stdin();
};
