#include "../include/parser.h"
#include <stack>
#include <vector>
#include <iostream>

Parser::Parser(std::vector<Token> tokens) : tokens(tokens), pos(0) {}
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
  std::vector<FunctionNode *> function_calls;

  // Keep reading tokens until we reach a semicolon/comma or the wrapping parenthesis.
  while (!(at_end() || peek().type == END_STATEMENT || peek().type == COMMA ||
           (peek().type == RIGHT_PARENTHESIS && parenthesis_index == 0))) {
    Token t = advance();
    if (t.type == LEFT_PARENTHESIS) {
      parenthesis_index++;
    } else if (t.type == RIGHT_PARENTHESIS) {
      parenthesis_index--;
    }

    if (t.type == IDENTIFIER && peek().type == LEFT_PARENTHESIS) {
      FunctionNode *function_root = parse_function(false);
      function_root->identifier = t;
      function_calls.push_back(function_root);
      infix.push_back({FUNCTION, t.value});
    } else {
      infix.push_back(t);
    }
  }

  // Skip semicolon/comma
  if (peek().type == END_STATEMENT || peek().type == COMMA) {
    advance();
  }

  /*std::cout << "Infix: ";
  for (Token t : infix) {
    std::cout << t.value << " ";
  }
  std::cout << std::endl;*/

  // Convert tokens to postfix
  std::vector<Token> postfix = to_postfix(infix);

  /*std::cout << "Postfix: ";
  for (Token t : postfix) {
    std::cout << t.value << " ";
  }
  std::cout << std::endl;*/

  // Build Expression Tree
  std::stack<Node *> stack;
  for (Token t : postfix) {
    if (is_operand(t.type)) {
      stack.push(new TerminalNode(t));
      
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

    } else if (t.type == FUNCTION) {
      for (FunctionNode *n : function_calls) {
        if (n->identifier.value == t.value) {
          stack.push(n);
        }
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
    if (is_operand(t.type) || t.type == FUNCTION) {
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
      false_body = parse_if(); // Recursively parse nesed if statements
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

VariableNode *Parser::parse_variable() {
  consume(VAR, "Expected 'var' declaration");
  Node *initializer = parse_expression();
  return new VariableNode(initializer);
}

FunctionNode *Parser::parse_function(bool definition) {
  Token identifier;
  if (definition) {
    consume(FUNCTION, "Expected 'function' before identifier");
    identifier = advance();
  } else {
    identifier = {IDENTIFIER, ""}; // Empty identifier for now, will be set after expression is parsed.
  }

  consume(LEFT_PARENTHESIS, "Expected '(' after identifier");
  std::vector<Node *> parameters;

  while (peek().type != RIGHT_PARENTHESIS) {
    Node *parameter = parse_expression();

    // Make sure function parameter is just an identifier on definition
    if (definition && static_cast<TerminalNode *>(parameter)->token.type != IDENTIFIER)
      throw std::runtime_error("Invalid function parameter");

    parameters.push_back(parameter);
  }
  consume(RIGHT_PARENTHESIS, "Expected ')' after parameters");

  BlockNode *body = nullptr;
  if (definition) {
    body = parse_block();
  }

  return new FunctionNode(identifier, parameters, body);
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
    case VAR: return parse_variable();
    case FUNCTION: return parse_function(true);
    case RETURN: return parse_return();
    default: return parse_expression();
  }
}

Node *Parser::parse() {
  std::vector<Node *> root_statements;

  while (!at_end()) {
    root_statements.push_back(parse_statement());
  }

  return new BlockNode(root_statements);
}

void Parser::traverse(Node *root) {
  if (!root)
    return;

  std::stack<std::pair<Node *, int>> stack;
  stack.push({root, 0});

  while (!stack.empty()) {
    auto [current, depth] = stack.top();
    stack.pop();
    std::string indent(depth * 2, ' ');

    if (auto *bn = dynamic_cast<BlockNode *>(current)) {
      for (auto it = bn->statements.rbegin(); it != bn->statements.rend();
           it++) {
        stack.push({*it, depth});
      }
    } else if (auto *vn = dynamic_cast<VariableNode *>(current)) {
      std::cout << indent << "Variable" << std::endl;
      stack.push({vn->initializer, depth + 1});
    } else if (auto *tn = dynamic_cast<TerminalNode *>(current)) {
      std::cout << indent << tn->token.value << std::endl;
    } else if (auto *un = dynamic_cast<UnaryNode *>(current)) {
      std::cout << indent << un->token.value << std::endl;
      stack.push({un->child, depth + 1});
    } else if (auto *bnn = dynamic_cast<BinaryNode *>(current)) {
      std::cout << indent << bnn->token.value << std::endl;
      stack.push({bnn->right, depth + 1});
      stack.push({bnn->left, depth + 1});
    } else if (auto *in = dynamic_cast<IfNode *>(current)) {
      std::cout << indent << "If" << std::endl;
      stack.push({in->false_body, depth + 1});
      stack.push({in->true_body, depth + 1});
      stack.push({in->condition, depth + 1});
    } else if (auto *wn = dynamic_cast<WhileNode *>(current)) {
      std::cout << indent << "While" << std::endl;
      stack.push({wn->body, depth + 1});
      stack.push({wn->condition, depth + 1});
    } else if (auto *fn = dynamic_cast<ForNode *>(current)) {
      std::cout << indent << "For" << std::endl;
      stack.push({fn->body, depth + 1});
      stack.push({fn->update, depth + 1});
      stack.push({fn->condition, depth + 1});
      stack.push({fn->initialization, depth + 1});
    } else if (auto *fnn = dynamic_cast<FunctionNode *>(current)) {
      std::cout << indent << fnn->identifier.value << "(" << std::endl;
      for (Node *p : fnn->parameters) {
        std::cout << indent << indent;
        traverse(p);
      }
      std::cout << indent << ")" << std::endl;
      stack.push({fnn->body, depth + 1});
    } 
  }
}

bool Parser::at_end() { return pos >= static_cast<int>(tokens.size()); }
