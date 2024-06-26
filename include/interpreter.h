#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"
#include "parser.h"
#include "lexer.h"
#include <vector>

/**
 * The Interpreter class executes the abstract syntax tree (AST) generated by the Parser.
 * It maintains a runtime environment, manages scopes, and handles variable and function evaluations.
 */
class Interpreter {
private:
  BlockNode* ast; ///< Pointer to the root of the AST.

  bool debug_mode; ///< Flag to enable debug mode which provides detailed logs.

  std::vector<std::vector<Node*>> scope; ///< Stack of scopes, each containing a list of nodes (variables/functions).
  size_t scope_index; ///< Current index in the scope stack.

  /**
   * Increases the scope level.
   */
  void scope_increase();

  /**
   * Decreases the scope level and pops the top scope.
   */
  void scope_decrease();

  /**
   * Prints the current state of all scopes, useful for debugging.
   */
  void print_scope();

  /**
   * Retrieves the function defintion matching the identifier
   * @param identifier The name of the function
   */
  FunctionNode *get_function_from_scope(std::string identifier);

  /**
   * Retrieves the value of a variable from the current scope.
   * @param identifier The name of the variable.
   * @return Value* Pointer to the Value object associated with the variable.
   */
  Value* get_variable_value(std::string identifier);

  /**
   * Sets or updates the value of a variable in the current scope.
   * @param identifier The name of the variable.
   * @param new_value Pointer to the new value to be assigned to the variable.
   */
  void set_variable_value(std::string identifier, Value* new_value);

  /**
   * Defines a variable by evaluating its initializer and storing it in the current scope.
   * @param vn Pointer to the VariableNode representing the variable definition.
   */
  void define_variable(VariableNode* vn);

  /**
   * Evaluates a function call by setting up the environment, evaluating parameters, and executing the function body.
   * @param identifier Name of the function.
   * @param parameters Vector of pointers to Values passed as arguments to the function.
   * @return Value* Result of the function execution.
   */
  Value* evaluate_function(std::string identifier, std::vector<Value*> parameters);

  /**
   * Evaluates an AST node and returns its value.
   * @param node Pointer to the node to be evaluated.
   * @return Value* Result of the evaluation.
   */
  Value* evaluate(Node* node);

  /**
   * Visits an AST node and performs actions based on its type.
   * @param node Pointer to the node to be visited.
   */
  void visit(Node* node);

public:
  /**
   * Constructor that initializes the interpreter with the provided code.
   * @param code The source code to be interpreted.
   * @param debug_mode Whether debugging is enabled.
   */
  Interpreter(std::string code, bool debug_mode);

  /**
   * Destructor that cleans up the AST and other dynamically allocated resources.
   */
  ~Interpreter();

  /**
   * Executes the program by visiting and evaluating the AST.
   */
  void execute();
};

#endif // INTERPRETER_H
