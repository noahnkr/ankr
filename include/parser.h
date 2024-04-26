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

  UnaryNode *parse_return();

  Node *parse_expression();

  Node *parse_statement();

  std::vector<Token> to_postfix(std::vector<Token> infix);

  Token peek();

  Token advance();

  void consume(TokenType expected, std::string message);

  static void draw_tree_rec(Node *root, std::string tree, std::string padding,
                            std::string pointer, bool has_right);

  bool at_end();

public:
  Parser(std::vector<Token> tokens);
  ~Parser();

  Node *parse();

  static void traverse(Node *root);

  static std::string draw_tree(Node *root);

};

#endif // PARSER_H
