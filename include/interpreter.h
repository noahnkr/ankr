#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"
#include <vector>

class Interpreter {
private:
  BlockNode *ast;

  std::vector<std::vector<Node *>> scope;
  size_t scope_index;

  Value *get_variable_value(std::string identifier);

  void set_variable_value(std::string identifier, Value *new_value);

  void define_variable(VariableNode* vn);

  Value* evaluate_function(std::string identifier, std::vector<Value *> parameters);

  Value *evaluate(Node *node);

  void visit(Node *node);

public:
  Interpreter(BlockNode *ast);
  ~Interpreter();

  void execute();
};

#endif // INTERPRETER_H
