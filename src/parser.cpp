#include "../include/parser.h"
#include <stack>
#include <stdexcept>
#include <vector>

Parser::Parser(std::vector<Token> tokens) : tokens(tokens), pos(0) {}

bool Parser::match(TokenType expected) {
  if (at_end())
    return false;
  if (tokens[pos].type == expected) {
    pos++;
    return true;
  }
  return false;
}

Token Parser::peek() {
  if (at_end())
    return {END_FILE, ""};
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

  throw std::runtime_error(message); // technically a compile-time error.
}

Node *Parser::parse_block() {
  if (match(IF))
    return parse_if();
  if (match(WHILE))
    return parse_while();
  if (match(FOR))
    return parse_for();
}

Node *Parser::parse_expression() {
  int parenthesis_index = 0;
  std::vector<Token> infix;

  // Keep reading tokens until we reach a semicolon or the wrapping right
  // parenthesis.
  while (!(at_end() || peek().type == END_STATEMENT ||
           (peek().type == RIGHT_PARENTHESIS && parenthesis_index == 0))) {
    Token t = advance();
    if (t.type == LEFT_PARENTHESIS)
      parenthesis_index++;
    else if (t.type == RIGHT_PARENTHESIS)
      parenthesis_index--;

    infix.push_back(t);
  }

  // Convert tokens to postfix
  std::vector<Token> postfix = to_postfix(infix);

  // Build Expression Tree
  std::stack<Node *> stack;
  for (Token t : postfix) {
    if (is_operand(t.type)) {
      stack.push(new TerminalNode(t));
    } else if (is_operator(t.type)) {
      // Unary Operators
      if (t.type == NOT /*|| t.type == NEGATIVE*/) {
        stack.push(new UnaryNode(t, stack.top()));
        stack.pop();
        continue;
      }

      // Binary Operators
      Node *right = stack.top();
      stack.pop();
      Node *left = stack.top();
      stack.pop();
      stack.push(new BinaryNode(t, left, right));
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

  for (Token t : infix) {
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

Node *Parser::parse_if() {
  consume(LEFT_PARENTHESIS, "Expect '(' after 'if'.");
  Node *condition = parse_expression();
  consume(RIGHT_PARENTHESIS, "Expect ')' after  'if' condition.");
  consume(LEFT_BRACKET, "Expected '{' after condition");
  Node *if_body = parse_block();
  consume(RIGHT_BRACKET, "Expected '}' after if body");
}

Node *Parser::parse() {
  std::vector<Node *> root_statements;

  while (!at_end()) {
    root_statements.push_back(parse_block());
  }

  return new BlockNode(root_statements);
}

bool Parser::at_end() { return pos >= tokens.size(); }
