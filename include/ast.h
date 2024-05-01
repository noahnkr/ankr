#ifndef AST_H
#define AST_H

#include "token.h"
#include "value.h"
#include <vector>

/**
 * Base class for all AST nodes.
 * Provides a virtual destructor and a pure virtual method for converting nodes to strings.
 */
struct Node {
  virtual ~Node() = default;
  virtual std::string to_string() const = 0;
};

/**
 * Represents a block of statements, typically enclosed by curly braces.
 * Used in control structures and function bodies.
 */
struct BlockNode : Node {
  std::vector<Node*> statements;

  BlockNode(std::vector<Node*> statements) : statements(std::move(statements)) {}
  ~BlockNode() {
    for (Node* n : statements) {
      delete n;
    }
  }

  std::string to_string() const override { return "Block"; }
};

/**
 * Represents a variable in the program. This node can be used for both variable declarations
 * and variable references, depending on the context provided by 'is_definition'.
 */
struct VariableNode : Node {
  Token identifier;
  Node* initializer;
  bool is_definition;

  VariableNode(Token identifier, Node* initializer, bool is_definition)
      : identifier(std::move(identifier)), initializer(initializer), is_definition(is_definition) {}
  ~VariableNode() { delete initializer; }

  std::string to_string() const override { return identifier.value; }
};

/**
 * Represents a function in the program. This can be a function definition or a function call,
 * determined by 'is_definition'. Parameters are represented as nodes which can be either variable declarations
 * or expressions depending on whether it's a definition or a call.
 */
struct FunctionNode : Node {
  Token identifier;
  std::vector<Node*> parameters;
  BlockNode* body;
  bool is_definition;

  FunctionNode(Token identifier, std::vector<Node*> parameters, BlockNode* body, bool is_definition)
      : identifier(std::move(identifier)), parameters(std::move(parameters)), body(body), is_definition(is_definition) {}
  ~FunctionNode() {
    for (Node* n : parameters) {
      delete n;
    }
    delete body;
  }

  std::string to_string() const override {
    return (is_definition ? "function " : "call ") + identifier.value;
  }
};

/**
 * Represents a literal or a simple value in the program. 
 * This is a terminal node in the AST that directly contains a value.
 */
struct TerminalNode : Node {
  Value* v;

  TerminalNode(Value* v) : v(v) {}
  ~TerminalNode() { delete v; }

  std::string to_string() const override { return v->to_string(); }
};

/**
 * Represents a unary operation, such as negation, increment, or logical not.
 */
struct UnaryNode : Node {
  Token token;
  Node* child;

  UnaryNode(Token token, Node* child) : token(std::move(token)), child(child) {}
  ~UnaryNode() { delete child; }

  std::string to_string() const override { return token.value + " " + child->to_string(); }
};

/**
 * Represents a binary operation, such as addition, subtraction, or assignment.
 */
struct BinaryNode : Node {
  Token token;
  Node* left;
  Node* right;

  BinaryNode(Token token, Node* left, Node* right)
      : token(std::move(token)), left(left), right(right) {}
  ~BinaryNode() {
    delete left;
    delete right;
  }

  std::string to_string() const override {
    return "(" + left->to_string() + " " + token.value + " " + right->to_string() + ")";
  }
};

/**
 * Represents an if-statement in the program, including 'if', 'else if', and 'else' branches.
 */
struct IfNode : Node {
  Node* condition;
  Node* true_body;
  Node* false_body;

  IfNode(Node* condition, Node* true_body, Node* false_body)
      : condition(condition), true_body(true_body), false_body(false_body) {}
  ~IfNode() {
    delete condition;
    delete true_body;
    delete false_body;
  }

  std::string to_string() const override { return "if " + condition->to_string(); }
};

/**
 * Represents a while-loop in the program.
 */
struct WhileNode : Node {
  Node* condition;
  BlockNode* body;

  WhileNode(Node* condition, BlockNode* body)
      : condition(condition), body(body) {}
  ~WhileNode() {
    delete condition;
    delete body;
  }

  std::string to_string() const override { return "while " + condition->to_string(); }
};

/**
 * Represents a for-loop in the program. Consists of initialization, condition check, update, and body.
 */
struct ForNode : Node {
  Node* initialization;
  Node* condition;
  Node* update;
  BlockNode* body;

  ForNode(Node* initialization, Node* condition, Node* update, BlockNode* body)
      : initialization(initialization), condition(condition), update(update), body(body) {}
  ~ForNode() {
    delete initialization;
    delete condition;
    delete update;
    delete body;
  }

  std::string to_string() const override { return "for"; }
};

#endif // AST_H
