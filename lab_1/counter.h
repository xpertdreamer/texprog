// TODO: documentation

#ifndef COUNTER_H
#define COUNTER_H

/**
 * @file counter.h
 * @brief Word counter class for counting word occurrences in a file.
 */

#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>
#include <unordered_map>

#include "util.h"
#include "conf.h"

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
  std::unordered_map<std::string, std::vector<size_t>> indecies; ///<Map of indecies of each word in file

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
    // thats just works
    buf_stream << input.rdbuf();
    input.close();
    DEBUG("File %s successfully closed, and input sent to the buffer\n",
          path.c_str());
    #if TEST
    std::cout << buf_stream.str();
    buf_stream.clear();
    buf_stream.seekg(0, std::ios::beg);
    #endif
  }

  /**
  * @brief Destructor.
  * Logs a debug message when the object is destroyed.
  */
  inline ~Counter() {
    DEBUG("Counter: destructor closed\n");
  }

  /**
  * @brief Prints all counted word pairs to standard output.
  * Each line contains a word and its occurrence count, separated by '-'.
  * @note This method does not modify the object state.
  * @note Use this on debug
  */
  inline void print_counted() const {
    for (auto pair : pairs) {
        std::cout << pair.first << " - " << pair.second << std::endl;
    }
  }

  inline void print_indicies() const {
    if (tokens.empty()) {
      DEBUG("Vector of tokens is empty");
      return;
    }
    for (auto tok : indecies) {
        std::cout << tok.first << " - ";
        for (size_t i = 0; i < tok.second.size(); ++i) {
            std::cout << tok.second[i] << (i < tok.second.size() - 1 ? ", " : "\n");
        }
    }
  }

  /**
  * @brief Prints all tokens collected by tokenize
  * @note This method does not modify the object state.
  * @note Use this on debug
  */
  inline void print_tokens() const {
    if (tokens.empty()) {
      DEBUG("Vector of tokens is empty");
      return;
    }
    for (auto tok : tokens) {
        std::cout << tok << std::endl;
    }
  }

  /**
  * @brief Tokenizes the buffered file contents.
  *
  * @details Splits the contents of @c buf_stream into tokens and stores them in the @c tokens vector for further processing.
  */
  void tokenize();

  /**
   * @brief Counts the occurrences of each token.
   *
   * @details Fill the @c pairs map with each unique token and the number of times it appears in the file.
   */
  void count();

  /**
   * @brief Builds an index of token positions.
   *
   * @details Fill the @c indecies map with each unique token and a vector of positions where it appears in the file.
   */
  void index();
};

#endif
