#include "counter.h"
#include <string>

void Counter::tokenize() {
  START(tokenize);
  DEBUG("Call tokenize\n");
  const std::string delimiters = "\"\',.;!?: \t\n\r";
  std::string text = buf_stream.str();
  std::string token;
  bool is_del[256] = {false};
  for (unsigned char d : delimiters) is_del[d] = true;
  // i dont know if its valid to read char by char but its my maximum for now
  char c;
  while (buf_stream.get(c)) {
      if (is_del[c]) {
    //if (delimiters.find(c) != std::string::npos) {
      if (!token.empty()) {
        tokens.emplace_back(token);
        token.clear();
      }
    } else token += c;
  }
  if (!token.empty()) {
    tokens.emplace_back(token);
  }
  END(tokenize);
}
