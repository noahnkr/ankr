#include "../include/interpreter.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>

int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
    return 1;
  }


  // Enable debug mode from command line
  bool debug_mode = false;
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-d") == 0) {
      debug_mode = true;
    }
  }

  std::string filename = argv[1];
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return 1;
  }

  // Convert file into string of text
  std::string code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()); 

  Interpreter interpreter(code, debug_mode);
  interpreter.execute();

  return 0;
}
