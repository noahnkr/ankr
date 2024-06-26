#include "../include/parser.h"
#include <stack>
#include <vector>
#include <iostream>

Parser::Parser(std::vector<Token> tokens, bool debug_mode) : tokens(tokens), pos(0), debug_mode(debug_mode) {}
Parser::~Parser() {}

Token Parser::peek() {
  if (at_end()) {
    return {END_FILE, ""};
  }
  return tokens[pos];
}

Token Parser::advance() {
  if (!at_end())
    pos++;
  return tokens[pos - 1];
}

void Parser::consume(TokenType expected, std::string message) {
  if (peek().type == expected) {
    advance();
    return;
  }

  throw std::runtime_error(message);
}


Node *Parser::parse_expression() {
  int parenthesis_index = 0;
  std::vector<Token> infix;
  std::vector<FunctionNode *> functions;
  std::vector<VariableNode *> variables;

  // Keep reading tokens until we reach a semicolon/comma or the wrapping parenthesis.
  while (!(at_end() || peek().type == END_STATEMENT || peek().type == COMMA ||
           (peek().type == RIGHT_PARENTHESIS && parenthesis_index == 0))) {
    Token t = advance();
    if (debug_mode) {
      std::cout <<  t.value << std::endl;
    }
    if (t.type == LEFT_PARENTHESIS) {
      parenthesis_index++;
    } else if (t.type == RIGHT_PARENTHESIS) {
      parenthesis_index--;
    }

    if (t.type == IDENTIFIER) {
      // Function Call
      if (peek().type == LEFT_PARENTHESIS) {
        FunctionNode *function_root = parse_function(false);
        function_root->identifier = t;
        functions.push_back(function_root);
        infix.push_back({FUNCTION, t.value});

      // Variable
      } else {
        VariableNode *variable_root = new VariableNode(t, nullptr, false);
        variables.push_back(variable_root);
        infix.push_back({VAR, t.value});
      }

      continue;
    }

    infix.push_back(t);
  }

  // Skip semicolon/comma
  if (peek().type == END_STATEMENT || peek().type == COMMA) {
    advance();
  }

  if (debug_mode) {
    std::cout << "Infix: ";
    for (Token t : infix) {
      std::cout << t.value << " ";
    }
    std::cout << std::endl;
  }

  // Convert tokens to postfix
  std::vector<Token> postfix = to_postfix(infix);

  if (debug_mode) {
    std::cout << "Postfix: ";
    for (Token t : postfix) {
      std::cout << t.value << " ";
    }
    std::cout << std::endl;
  }

  // Build Expression Tree
  std::stack<Node *> stack;
  for (Token t : postfix) {
    if (is_operand(t.type)) {
      Node *operand;
      if (t.type == INT) {
        IntValue *val = new IntValue(stoi(t.value));
        operand = new TerminalNode(val);

      } else if (t.type == FLOAT) {
        FloatValue *val = new FloatValue(stof(t.value));
        operand = new TerminalNode(val);

      } else if (t.type == TRUE || t.type == FALSE) {
        BoolValue *val = new BoolValue(t.value == "true");
        operand = new TerminalNode(val);

      } else if (t.type == STRING) {
        StringValue *val = new StringValue(t.value);
        operand = new TerminalNode(val);

      } else if (t.type == VAR) {
        for (VariableNode *vn : variables) {
          if (vn->identifier.value  == t.value) {
            operand = vn;
            break;
          }
        }
      } else if (t.type == FUNCTION) {
        for (FunctionNode *fn : functions) {
          if (fn->identifier.value == t.value) {
            operand = fn;
            break;
          }
        }
      }    
      stack.push(operand);
    } else if (is_operator(t.type)) {
      if (is_unary(t.type)) {
        Node *operand = stack.top();
        stack.pop();
        stack.push(new UnaryNode(t, operand));

      } else {
        Node *right = stack.top();
        stack.pop();
        Node *left = stack.top();
        stack.pop();
        stack.push(new BinaryNode(t, left, right));
      }
    }
  }


  if (stack.empty()) {
    return nullptr;
  }

  // Root node is last element in stack
  Node *root = stack.top();
  stack.pop();
  return root;
}

