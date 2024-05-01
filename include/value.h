#ifndef VALUE_H
#define VALUE_H

#include "token.h"
#include <iostream>
#include <memory>
#include <string>

/**
 * Abstract base class for all value types in the interpreter.
 * Provides the interface for converting values to strings, getting the type name,
 * and applying operators between values.
 */
class Value {
public:
  virtual ~Value() {}

  /**
   * Converts the value to a string representation.
   * @return String representation of the value.
   */
  virtual std::string to_string() const = 0;

  /**
   * Retrieves the type name of the value as a string.
   * @return The type name of the value.
   */
  virtual std::string get_type() const = 0;

  /**
   * Applies an operator to this value and another value.
   * @param op The operator as a token.
   * @param to The value to apply the operator with.
   * @return A new Value instance that is the result of the operation.
   */
  virtual Value* apply_operator(Token op, Value *to) = 0;
};

/**
 * Represents an integer value in the interpreter.
 */
class IntValue : public Value {
public:
  int value; ///< The integer value.

  explicit IntValue(int value) : value(value) {}

  std::string to_string() const override;
  std::string get_type() const override;
  Value* apply_operator(Token op, Value *to) override;
};

/**
 * Represents a floating-point value in the interpreter.
 */
class FloatValue : public Value {
public:
  double value; ///< The floating-point value.

  explicit FloatValue(float value) : value(value) {}

  std::string to_string() const override;
  std::string get_type() const override;
  Value* apply_operator(Token op, Value *to) override;
};

/**
 * Represents a string value in the interpreter.
 */
class StringValue : public Value {
public:
  std::string value; ///< The string value.

  explicit StringValue(std::string value) : value(std::move(value)) {}

  std::string to_string() const override;
  std::string get_type() const override;
  Value* apply_operator(Token op, Value *to) override;
};

/**
 * Represents a boolean value in the interpreter.
 */
class BoolValue : public Value {
public:
  bool value; ///< The boolean value.

  explicit BoolValue(bool value) : value(value) {}

  std::string to_string() const override;
  std::string get_type() const override;
  Value* apply_operator(Token op, Value *to) override;
};

/**
 * Represents a void value in the interpreter, typically used to signify no return value.
 */
class VoidValue : public Value {
public:
  int val;

  VoidValue() : val(0) {}

  std::string to_string() const override;
  std::string get_type() const override;
  Value* apply_operator(Token op, Value *to) override;
};

#endif // VALUE_H
