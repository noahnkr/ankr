#include "../include/interpreter.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

// TODO:
//  - Break statement
//  - Handle comments
//  - Better error handling with line numbers

Interpreter::Interpreter(std::string code, bool debug_mode)
    : ast(), debug_mode(debug_mode), scope(), scope_index() {

  scope.push_back(std::vector<Node *>()); // Global Scope
  Lexer lexer(code);

  if (debug_mode) {
    std::cout << "Creating Tokens...";
  }

  std::vector<Token> tokens = lexer.tokenize();

  if (debug_mode) {
    std::cout << "Tokens: ";
    for (Token t : tokens) {
      std::cout << t.value << " ";
    }
    std::cout << std::endl;
  }

  Parser parser(tokens, debug_mode);
  ast = parser.parse();

  if (debug_mode) {
    std::cout << "AST:" << std::endl << Parser::draw_tree(ast) << std::endl;
  }
};

Interpreter::~Interpreter() {
  delete ast;
};

void Interpreter::scope_increase() {
  scope.push_back(std::vector<Node *>());
  scope_index++;
}

void Interpreter::scope_decrease() {
  scope.pop_back();
  scope_index--;
}

void Interpreter::print_scope() {
  int level = 0;
  for (std::vector<Node *> s : scope) {
    std::cout << "Level " << level << ": ";
    if (s.size() == 0) {
      std::cout << "Empty";
    }
    for (size_t i = 0; i < s.size(); i++) {
      Node *n = s[i];
      std::cout << "{ " << n->to_string();
      if (auto *variable = dynamic_cast<VariableNode *>(n)) {
        std::cout << ": " << dynamic_cast<TerminalNode *>(variable->initializer)->to_string();
      }

      std::cout << " }";

      if (i + 1 < s.size()) {
        std::cout << ", ";
      }
    }
    level++;
    std::cout << std::endl;
  }
}

FunctionNode *Interpreter::get_function_from_scope(std::string identifier) {
  // Iterate from the current scope back to the global scope
  for (size_t i = scope_index; i >= 0; i--) {
    for (Node *n : scope[i]) {
      if (auto *func_node = dynamic_cast<FunctionNode *>(n)) {
        if (func_node->identifier.value == identifier) {
          return func_node;
        }
      }
    }
  }
  return nullptr; // Return nullptr if the function is not found in any scope
}