std::vector<Token> Parser::to_postfix(std::vector<Token> infix) {
  std::vector<Token> postfix;
  std::stack<Token> stack;

  for (size_t i = 0; i < infix.size(); i++) {
    Token t = infix[i];
    if (is_operand(t.type)) {
      postfix.push_back(t);
    } else if (t.type == LEFT_PARENTHESIS) {
      stack.push(t);
    } else if (t.type == RIGHT_PARENTHESIS) {
      while (!stack.empty() && stack.top().type != LEFT_PARENTHESIS) {
        postfix.push_back(stack.top());
        stack.pop();
      }
      stack.pop(); // Remove the parenthesis from stack
    } else if (is_operator(t.type)) {
      while (!stack.empty() &&
             precedence(t.type) <= precedence(stack.top().type)) {
        postfix.push_back(stack.top());
        stack.pop();
      }
      stack.push(t);
    }
  }

  // Append remaining symbols to postfix
  while (!stack.empty()) {
    postfix.push_back(stack.top());
    stack.pop();
  }

  return postfix;
}

IfNode *Parser::parse_if() {
  consume(IF, "Expected 'if'");
  consume(LEFT_PARENTHESIS, "Expected '(' after 'if'");
  Node *condition = parse_expression();
  consume(RIGHT_PARENTHESIS, "Expected ')' after 'if' condition");
  Node *true_body = parse_block();
  Node *false_body = nullptr;

  // Check for 'else if' or 'else'
  if (peek().type == ELSE) {
    advance();
    if (peek().type == IF) {
      false_body = parse_if(); // Recursively parse nested if statements
    } else {
      false_body = parse_block();
    }
  }

  return new IfNode(condition, true_body, false_body);
}

WhileNode *Parser::parse_while() {
  consume(WHILE, "Expected 'while'");
  consume(LEFT_PARENTHESIS, "Expected '(' after 'while'");
  Node *condition = parse_expression();
  consume(RIGHT_PARENTHESIS, "Expected ')' after 'while' condition");
  BlockNode* body = parse_block();
  return new WhileNode(condition, body);
}

VariableNode *Parser::parse_variable(bool is_definition) {
  Token identifier;
  Node *initializer = nullptr;

  if (is_definition) {
    consume(VAR, "Expected 'var' declaration");
    identifier = peek();
    initializer = parse_expression();
  } else {
    identifier = advance();
  }

  return new VariableNode(identifier, initializer, is_definition);
}

FunctionNode *Parser::parse_function(bool is_definition) {
  Token identifier;
  if (is_definition) {
    consume(FUNCTION, "Expected 'function' before identifier");
    identifier = advance();
  } else {
    identifier = {IDENTIFIER, ""}; // Identifier already read, will be set after expression is parsed.
  }

  consume(LEFT_PARENTHESIS, "Expected '(' after identifier");
  std::vector<Node *> parameters;

  while (peek().type != RIGHT_PARENTHESIS) {
    Node *parameter = parse_expression();
    parameters.push_back(parameter);
  }
  consume(RIGHT_PARENTHESIS, "Expected ')' after parameters");

  BlockNode *body = nullptr;
  if (is_definition) {
    body = parse_block();
  }

  return new FunctionNode(identifier, parameters, body, is_definition);
}

UnaryNode *Parser::parse_return() {
  consume(RETURN, "Expected 'return'");
  return new UnaryNode({RETURN, "return"}, parse_expression());
}

ForNode *Parser::parse_for() {
  consume(FOR, "Expected 'for'");
  consume(LEFT_PARENTHESIS, "Expected '(' after 'for'");

  Node *initialization = parse_statement();
  Node *condition = parse_expression();
  Node *update = parse_expression();

  consume(RIGHT_PARENTHESIS, "Expected ')' after 'for' condition");
  BlockNode *body = parse_block();
  return new ForNode(initialization, condition, update, body);
}

BlockNode *Parser::parse_block() {
  std::vector<Node *> statements;

  consume(LEFT_BRACKET, "Expected '{' at start of block");

  while (!(at_end() || peek().type == RIGHT_BRACKET)) {
    statements.push_back(parse_statement());
  }

  consume(RIGHT_BRACKET, "Expected '}' at end of block");

  return new BlockNode(statements);
}

