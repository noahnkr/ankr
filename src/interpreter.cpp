#include "../include/interpreter.h"
#include <stdexcept>

Interpreter::Interpreter(BlockNode* ast) : ast(ast), scope(), scope_index() {};

Interpreter::~Interpreter() { delete ast; };

void Interpreter::define_function(FunctionNode* fn) {

}

Value* Interpreter::evaluate(Node *node) {
  if (auto *vn = dynamic_cast<VariableNode *>(node)) {


  } else if (auto *tn = dynamic_cast<TerminalNode *>(node)) {
    return tn->v;
  } else if (auto *un = dynamic_cast<UnaryNode *>(node)) {
  } else if (auto *bnn = dynamic_cast<BinaryNode *>(node)) {
    Value *left = evaluate(bnn->left);
    Value *right = evaluate(bnn->right);

  } else if (auto *fnn = dynamic_cast<FunctionNode *>(node)) {
  } else {
    return nullptr;
  }
}

void Interpreter::visit(Node *node) {
  if (!node) {
    return;
  }

  if (auto *bn = dynamic_cast<BlockNode *>(node)) {
    for (Node *s : bn->statements) {
      visit(s);
    }

  } else if (auto *vn = dynamic_cast<VariableNode *>(node)) {
    if (vn->is_definition) {
      scope[scope_index].push_back(vn);
    } else {
      evaluate(vn);
    }
  } else if (auto *tn = dynamic_cast<TerminalNode *>(node)) {
  } else if (auto *un = dynamic_cast<UnaryNode *>(node)) {
  } else if (auto *bnn = dynamic_cast<BinaryNode *>(node)) {
  } else if (auto *in = dynamic_cast<IfNode *>(node)) {
  } else if (auto *wn = dynamic_cast<WhileNode *>(node)) {
  } else if (auto *fn = dynamic_cast<ForNode *>(node)) {
  } else if (auto *fnn = dynamic_cast<FunctionNode *>(node)) {
  } else {}
}

void Interpreter::execute() {

}

