#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <string>
#include <vector>

/**
 * The Lexer class is responsible for converting a string of source code into
 * a sequence of tokens. These tokens are then used by a parser to create an
 * abstract syntax tree (AST) of the input code.
 */
class Lexer {
private:
  std::string code; ///< The source code to tokenize.
  size_t pos;       ///< Current position in the source code string.
  char current_char;///< Current character at the position in the source code.

  /**
   * Advances the current position in the source code by one character.
   * Updates the current_char to the new character at the current position.
   */
  void advance();

  /**
   * Peeks the next character in the source code without advancing the position.
   * @return The next character or EOF if at the end of the source code.
   */
  char peek();

  /**
   * Consumes all whitespace characters from the current position until a non-whitespace
   * character is found.
   */
  void consume_whitespace();

  /**
   * Consumes characters representing a number from the current position and forms
   * a token representing an integer or float.
   * @return A Token of type INT or FLOAT based on the consumed characters.
   */
  Token consume_number();

  /**
   * Consumes characters inside quotes to form a string token.
   * Assumes that the current character is the opening quote.
   * @return A Token of type STRING containing the string value.
   */
  Token consume_string();

public:
  /**
   * Constructs a Lexer object initialized with source code.
   * @param code The source code as a string.
   */
  Lexer(std::string code);

  /**
   * Processes the entire source code to tokenize it.
   * @return A vector of tokens derived from the source code.
   */
  std::vector<Token> tokenize();
};

#endif // LEXER_H
