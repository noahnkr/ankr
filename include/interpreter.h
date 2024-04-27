#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"
#include <vector>

class Interpreter {
private:
  BlockNode *ast;
  std::vector<std::vector<Node *>> scope;
  size_t scope_index;

  void define_variable(VariableNode* vn);

  void define_function(FunctionNode* fn);

  Value *evaluate(Node *node);

  void visit(Node *node);

public:
  Interpreter(BlockNode *ast);
  ~Interpreter();

  void execute();
};

#endif // INTERPRETER_H
