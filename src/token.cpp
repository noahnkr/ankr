#include "../include/token.h"
#include <vector>
#include <algorithm>

bool is_operator(TokenType type) {
  std::vector<TokenType> operators = {ADD,
                                      SUBTRACT,
                                      MULTIPLY,
                                      DIVIDE,
                                      MODULO,
                                      ASSIGN,
                                      ASSIGN_ADD,
                                      ASSIGN_SUBTRACT,
                                      ASSIGN_MULTIPLY,
                                      ASSIGN_DIVIDE,
                                      ASSIGN_MODULO,
                                      AND,
                                      OR,
                                      NOT};

  return std::find(operators.begin(), operators.end(), type) != operators.end();
}

bool is_operand(TokenType type) {
  std::vector<TokenType> operands = {IDENTIFIER, INT, FLOAT, STRING, TRUE, FALSE};

  return std::find(operands.begin(), operands.end(), type) != operands.end();
}

int precedence(TokenType type) {
  switch (type) {
  case ADD:
  case SUBTRACT:
  case NOT:
    return 1;
  case MULTIPLY:
  case DIVIDE:
  case MODULO:
    return 2;
  case LESS_THAN:
  case GREATER_THAN:
  case LESS_THAN_OR_EQUAL:
  case GREATER_THAN_OR_EQUAL:
    return 3;
  case EQUAL:
  case NOT_EQUAL:
    return 4;
  case AND:
  case OR:
    return 5;
  default:
    return -1;
  }
}

std::map<std::string, Token> token_map = {
    // Keywords
    {"if", {IF, "if"}},
    {"else", {ELSE, "else"}},
    {"while", {WHILE, "while"}},
    {"for", {FOR, "for"}},
    {"function", {FUNCTION, "function"}},
    {"var", {VAR, "var"}},
    {"return", {RETURN, "return"}},
    {"true", {TRUE, "true"}},
    {"false", {FALSE, "false"}},
    {"break", {BREAK, "break"}},
    // Operators and Punctuation
    {"+", {ADD, "+"}},
    {"-", {SUBTRACT, "-"}},
    {"*", {MULTIPLY, "*"}},
    {"/", {DIVIDE, "/"}},
    {"%", {MODULO, "%"}},
    {"(", {LEFT_PARENTHESIS, "("}},
    {")", {RIGHT_PARENTHESIS, ")"}},
    {"{", {LEFT_BRACKET, "{"}},
    {"}", {RIGHT_BRACKET, "}"}},
    {",", {COMMA, ","}},
    {".", {DOT, "."}},
    // Assignment
    {"=", {ASSIGN, "="}},
    {"+=", {ASSIGN_ADD, "+="}},
    {"-=", {ASSIGN_SUBTRACT, "-="}},
    {"*=", {ASSIGN_MULTIPLY, "*="}},
    {"/=", {ASSIGN_DIVIDE, "/="}},
    {"%=", {ASSIGN_MODULO, "%="}},
    // Boolean Logic
    {"==", {EQUAL, "=="}},
    {"!=", {NOT_EQUAL, "!="}},
    {">", {GREATER_THAN, ">"}},
    {"<", {LESS_THAN, "<"}},
    {">=", {GREATER_THAN_OR_EQUAL, ">="}},
    {"<=", {LESS_THAN_OR_EQUAL, "<="}},
    {"&&", {AND, "&&"}},
    {"||", {OR, "||"}},
    {"!", {NOT, "!"}},
    // Other
    {";", {END_STATEMENT, ";"}},
    {"//", {COMMENT, "//"}}};
