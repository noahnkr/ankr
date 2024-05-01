#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "token.h"
#include <vector>

/**
 * The Parser class takes a sequence of tokens and constructs an abstract syntax tree (AST).
 * It supports parsing a variety of constructs including blocks, control flow statements, 
 * variable and function declarations, and expressions.
 */
class Parser {
private:
  std::vector<Token> tokens; ///< Tokens to be parsed.
  size_t pos; ///< Current position in the tokens vector.
  bool debug_mode; ///< Indicates whether to log parsing steps for debugging.

  /**
   * Parses a block of statements enclosed by curly braces.
   * @return Pointer to a BlockNode representing the parsed block.
   */
  BlockNode *parse_block();

  /**
   * Parses an 'if' statement, potentially followed by 'else if' and 'else' blocks.
   * @return Pointer to an IfNode representing the parsed conditional structure.
   */
  IfNode *parse_if();

  /**
   * Parses a 'while' loop statement.
   * @return Pointer to a WhileNode representing the parsed loop.
   */
  WhileNode *parse_while();

  /**
   * Parses a 'for' loop statement.
   * @return Pointer to a ForNode representing the parsed loop.
   */
  ForNode *parse_for();

  /**
   * Parses a variable declaration or usage.
   * @param is_definition Specifies whether the variable is being declared (true) or referenced (false).
   * @return Pointer to a VariableNode representing the parsed variable.
   */
  VariableNode *parse_variable(bool is_definition);

  /**
   * Parses a function declaration or function call.
   * @param is_definition Specifies whether the function is being declared (true) or called (false).
   * @return Pointer to a FunctionNode representing the parsed function.
   */
  FunctionNode *parse_function(bool is_definition);

  /**
   * Parses a return statement.
   * @return Pointer to a UnaryNode representing the parsed return statement.
   */
  UnaryNode *parse_return();

  /**
   * Parses a general expression.
   * @return Node* representing the root of the expression's AST.
   */
  Node *parse_expression();

  /**
   * Parses a single statement which can be any construct recognized by the parser.
   * @return Node* representing the parsed statement.
   */
  Node *parse_statement();

  /**
   * Converts an infix expression to postfix notation to facilitate easier parsing.
   * @param infix A vector of tokens representing an infix expression.
   * @return A vector of tokens in postfix order.
   */
  std::vector<Token> to_postfix(std::vector<Token> infix);

  /**
   * Peeks at the next token without advancing the parser.
   * @return The next token or a token of type END_FILE if at the end.
   */
  Token peek();

  /**
   * Advances to the next token and returns the current token before moving.
   * @return The current token before advancing.
   */
  Token advance();

  /**
   * Consumes the next token if it matches the expected type, otherwise throws an error.
   * @param expected The expected TokenType.
   * @param message Error message to throw if the token does not match.
   */
  void consume(TokenType expected, std::string message);

  /**
   * Helper function to draw the AST recursively.
   * @param node The current node to draw.
   * @param tree Reference to a string where the drawing accumulates.
   * @param padding Padding for current recursion depth.
   * @param pointer Graphical pointer for tree drawing.
   * @param has_next Indicates whether there are more siblings after this node (affects drawing).
   */
  static void draw_tree_rec(Node *node, std::string *tree, std::string padding, std::string pointer, bool has_next);

  /**
   * Checks if all tokens have been processed.
   * @return True if all tokens are processed, false otherwise.
   */
  bool at_end();

public:
  /**
   * Constructs a Parser with a list of tokens to parse and an optional debug mode.
   * @param tokens Vector of tokens to parse.
   * @param debug_mode Whether to output debug information during parsing.
   */
  Parser(std::vector<Token> tokens, bool debug_mode);
  
  /**
   * Destructor.
   */
  ~Parser();

  /**
   * Parses the tokens into an AST.
   * @return A BlockNode that is the root of the constructed AST.
   */
  BlockNode *parse();

  /**
   * Draws the AST in a human-readable form.
   * @param root Root of the AST to draw.
   * @return A string representing the graphical view of the AST.
   */
  static std::string draw_tree(BlockNode *root);

};

#endif // PARSER_H
