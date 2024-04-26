#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "token.h"
#include <vector>

class Parser {
private:
  std::vector<Token> tokens;
  int pos;

  BlockNode *parse_block();

  IfNode *parse_if();

  WhileNode *parse_while();

  ForNode *parse_for();

  VariableNode *parse_variable();

  FunctionNode *parse_function(bool definition);

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

  static void traverse(Node *root);
};

#endif // PARSER_H
