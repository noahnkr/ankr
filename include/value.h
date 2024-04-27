#include "token.h"
#include <iostream>
#include <memory>
#include <string>

class Value {
public:
  virtual ~Value() {}
  virtual std::string to_string() const = 0;
  virtual Value* apply_operator(Token op, Value *to) = 0;
};

class IntValue : public Value {
public:
  int value;

  IntValue(int value) : value(value) {}
  std::string to_string() const override { return std::to_string(value); }
  Value* apply_operator(Token op, Value *to) override;
};

class FloatValue : public Value {
public:
  double value;

  FloatValue(float value) : value(value) {}
  std::string to_string() const override { return std::to_string(value); }
  Value* apply_operator(Token op, Value *to) override;
};

class StringValue : public Value {
public:
  std::string value;

  StringValue(std::string value) : value(std::move(value)) {}
  std::string to_string() const override { return value; }
  Value* apply_operator(Token op, Value *to) override;
};

class BoolValue : public Value {
public:
  bool value;

  BoolValue(bool value) : value(value) {}
  std::string to_string() const override { return value ? "true" : "false"; }
  Value* apply_operator(Token op, Value *to) override;
};

