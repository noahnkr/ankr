#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/token.h"
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <utility>

void traverse(Node *root) {
  if (!root) return;

  std::stack<std::pair<Node *, int>> stack;
  stack.push({root, 0});

  while (!stack.empty()) {
    auto [current, depth] = stack.top();
    stack.pop();
    std::string indent(depth * 2, ' ');

    if (auto *bn = dynamic_cast<BlockNode *>(current)) {
      std::cout << indent << "Block" << std::endl;
      for (auto it = bn->statements.rbegin(); it != bn->statements.rend(); it++) {
        stack.push({*it, depth + 1});
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
    }
  }
}

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
  std::vector<Token> tokens = lexer.tokenize();
  std::cout << "Tokens: " << std::endl;
  for (Token t : tokens) {
    std::cout << t.value << " ";
  }
  std::cout << std::endl;

  std::cout << "Parsing..." << std::endl;
  Parser parser(tokens);
  Node *root = parser.parse();
  std::cout << "Parsed." << std::endl;
  traverse(root);
  // Interpreter interpreter;
  // interpreter.visit(root);

  return 0;
}
