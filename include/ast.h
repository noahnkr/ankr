#ifndef AST_H
#define AST_H

#include "token.h"
#include <vector>

struct Node {
  virtual ~Node() = default;
};

struct BlockNode : Node {
  std::vector<Node *> statements;

  BlockNode(std::vector<Node *> statements) : statements(statements) {}
  ~BlockNode() {
    for (Node *n : statements) {
      delete n;
    }
  }
};

struct TerminalNode : Node {
  Token token;
  TerminalNode(Token token) : token(token) {}
  ~TerminalNode() {}
};

struct UnaryNode : Node {
  Token token;
  Node *child;
  UnaryNode(Token token, Node *child) : token(token), child(child) {}
  ~UnaryNode() { delete child; }
};

struct BinaryNode : Node {
  Token token;
  Node *left;
  Node *right;
  BinaryNode(Token token, Node *left, Node *right)
      : token(token), left(left), right(right) {}
  ~BinaryNode() {
    delete left;
    delete right;
  }
};

struct IfNode : Node {
  Node *condition;
  Node *if_body;
  Node *else_body;
  IfNode(Node *condition, Node *if_body, Node *else_body)
      : condition(condition), if_body(if_body), else_body(else_body) {}
  ~IfNode() {
    delete condition;
    delete if_body;
    delete else_body;
  }
};

struct WhileNode : Node {
  Node *condition;
  Node *body;
  WhileNode(Node *condition, Node *body) : condition(condition), body(body) {}
  ~WhileNode() {
    delete condition;
    delete body;
  }
};

struct ForNode : Node {
  Node *initialization;
  Node *condition;
  Node *update;
  Node *body;
  ForNode(Node *initialization, Node *condition, Node *update, Node *body)
      : initialization(initialization), condition(condition), update(update),
        body(body) {}
  ~ForNode() {
    delete initialization;
    delete condition;
    delete update;
    delete body;
  }
};

#endif // AST_H
