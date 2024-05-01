#ifndef AST_H
#define AST_H

#include "token.h"
#include "value.h"
#include <vector>

/*
 * Abstract Node Class
 */
struct Node {
  virtual ~Node() = default;
  virtual std::string to_string() const = 0;
};

/*
 * Node that stores a list of statements. Represents a code "block" surrounded
 * by curly braces used in 'if', 'while', 'for', and function definitions.
 */
struct BlockNode : Node {
  std::vector<Node *> statements;

  std::string to_string() const override { return "[]"; }

  BlockNode(std::vector<Node *> statements) : statements(statements) {}
  ~BlockNode() {
    for (Node *n : statements) {
      delete n;
    }
  }
};

/*
 * Stores information for a variable. This Node has two mutually exclusive uses
 * which are for variable definitions and variable calls. For definitions, it stores the 
 * evaluated value of the initializer sub-tree (for definitions, i.e 'var x = 10'). For calls,
 * it stores an identifier of variable with no initializer, whos value will be * found later.
 */
struct VariableNode : Node {
  Token identifier;
  Node *initializer;
  bool is_definition;

  std::string to_string() const override { return is_definition ? "var" : identifier.value; }
  VariableNode(Token identifier, Node *initializer, bool is_definition)
      : identifier(identifier), initializer(initializer),
        is_definition(is_definition) {}
  ~VariableNode() { delete initializer; }
};

/*
 * Similar to the variable node, this also has two functionalities for function definitions and function calls.
 * This is achieved storing the identifier of the function and either storing empty VariableNodes with just the
 * identifier for function parameters on defition, or a root of an expression node for function call parameters.
 */
struct FunctionNode : Node {
  Token identifier;
  std::vector<Node *> parameters;
  BlockNode *body;
  bool is_definition;

  std::string to_string() const override { 
    std::string ret;
    ret += is_definition ? "function " : "";
    ret += identifier.value + "(";
    for (size_t i = 0; i < parameters.size() && !is_definition; i++) {
      Node *p = parameters[i];
      ret += p->to_string();
      if (i + 1 < parameters.size())
        ret += ", ";
    }
    ret += ")";
    return ret;
  }
  FunctionNode(Token identifier, std::vector<Node *> parameters, BlockNode *body, bool is_definition)
      : identifier(identifier), parameters(parameters), body(body), is_definition(is_definition) {}
  ~FunctionNode() { 
    for (Node *n : parameters) {
      delete n;
    }
    delete body; 
  }
};

/*
 * Leaf node that stores an operand (int, float, string, or bool).
 */
struct TerminalNode : Node {
  Value *v;

  std::string to_string() const override { return v->to_string(); }
  TerminalNode(Value *v) : v(v) {}
  ~TerminalNode() {}
};

/*
 * Unary operator, ('++', '--', '!', '-', 'return').
 */
struct UnaryNode : Node {
  Token token;
  Node *child;

  std::string to_string() const override { return token.value; }
  UnaryNode(Token token, Node *child) : token(token), child(child) {}
  ~UnaryNode() { delete child; }
};

/*
 * Binary operator, i.e ('+', '-', '&&', '>=', '=', '*=', etc).
 */
struct BinaryNode : Node {
  Token token;
  Node *left;
  Node *right;

  std::string to_string() const override { return token.value; }
  BinaryNode(Token token, Node *left, Node *right)
      : token(token), left(left), right(right) {}
  ~BinaryNode() {
    delete left;
    delete right;
  }
};

/*
 * Represents an if node which stores the root of the condition expression. If the expression
 * is evaluated to be true, the true_body BlockNode will be visited. The false_body can either
 * be a BlockNode or another chained if statement for 'else if'.
 */
struct IfNode : Node {
  Node *condition;
  Node *true_body;
  Node *false_body; // Used to chain if statements

  std::string to_string() const override { return "if"; }
  IfNode(Node *condition, Node *true_body, Node *false_body)
      : condition(condition), true_body(true_body), false_body(false_body) {}
  ~IfNode() {
    delete condition;
    delete true_body;
    delete false_body;
  }
};

/*
 * Represents a while loop. If the condition is evaluated to be true, the body BlockNode will be visied.
 */
struct WhileNode : Node {
  Node *condition;
  BlockNode *body;

  std::string to_string() const override { return "while"; }
  WhileNode(Node *condition, BlockNode *body) : condition(condition), body(body) {}
  ~WhileNode() {
    delete condition;
    delete body;
  }
};

/*
 * Represents a for loop. Performs the initialization statement, evaluates the
 * condition, if true, it visits the body BlockNode and then updates.
 */
struct ForNode : Node {
  Node *initialization;
  Node *condition;
  Node *update;
  BlockNode *body;

  std::string to_string() const override { return "for"; }
  ForNode(Node *initialization, Node *condition, Node *update, BlockNode *body)
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
