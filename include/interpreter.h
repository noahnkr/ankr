#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"
#include "parser.h"
#include "lexer.h"
#include <vector>

class Interpreter {
private:
  BlockNode *ast;

  bool debug_mode;

  std::vector<std::vector<Node *>> scope;
  size_t scope_index;

  void print_scope();

  Value *get_variable_value(std::string identifier);

  void set_variable_value(std::string identifier, Value *new_value);

  void define_variable(VariableNode* vn);

  Value* evaluate_function(std::string identifier, std::vector<Value *> parameters);

  Value *evaluate(Node *node);

  void visit(Node *node);

public:
  Interpreter(std::string code, bool debug_mode);
  ~Interpreter();

  void execute();
};

#endif // INTERPRETER_H
