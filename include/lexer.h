#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <string>
#include <vector>

class Lexer {
private:
  std::string code;

  size_t pos;

  char current_char;

  void advance();

  char peek();

  void consume_whitespace();

  Token consume_number();

  Token consume_string();

public:
  Lexer(std::string code);

  std::vector<Token> tokenize();
};

#endif // LEXER_H
