#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/interpreter.h"
#include "../include/token.h"
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
    return 1;
  }

  std::string filename = argv[1];
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return 1;
  }

  std::string code(
      (std::istreambuf_iterator<char>(file)),
      std::istreambuf_iterator<char>()); // Convert input file into string
  Lexer lexer(code);
  std::cout << "Creating Tokens..." << std::endl;
  std::vector<Token> tokens = lexer.tokenize();
  std::cout << "Tokens: ";
  for (Token t : tokens) {
    std::cout << t.value << " ";
  }
  std::cout << std::endl;

  std::cout << "Parsing..." << std::endl;
  Parser parser(tokens);
  BlockNode *ast = parser.parse();
  std::cout << "Parsed." << std::endl;
  std::cout << "Traversing..." << std::endl;
  std::cout << "AST:" << std::endl;
  std::cout << Parser::draw_tree(ast) << std::endl;
  std::cout << "Traversed." << std::endl;
  Interpreter interpreter(ast);
  std::cout << "Interpreting..." << std::endl;
  //interpreter.execute();

  return 0;
}
