#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "token.h"
#include <vector>

class Parser {
private:
  std::vector<Token> tokens;
  int pos;

  Node *parse_block();

  Node *parse_if();

  Node *parse_while();

  Node *parse_for();

  Node *parse_variable();

  Node *parse_function();

  Node *parse_expression();

  Node *parse_statement();

  std::vector<Token> to_postfix(std::vector<Token> infix);

  Token peek();

  Token advance();

  void consume(TokenType expected, std::string message);

  bool at_end();

public:
  Parser(std::vector<Token> tokens);
  ~Parser();

  Node *parse();
};

#endif // PARSER_H
