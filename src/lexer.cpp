#include "../include/lexer.h"
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

Lexer::Lexer(std::string code) : code(code), pos(0), current_char(code[0]) {}

void Lexer::advance() {
  pos++;
  current_char = pos < code.size() ? code[pos] : EOF;
}

char Lexer::peek() {
  size_t next_pos = pos + 1;
  return next_pos < code.size() ? code[next_pos] : EOF;
}

void Lexer::consume_whitespace() {
  while (current_char != EOF && isspace(current_char)) {
    advance();
  }
}

Token Lexer::consume_number() {
  std::string num;
  TokenType type = INT;
  while (std::isdigit(current_char) || current_char == '.') {
    if (current_char == '.') {
      type = FLOAT;
    }
    num += current_char;
    advance();
  }
  std::cout << "Number: " << num << std::endl;
  return {type, num};
}

Token Lexer::consume_string() {
  std::string str;
  advance(); // Skip opening quote
  while (current_char != '\"' && current_char != EOF) {
    str += current_char;
    advance();
  }
  advance(); // Skip closing quote
  std::cout << "String: " << str << std::endl;
  return {STRING, str};
}

std::vector<Token> Lexer::tokenize() {
  std::vector<Token> tokens;
  while (current_char != EOF) {
    if (current_char != ' ' && current_char != '\n') {
      std::cout << "Current Char: " << current_char << ", ";
    }
    // Space
    if (std::isspace(current_char)) {
      consume_whitespace();

      // Integer / Float
    } else if (std::isdigit(current_char)) {
      tokens.push_back(consume_number());

      // String
    } else if (current_char == '\"') {
      tokens.push_back(consume_string());

      // Equality Operators
    } else if (current_char == '>' || current_char == '<' ||
               current_char == '=' || current_char == '!' ||
               current_char == '+' || current_char == '-' ||
               current_char == '*' || current_char == '/') {
      std::string value;
      value += current_char;
      if (peek() == '=') {
        value += '=';
        advance();
      }
      std::cout << "Token: " << value << std::endl;
      tokens.push_back(token_map.at(value));
      advance();

    } else if (current_char == '!') {
      tokens.push_back({NOT, "!"});
      advance();

    } else {
      std::string value;
      do {
        value += current_char;
        advance();

      } while (token_map.find(value) == token_map.end() &&
               !std::isspace(current_char) && current_char != ';' &&
               current_char != '(' && current_char != ')' &&
               current_char != EOF);

      // Token found
      if (token_map.find(value) != token_map.end()) {
        std::cout << "Token: " << value << std::endl;
        tokens.push_back(token_map.at(value));

        // Otherwise, must be an identifier
      } else {
        std::cout << "Identifier: " << value << std::endl;
        tokens.push_back({IDENTIFIER, value});
      }
    }
  }

  return tokens;
}