Value *Interpreter::get_variable_value(std::string identifier) {
  // Find's variable value in scope. Local variables get precedence over global
  // variables.
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
  // Find's variable in scope and assigns new value. Local variables get
  // precedence over global variables.
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

Value *Interpreter::evaluate_function(std::string identifier,
                                      std::vector<Value *> parameters) {
  // TODO: Modulize to include libraries with standard functions
  if (identifier == "input") {
    if (parameters.size() != 0) {
      std::ostringstream msg;
      msg << "Too many parameters. Expected: 0, Actual: " << parameters.size();
      std::runtime_error(msg.str());
    } else {
      std::string input;
      std::getline(std::cin, input);
      bool is_int = false;
      bool is_float = false;

      for (char c : input) {
        if (std::isdigit(c)) {
          is_int = true;
        } else if (c == '.' && is_int) {
          is_float = true;
        }
      }

      Value *input_value;
      if (is_int && !is_float) {
        input_value = new IntValue(stoi(input));
      } else if (is_float) {
        input_value = new FloatValue(stof(input));
      } else {
        if (input == "true") {
          input_value = new BoolValue(true);
        } else if (input == "false") {
          input_value = new BoolValue(false);
        } else {
          input_value = new StringValue(input);
        }
      }

      return input_value;
    }
  } else if (identifier == "output") {
    if (parameters.size() > 1 || parameters.size() < 1) {
      std::ostringstream msg;
      msg << "Too " << (parameters.size() > 1 ? "many " : "few ")
          << "parameters. Expected: 1, Actual: " << parameters.size();
      std::runtime_error(msg.str());
    } else {
      std::string output_value = parameters[0]->to_string();
      std::cout << output_value << std::endl;
      return new VoidValue();
    }

  } else if (identifier == "rand") {
    if (parameters.size() > 1 || parameters.size() < 1) {
      std::ostringstream msg;
      msg << "Too " << (parameters.size() > 1 ? "many " : "few ")
          << "parameters. Expected: 1, Actual: " << parameters.size();
      std::runtime_error(msg.str());
    } else {
      if (auto *rand_ciel = dynamic_cast<IntValue *>(parameters[0])) {
        return new IntValue(rand() % rand_ciel->value);
      } else {
        std::ostringstream msg;
        msg << "Invalid parameter type. Expected: 'int', Actual: '"
            << parameters[0]->to_string() << "'" << std::endl;
        std::runtime_error(msg.str());
      }
    }
  }

  FunctionNode *func = get_function_from_scope(identifier);

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

  // Create a new scope for the function call.
  scope_increase();

  if (debug_mode) {
    std::cout << "Defining parameters..." << std::endl;
  }

  // Define temporary parameter variables in new scope
  for (size_t i = 0; i < parameters.size(); i++) {
    Node *p = func->parameters[i];
    if (auto *parameter = dynamic_cast<VariableNode *>(p)) {
      // Assign each parameter's respective values being passed
      TerminalNode *value = new TerminalNode(parameters[i]);
      VariableNode *parameter_variable =
          new VariableNode(parameter->identifier, value, false);
      scope[scope_index].push_back(parameter_variable);
    } else {
      throw std::runtime_error("Function parameter must be an identifier");
    }
  }

  if (debug_mode) {
    std::cout << "Scope:" << std::endl;
    print_scope();
  }

  Value *ret = evaluate(func->body);

  scope_decrease();
  return ret;
}

Value *Interpreter::evaluate(Node *node) {
  if (!node) {
    throw std::runtime_error(
        "Invalid node structure"); // Should never happen...
  }

  if (debug_mode) {
    std::cout << "Evaluating: " << node->to_string() << std::endl;
  }

  // Function Body
  if (auto *bn = dynamic_cast<BlockNode *>(node)) {
    Value *ret = new VoidValue();
    for (Node *s : bn->statements) {
      if (auto *is_return = dynamic_cast<UnaryNode *>(s)) {
        if (is_return->token.type == RETURN) {
          ret = evaluate(is_return);
          break;
        } else {
          visit(s);
        }
      } else {
        visit(s);
      }
    }
    return ret;

  } else if (auto *vn = dynamic_cast<VariableNode *>(node)) {
    if (debug_mode) {
      std::cout << "Scope: " << std::endl;
      print_scope();
    }
    return get_variable_value(vn->identifier.value);

  } else if (auto *tn = dynamic_cast<TerminalNode *>(node)) {
    return tn->v;

  } else if (auto *un = dynamic_cast<UnaryNode *>(node)) {
    Value *child = evaluate(un->child);
    return child->apply_operator(un->token,
                                 nullptr); // Unary operation applies to child,
                                           // therefore no need for 2nd node.

  } else if (auto *bnn = dynamic_cast<BinaryNode *>(node)) {
    Value *left = evaluate(bnn->left);
    Value *right = evaluate(bnn->right);
    return left->apply_operator(bnn->token, right);

  } else if (auto *fnn = dynamic_cast<FunctionNode *>(node)) {
    if (debug_mode) {
      std::cout << "Scope: " << std::endl;
      print_scope();
    }
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

  if (debug_mode) {
    std::cout << "Visiting: " << node->to_string() << std::endl;
  }

  if (auto *bn = dynamic_cast<BlockNode *>(node)) {
    for (Node *s : bn->statements) {
      visit(s);
    }

  } else if (auto *vn = dynamic_cast<VariableNode *>(node)) {
    if (vn->is_definition) {
      VariableNode *variable;
      Value *stored_value;
      if (auto *assign = dynamic_cast<BinaryNode *>(vn->initializer)) {
        variable = dynamic_cast<VariableNode *>(assign->left);
        stored_value = evaluate(assign->right);
      } else {
        variable = dynamic_cast<VariableNode *>(vn->initializer);
        stored_value = new VoidValue();
      }
      VariableNode *stored_variable = new VariableNode(
          variable->identifier, new TerminalNode(stored_value), false);
      scope[scope_index].push_back(stored_variable);
    } else {
      evaluate(vn);
    }

  } else if (auto *un = dynamic_cast<UnaryNode *>(node)) {
    if (un->token.type == RETURN) {
      if (scope_index == 0) {
        throw std::runtime_error("Return is not allowed here.");
      }
      return;
    }

    if (auto *variable = dynamic_cast<VariableNode *>(un->child)) {
      Value *stored_value = evaluate(un);
      set_variable_value(variable->identifier.value, stored_value);
    } else {
    }

  } else if (auto *bnn = dynamic_cast<BinaryNode *>(node)) {
    if (is_assign(bnn->token.type)) {
      Token assign_operator = bnn->token;
      VariableNode *variable = dynamic_cast<VariableNode *>(bnn->left);
      Value *variable_value = get_variable_value(variable->identifier.value);
      Value *stored_value =
          variable_value->apply_operator(assign_operator, evaluate(bnn->right));
      set_variable_value(variable->identifier.value, stored_value);
    } else {
      evaluate(bnn);
    }
  } else if (auto *in = dynamic_cast<IfNode *>(node)) {
    Value *condition_value = evaluate(in->condition);
    if (auto *bool_condition = dynamic_cast<BoolValue *>(condition_value)) {
      if (bool_condition->value) {
        visit(in->true_body);
      } else {
        visit(in->false_body);
      }
    } else {
      throw std::runtime_error("If condition must be a boolean expression");
    }
  } else if (auto *wn = dynamic_cast<WhileNode *>(node)) {
    scope_increase();
    while (true) {
      BoolValue *condition = dynamic_cast<BoolValue *>(evaluate(wn->condition));
      if (!condition || !condition->value) {
        break;
      }
      visit(wn->body);
    }
    scope_decrease();
  } else if (auto *fn = dynamic_cast<ForNode *>(node)) {
    scope_increase();
    visit(fn->initialization);
    while (true) {
      BoolValue *condition = dynamic_cast<BoolValue *>(evaluate(fn->condition));
      if (!condition || !condition->value) {
        break;
      }
      visit(fn->body);
      visit(fn->update);
    }
    scope_decrease();
  } else if (auto *fnn = dynamic_cast<FunctionNode *>(node)) {
    if (fnn->is_definition) {
      scope[scope_index].push_back(fnn);
    } else {
      evaluate(fnn);
    }
  }
}

void Interpreter::execute() { visit(ast); }
