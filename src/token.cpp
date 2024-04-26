#include "../include/token.h"
#include <algorithm>
#include <unordered_set>

bool is_operator(TokenType type) {
  std::unordered_set<TokenType> operators = {
      ADD,
      SUBTRACT,
      MULTIPLY,
      DIVIDE,
      MODULO,
      AND,
      OR,
      NOT,
      NEGATIVE,
      INCREMENT,
      DECREMENT,
      RETURN,
      ASSIGN,
      ASSIGN_ADD,
      ASSIGN_SUBTRACT,
      ASSIGN_MULTIPLY,
      ASSIGN_DIVIDE,
      ASSIGN_MODULO,
      EQUAL,
      NOT_EQUAL,
      LESS_THAN,
      LESS_THAN_OR_EQUAL,
      GREATER_THAN,
      GREATER_THAN_OR_EQUAL,
  };

  return operators.count(type) > 0;
}

bool is_operand(TokenType type) {
  std::unordered_set<TokenType> operands = {IDENTIFIER, INT, FLOAT, STRING, TRUE, FALSE};

  return operands.count(type) > 0;
}

bool is_unary(TokenType type) {
  std::unordered_set<TokenType> unary_operators = {NOT, NEGATIVE, INCREMENT, DECREMENT, RETURN};

  return unary_operators.count(type) > 0;
}


int precedence(TokenType type) {
  switch (type) {
  case ADD:
  case SUBTRACT:
    return 1;
  case MULTIPLY:
  case DIVIDE:
  case MODULO:
    return 2;
  case NOT:
  case NEGATIVE:
  case INCREMENT:
  case DECREMENT:
  case RETURN:
    return 3;
  case LESS_THAN:
  case GREATER_THAN:
  case LESS_THAN_OR_EQUAL:
  case GREATER_THAN_OR_EQUAL:
    return 4;
  case EQUAL:
  case NOT_EQUAL:
    return 5;
  case AND:
    return 6;
  case OR:
    return 7;
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
    {"-", {NEGATIVE, "-"}},
    {"++", {INCREMENT, "++"}},
    {"--", {DECREMENT, "--"}},
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
