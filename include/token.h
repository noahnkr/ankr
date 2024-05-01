#ifndef TOKEN_H
#define TOKEN_H

#include <map>
#include <string>

/**
 * @enum TokenType
 * @brief Defines possible categories of tokens that can appear in the source code.
 *
 * This enumeration lists all the types of tokens that the lexer can produce, including
 * keywords, literals, operators, punctuation, and special symbols.
 */
enum TokenType {
  // Keywords
  IF, ELSE, WHILE, FOR, FUNCTION, VAR, RETURN, TRUE, FALSE, BREAK,
  // Literals
  INT, FLOAT, STRING,
  // Operators and Punctuation
  ADD, SUBTRACT, MULTIPLY, DIVIDE, MODULO, NEGATIVE, INCREMENT, DECREMENT,
  LEFT_PARENTHESIS, RIGHT_PARENTHESIS, LEFT_BRACKET, RIGHT_BRACKET, COMMA, DOT,
  // Assignment Operators
  ASSIGN, ASSIGN_ADD, ASSIGN_SUBTRACT, ASSIGN_MULTIPLY, ASSIGN_DIVIDE, ASSIGN_MODULO,
  // Boolean Operators
  EQUAL, NOT_EQUAL, GREATER_THAN, LESS_THAN, GREATER_THAN_OR_EQUAL, LESS_THAN_OR_EQUAL, AND, OR, NOT,
  // IO Keywords
  OUTPUT, INPUT,
  // Miscellaneous
  END_STATEMENT, END_FILE, COMMENT, IDENTIFIER
};

/**
 * @struct Token
 * @brief Represents a token in the source code with a specific type and associated string value.
 *
 * Each token object contains a type, which categorizes the token, and a value, which provides
 * the specific text from the source code corresponding to this token.
 */
struct Token {
  TokenType type;      ///< Type of the token.
  std::string value;   ///< The textual value of the token.
};

/**
 * @brief Checks if the given TokenType is an operator.
 * @param type TokenType to check.
 * @return true if the token type is an operator, false otherwise.
 */
extern bool is_operator(TokenType type);

/**
 * @brief Checks if the given TokenType is an operand.
 * @param type TokenType to check.
 * @return true if the token type is an operand, false otherwise.
 */
extern bool is_operand(TokenType type);

/**
 * @brief Checks if the given TokenType is a unary operator.
 * @param type TokenType to check.
 * @return true if the token type is a unary operator, false otherwise.
 */
extern bool is_unary(TokenType type);

/**
 * @brief Checks if the given TokenType is an assignment operator.
 * @param type TokenType to check.
 * @return true if the token type is an assignment operator, false otherwise.
 */
extern bool is_assign(TokenType type);

/**
 * @brief Determines the precedence level of an operator token.
 * @param type TokenType of the operator.
 * @return Integer indicating the precedence level, with lower numbers being higher precedence.
 */
extern int precedence(TokenType type);

/**
 * @brief A map from string representations of tokens to their corresponding Token objects.
 * This map is used to quickly translate characters and strings from the source code into tokens.
 */
extern std::map<std::string, Token> token_map;

#endif // TOKEN_H
