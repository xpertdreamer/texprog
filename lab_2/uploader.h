#ifndef UPLOADER_H
#define UPLOADER_H

#include <string>

/**
 * @class Uploader
 * @brief Provides utilities for uploading document content from files or standard input.
 */
class Uploader {
    public:
        /**
         * @brief Uploads content from a file at the given path.
         * @param path Path to the file to be uploaded
         * @return Content of the file as a narrow string
         */
        static std::string
        upload_file(const std::string& path);

        /**
         * @brief Uploads content from standard input.
         * @return Content read from stdin as a wide string
         */
        static std::wstring
        upload_stdin();
};

#endif
