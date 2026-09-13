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
  bool is_del[256] = {false};
  for (unsigned char d : delimiters) is_del[d] = true;
  std::string token;
  char c;
  while (buf_stream.get(c)) {
      unsigned char uc = static_cast<unsigned char>(c);
      if (is_del[uc]) {
          if (!token.empty()) {
              lower(token);
              tokens.emplace_back(token);
              token.clear();
          }
      }
      // NOTE: russian quote-marks has two-bytes ASCII codes (D2 AB) and (C2 BB)
      else if (uc == 0xC2 && (buf_stream.peek() == 0xAB || buf_stream.peek() == 0xBB)) {
          buf_stream.get(c);
          if (!token.empty()) {
              lower(token);
              tokens.emplace_back(token);
              token.clear();
          }
      }
      // NOTE: '...' = (E2 80 A6)
      else if (uc == 0xE2) {
          char next1 = buf_stream.peek();
          if (static_cast<unsigned char>(next1) == 0x80) {
              buf_stream.get(c);
              if (static_cast<unsigned char>(buf_stream.peek()) == 0xA6) {
                  buf_stream.get(c);
                  if (!token.empty()) {
                      lower(token);
                      tokens.emplace_back(token);
                      token.clear();
                  }
                  continue;
              }
              buf_stream.putback(next1);
          }
          token += c;
      }
      else token += c;
  }
  if (!token.empty()) {
      lower(token);
      tokens.emplace_back(token);
  }
  END(tokenize);
}

void Counter::count() {
  START(count);
  DEBUG("Call count\n");
  if (tokens.empty()) {
      ERROR("No tokens to count providen");
      return;
  }
  for (const auto& token : tokens) {
      pairs[token]++;
  }
  END(count);
}

void Counter::index() {
    START(index);
    DEBUG("Call index\n");
    if (tokens.empty()) {
        ERROR("No tokens to index providen");
        return;
    }
    size_t i = 0;
    do {
        const std::string& token = tokens[i];
        indecies[token].emplace_back(i);
        i++;
    } while (i < tokens.size());
    END(index);
}
