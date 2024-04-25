#ifndef TOKEN_H
#define TOKEN_H

#include <map>
#include <string>

enum TokenType {
  // Keywords
  IF,
  ELSE,
  WHILE,
  FOR,
  FUNCTION,
  VAR,
  RETURN,
  TRUE,
  FALSE,
  BREAK,
  // Literals
  INT,
  FLOAT,
  STRING,
  // Operators and Punctuation
  ADD,
  SUBTRACT,
  MULTIPLY,
  DIVIDE,
  MODULO,
  NEGATIVE,
  LEFT_PARENTHESIS,
  RIGHT_PARENTHESIS,
  LEFT_BRACKET,
  RIGHT_BRACKET,
  COMMA,
  DOT,
  // Assignment
  ASSIGN,
  ASSIGN_ADD,
  ASSIGN_SUBTRACT,
  ASSIGN_MULTIPLY,
  ASSIGN_DIVIDE,
  ASSIGN_MODULO,
  // Boolean Logic
  EQUAL,
  NOT_EQUAL,
  GREATER_THAN,
  LESS_THAN,
  GREATER_THAN_OR_EQUAL,
  LESS_THAN_OR_EQUAL,
  AND,
  OR,
  NOT,
  // Other
  END_STATEMENT,
  END_FILE,
  COMMENT,
  IDENTIFIER
};

struct Token {
  TokenType type;
  std::string value;
};

extern bool is_operator(TokenType type);

extern bool is_operand(TokenType type);

extern int precedence(TokenType type);

extern std::map<std::string, Token> token_map;

#endif // TOKEN_H
