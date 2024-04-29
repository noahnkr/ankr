#include "../include/interpreter.h"
#include <sstream>
#include <stdexcept>

Interpreter::Interpreter(BlockNode* ast) : ast(ast), scope(), scope_index() {};

Interpreter::~Interpreter() { delete ast; };


Value* Interpreter::get_variable_value(std::string identifier) {
  // Find's variable value in scope. Local variables get precedence over global variables.
  for (size_t i = scope_index; i >= 0; i--) {
    for (Node *n : scope[i]) {
      if (auto *vn = dynamic_cast<VariableNode *>(n)) {
        if (vn->identifier.value == identifier) {
          return evaluate(vn->initializer);
        }
      } 
    }
  }

  // Runtime error thrown if the variable is not defined
  std::ostringstream msg;
  msg << "Variable " << identifier << " is not defined in this scope";
  throw std::runtime_error(msg.str());
}


void Interpreter::set_variable_value(std::string identifier, Value *new_value) {
  // Find's variable in scope and assigns new value. Local variables get precedence over global variables.
  for (size_t i = scope_index; i >= 0; i--) {
    for (Node *n : scope[i]) {
      if (auto *vn = dynamic_cast<VariableNode *>(n)) {
        if (vn->identifier.value == identifier) {
          vn->initializer = new TerminalNode(new_value);
          return;
        }
      } 
    }
  }

  // Runtime error thrown if the variable is not defined
  std::ostringstream msg;
  msg << "Variable " << identifier << " is not defined in this scope";
  throw std::runtime_error(msg.str());
}

Value* Interpreter::evaluate_function(std::string identifier, std::vector<Value *> parameters) {
  FunctionNode *func;
  for (size_t i = scope_index; i >= 0; i--) {
    for (Node *n : scope[i]) {
      if (auto *fnn = dynamic_cast<FunctionNode *>(n)) {
        if (fnn->identifier.value == identifier) {
          func = fnn;
          break;
        }
      }
    }
  }

  // Runtime error thrown if the function is not defined
  if (!func) {
    std::ostringstream msg;
    msg << "Function " << identifier << " is not defined in this scope";
    throw std::runtime_error(msg.str());
  }

  // Number of parameters must be equal.
  if (func->parameters.size() != parameters.size()) {
    std::ostringstream msg;
    if (func->parameters.size() > parameters.size()) {
      msg << "Too few arguments to function '";
    } else {
      msg << "Too many arguments to function '";
    }
    msg << identifier << "'. Expected: " << func->parameters.size() << " "
        << "Actual: " << parameters.size();
    throw std::runtime_error(msg.str());
  }

  scope.push_back(std::vector<Node *>());
  scope_index++;

  // Define temporary parameter variables in new scope
  for (size_t i = 0; i < parameters.size(); i++) {
    Node *p = func->parameters[i];
    if (auto *parameter = dynamic_cast<VariableNode *>(p)) {
      // Assign each parameter's respective values being passed
      TerminalNode *value = new TerminalNode(parameters[i]);
      VariableNode *parameter_variable = new VariableNode(parameter->identifier, value, false);
      scope[scope_index].push_back(parameter_variable);
    } else {
      throw std::runtime_error("Function parameter must be an identifier");
    }
  }

  Value *ret = new VoidValue();
  for (Node *s : func->body->statements) {
    if (auto *is_return = dynamic_cast<UnaryNode *>(s)) {
      if (is_return->token.type == RETURN) {
        ret = evaluate(s);
        break;
      } else {
        visit(s);
      }
    } else {
      visit(s);
    }
  }

  scope.pop_back();
  scope_index--;

  return ret;
}

Value* Interpreter::evaluate(Node *node) {
  if (!node) {
    throw std::runtime_error("Invalid node structure"); // Should never happen...
  }

  if (auto *vn = dynamic_cast<VariableNode *>(node)) {
    return get_variable_value(vn->identifier.value);

  } else if (auto *tn = dynamic_cast<TerminalNode *>(node)) {
    return tn->v;

  } else if (auto *un = dynamic_cast<UnaryNode *>(node)) {
    Value *child = evaluate(un->child);
    return child->apply_operator(un->token, nullptr); // Unary operation applies to child, 
                                                      // therefore no need for 2nd node.

  } else if (auto *bnn = dynamic_cast<BinaryNode *>(node)) {
    Value *left = evaluate(bnn->left);
    Value *right = evaluate(bnn->right);
    return left->apply_operator(bnn->token, right);

  } else if (auto *fnn = dynamic_cast<FunctionNode *>(node)) {
    std::vector<Value *> parameters;
    for (Node *p : fnn->parameters) {
      parameters.push_back(evaluate(p));
    }

    return evaluate_function(fnn->identifier.value, parameters);

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

  } else if (auto *un = dynamic_cast<UnaryNode *>(node)) {
    if (un->token.type == RETURN && scope_index == 0) {
      throw std::runtime_error("Return is not allowed here");
    }

    if (auto *variable = dynamic_cast<VariableNode *>(un->child)) {
      Value *stored_value = evaluate(un);
      set_variable_value(variable->identifier.value, stored_value);
    } else {
      throw std::runtime_error("Expression is not assignable");
    }

  } else if (auto *bnn = dynamic_cast<BinaryNode *>(node)) {
    if (is_assign(bnn->token.type)) {
      Token assign_operator = bnn->token;
      VariableNode *variable = dynamic_cast<VariableNode *>(bnn->left);
      Value *variable_value = get_variable_value(variable->identifier.value);
      Value *stored_value = variable_value->apply_operator(assign_operator, evaluate(bnn->right));
      std::cout << "Assigning value " << stored_value->to_string() << " to variable '" << variable->identifier.value << "'";  
      set_variable_value(variable->identifier.value, stored_value);
    } else {
      evaluate(bnn);
    }
  } else if (auto *in = dynamic_cast<IfNode *>(node)) {
  } else if (auto *wn = dynamic_cast<WhileNode *>(node)) {
  } else if (auto *fn = dynamic_cast<ForNode *>(node)) {
  } else if (auto *fnn = dynamic_cast<FunctionNode *>(node)) {
    if (fnn->is_definition) {
      scope[scope_index].push_back(fnn);
    } else {
      evaluate(fnn);
    }
  } else {}
}

void Interpreter::execute() {
  visit(ast);
}