Node *Parser::parse_statement() {
  switch (peek().type) {
    case IF: return parse_if();
    case WHILE: return parse_while();
    case FOR: return parse_for();
    case VAR: return parse_variable(true);
    case FUNCTION: return parse_function(true);
    case RETURN: return parse_return();
    default: return parse_expression();
  }
}

BlockNode *Parser::parse() {
  std::vector<Node *> root_statements;

  while (!at_end()) {
    root_statements.push_back(parse_statement());
  }

  return new BlockNode(root_statements);
}

std::string Parser::draw_tree(BlockNode *root) {
  if (!root) {
    return "";
  }

  std::string tree = "";
  for (Node *root_statement : root->statements) {
    draw_tree_rec(root_statement, &tree, "", "", false);
  }

  tree.erase(0, 1); // Remove first newline character
  return tree;
}

void Parser::draw_tree_rec(Node *node, std::string *tree, std::string padding,
                           std::string pointer, bool has_next) {
  if (!node) {
    return;
  }

  *tree += '\n' + padding + pointer + node->to_string();

  std::string padding_temp(padding);
  if (has_next) {
    padding_temp += "│  ";
  } else {
    padding_temp += "   ";
  }

  if (auto *bn = dynamic_cast<BlockNode *>(node)) {
    for (size_t i = 0; i < bn->statements.size(); i++) {
      Node *statement = bn->statements[i];
      std::string pointer = i + 1 < bn->statements.size() ? "├──" : "└──";
      draw_tree_rec(statement, tree, padding_temp, pointer, i + 1 < bn->statements.size());
    }

  } else if (auto *vn = dynamic_cast<VariableNode *>(node)) {
    draw_tree_rec(vn->initializer, tree, padding_temp, "└──", false);

  } else if (auto *un = dynamic_cast<UnaryNode *>(node)) {
    draw_tree_rec(un->child, tree, padding_temp, "└──", false);

  } else if (auto *bnn = dynamic_cast<BinaryNode *>(node)) {
    std::string pointer_right = "└──";
    std::string pointer_left = bnn->right ? "├──" : "└──";
    draw_tree_rec(bnn->left, tree, padding_temp, pointer_left, bnn->right);
    draw_tree_rec(bnn->right, tree, padding_temp, pointer_right, false);

  } else if (auto *in = dynamic_cast<IfNode *>(node)) {
    draw_tree_rec(in->condition, tree, padding_temp, "├──", true);
    std::string true_pointer = in->false_body ? "├──" : "└──";
    draw_tree_rec(in->true_body, tree, padding_temp, true_pointer, in->false_body);

    // Else if
    if (auto *else_if_node = dynamic_cast<IfNode *>(in->false_body)) {
      std::string false_pointer =  !else_if_node->false_body ? "├──" : "└──";
      draw_tree_rec(in->false_body, tree, padding_temp, false_pointer, false);

    // Else
    } else if (auto *else_node = dynamic_cast<BlockNode *>(in->false_body)) {
      draw_tree_rec(in->false_body, tree, padding_temp, "└──", else_node->statements.size() > 1); 
    }

  } else if (auto *wn = dynamic_cast<WhileNode *>(node)) {
    draw_tree_rec(wn->condition, tree, padding_temp, "├──", true);
    draw_tree_rec(wn->body, tree, padding_temp, "└──", false);

  } else if (auto *fn = dynamic_cast<ForNode *>(node)) {
    draw_tree_rec(fn->initialization, tree, padding_temp, "├──", true);
    draw_tree_rec(fn->condition, tree, padding_temp, "├──", true);
    draw_tree_rec(fn->update, tree, padding_temp, "├──", true);
    draw_tree_rec(fn->body, tree, padding_temp, "└──", false);

  } else if (auto *fnn = dynamic_cast<FunctionNode *>(node)) {
    // If its a function call, draw the function's paramter nodes.
    for (size_t i = 0; i < fnn->parameters.size() && !fnn->is_definition; i++) {
      Node *paramter = fnn->parameters[i];
      std::string pointer = i + 1 < fnn->parameters.size() ? "├──" : "└──";
      draw_tree_rec(paramter, tree, padding_temp, pointer, i + 1 < fnn->parameters.size());
    }

    // Draw the body of the function definition
    if (fnn->is_definition) {
      draw_tree_rec(fnn->body, tree, padding_temp, "└──", false);

    }
  }
}

bool Parser::at_end() { return pos >= tokens.size(); }
