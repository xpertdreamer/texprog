#ifndef UPLOADER_H
#define UPLOADER_H

#include "util.h"

#include <fstream>
#include <string>
#include <vector>

/**
 * @class Uploader
 * @brief Provides utilities for uploading document content from files or standard input.
 */
class Uploader {
    private:
        std::ifstream file;
        std::size_t batch_size;
        std::vector<char> buffer;
    public:
        Uploader(const std::string& path, std::size_t size)  {
            DEBUG("Call constructor");
            batch_size = size;
            buffer.resize(batch_size);
            file.open(path, std::ios::binary);
            DEBUG("Opened path=%s", path.c_str());
            file.seekg(0, std::ios::beg);
        }

        const std::vector<char>&
        get_buf() {
            return buffer;
        }

        std::size_t
        get_size() { return batch_size; }

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

        std::streamsize
        upload_batch();

        bool
        has_more_batches();

        bool
        stream_bad();
};

#endif
