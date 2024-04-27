#include "../include/value.h"
#include <stdexcept>

Value *IntValue::apply_operator(Token t, Value *to) {

  switch (t.type) {
    case ADD:
    case SUBTRACT:
    case MULTIPLY:
    case DIVIDE:
    case MODULO:
    case NOT:
    case NEGATIVE:
    case INCREMENT:
    case DECREMENT:
    case RETURN:
    case AND:
    case OR:
    case LESS_THAN:
    case GREATER_THAN:
    case LESS_THAN_OR_EQUAL:
    case GREATER_THAN_OR_EQUAL:
      return 6;
    case EQUAL:
    case NOT_EQUAL:
      return 7;
    default:
      throw std::runtime_error("Unknown Operator");

  }
}

Value *FloatValue::apply_operator(Token t, Value *to) {

}

Value *StringValue::apply_operator(Token t, Value *to) {

}

Value *BoolValue::apply_operator(Token t, Value *to) {

}
