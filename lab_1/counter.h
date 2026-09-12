/**
 * @file counter.h
 * @brief Word counter class for counting word occurrences in a file.
 */

#include <fstream>
#include <iosfwd>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>
#include <unordered_map>

#include "util.h"

#define TEST 1 // 0/1

/**
 * @class Counter
 * @brief Counts word occurrences in a given file
 * This class reads a file into an internal buffer, tokenizes its content,
 * and stores the frequency of each token in an unordered map.
 */
class Counter {
private:
  std::ifstream input; ///<Input file stream
  std::stringstream buf_stream; ///<Bufer holding content of the given file
  std::vector<std::string> tokens; ///<Vector of tokens
  std::unordered_map<std::string, std::size_t> pairs; ///<Map of word occurences

public:
  /**
   * @brief Default constructor.
   * Logs an error indicating that no input file was provided.
   */
  inline Counter() { ERROR("No input file providen!\n"); }

  /**
   * @brief Constructs a Counter and loads the file at path into the buffer.
   * Opens the file, reads its entire contents into an internal buffer stream,
   * and closes the file. If the file cannot be opened, an exception is thrown.
   * @param path Path to the input file.
   * @throws std::invalid_argument If the file cannot be opened.
   * @note The file contents are stored in an internal std::stringstream.
   */
  inline Counter(const std::string& path) {
    DEBUG("PATH: %s\n", path.c_str());
    input.open(path);
    if (!input) {
      ERROR("Cannot open input file!\n");
      throw std::invalid_argument("Counter constructor, Given input is invalid!\n");
    }
    DEBUG("File %s successfuly opened\n", path.c_str());
    // jump to the end of file
    input.seekg(0, std::ios::end);
    std::streampos len = input.tellg();
    // jump to the end of file
    input.seekg(0, std::ios::beg);
    std::vector<char> buffer(len);
    // read file to temporary buffer (faster than just .rdbuf() method according to StackOverflow)
    input.read(&buffer[0], len);
    // rdbuf returns pointer to file buffer
    buf_stream.rdbuf()->pubsetbuf(&buffer[0], len);
    input.close();
    DEBUG("File %s successfully closed, and input sent to the buffer\n",
          path.c_str());
    #if TEST
    std::cout << buf_stream.str();
    #endif
  }

  /**
  * @brief Destructor.
  * Logs a debug message when the object is destroyed.
  */
  inline ~Counter() {
    DEBUG("Counter: destructor closed\n");
  }

  // TODO: getters and setters

  /**
  * @brief Prints all counted word pairs to standard output.
  * Each line contains a word and its occurrence count, separated by '-'.
  * @note This method does not modify the object state.
  */
  inline void print_counted() const {
    for (auto pair : pairs) {
        std::cout << pair.first << " - " << pair.second << std::endl;
    }
  }

  // TODO: tokenization method
  bool tokenize();
};
