#include "../include/parser.h"
#include <stack>
#include <stdexcept>
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

  throw std::runtime_error(message); //TODO: Add handling and output for errors.
}

Node *Parser::parse_expression() {
  int parenthesis_index = 0;
  std::vector<Token> infix;

  // Keep reading tokens until we reach a semicolon or the wrapping right
  // parenthesis.
  while (!(at_end() || peek().type == END_STATEMENT ||
           (peek().type == RIGHT_PARENTHESIS && parenthesis_index == 0))) {
    Token t = advance();

    if (t.type == LEFT_PARENTHESIS) {
      parenthesis_index++;
    } else if (t.type == RIGHT_PARENTHESIS) {
      parenthesis_index--;
    }

    infix.push_back(t);
  }

  // Skip semicolon
  if (peek().type == END_STATEMENT) {
    advance();
  }

  // Convert tokens to postfix
  std::vector<Token> postfix = to_postfix(infix);
  std::cout << "Postfix: ";
  for (Token t : postfix) {
    std::cout << t.value << " ";
  }
  std::cout << std::endl;

  // Build Expression Tree
  std::stack<Node *> stack;
  for (Token t : postfix) {
    if (is_operand(t.type)) {
      stack.push(new TerminalNode(t));
    } else if (is_operator(t.type)) {
      // Unary Operators
      if (t.type == NOT || t.type == NEGATIVE) {
        Node *operand = stack.top();
        stack.pop();
        stack.push(new UnaryNode(t, operand));

        // Binary Operators
      } else {
        Node *right = stack.top();
        stack.pop();
        Node *left = stack.top();
        stack.pop();
        stack.push(new BinaryNode(t, left, right));
      }
    }
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
      // Function call
      if (i + 1 < infix.size() && infix[i + 1].type == LEFT_PARENTHESIS) {
      // Operand
      } else {
        postfix.push_back(t);
      }
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

Node *Parser::parse_if() {
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

Node *Parser::parse_while() {
  consume(WHILE, "Expected 'while'");
  consume(LEFT_PARENTHESIS, "Expected '(' after 'while'");
  Node *condition = parse_expression();
  consume(RIGHT_PARENTHESIS, "Expected ')' after 'while' condition");
  Node* body = parse_block();
  return new WhileNode(condition, body);
}

Node *Parser::parse_variable() {
  consume(VAR, "Expected 'var' declaration");
  Node *initializer = parse_expression();
  return new VariableNode(initializer);
}

Node *Parser::parse_function() {
  return nullptr;
}

Node *Parser::parse_for() {
  return nullptr;
}

Node *Parser::parse_block() {
  std::cout << "Parsing: Block" << std::endl;
  std::vector<Node *> statements;

  consume(LEFT_BRACKET, "Expected '{' at start of block");

  while (!(at_end() || peek().type == RIGHT_BRACKET)) {
    statements.push_back(parse_statement());
  }

  consume(RIGHT_BRACKET, "Expected '}' at end of block");

  return new BlockNode(statements);
}


Node *Parser::parse_statement() {
  std::cout << "Parsing Statement: " << peek().value << std::endl;
  switch (peek().type) {
    case IF: return parse_if();
    case WHILE: return parse_while();
    case FOR: return parse_for();
    case VAR: return parse_variable();
    case FUNCTION: return parse_function();
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

bool Parser::at_end() { return pos >= static_cast<int>(tokens.size()); }
