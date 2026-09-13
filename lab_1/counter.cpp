#include "counter.h"
#include <string>

static void lower(std::string& token) {
    if (token.empty()) return;
    unsigned char c1 = static_cast<unsigned char>(token[0]);
    // check ASCII code of first char
    if (c1 >= 'A' && c1 <= 'Z') {
        token[0] = static_cast<char>(c1 + 32);
        return;
    }
    // ru letter has 2 bytes
    if (token.size() >= 2) {
        unsigned char c2 = static_cast<unsigned char>(token[1]);
        // Ё = D0 81; ё = D1 91
        if (c1 == 0xD0 && c2 == 0x81) {
            token[0] = static_cast<char>(0xD1);
            token[1] = static_cast<char>(0x91);
            return;
        }
        // A-Я = D0 90 - D0 AF
        if (c1 == 0xD0 && c2 >= 0x90 && c2 <= 0xAF) {
            if (c2 <= 0x9F) {
                token[1] = static_cast<char>(c2 + 0x20);
            } else {
                token[0] = static_cast<char>(0xD1);
                token[1] = static_cast<char>(c2 - 0x20);
            }
        }
    }
}

void Counter::tokenize() {
  START(tokenize);
  DEBUG("Call tokenize\n");
  const std::string delimiters = "][)(\"\',.;!?:- \t\n\r";
  std::string token;
  bool is_del[256] = {false};
  for (unsigned char d : delimiters) is_del[d] = true;
  // i dont know if its valid to read char by char but its my maximum for now
  char c;
  while (buf_stream.get(c)) {
      if (is_del[(unsigned char)c]) {
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
